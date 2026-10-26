# ⚖️ Bước 4: Động Cơ Quản Trị Rủi Ro (Risk Management Engine)

> **Mục tiêu**: Kết hợp kết quả từ **Bộ luật gian lận** (Bước 2) + **Xác suất học máy** (Bước 3) + **Phân tầng khách hàng KYC** thành điểm rủi ro tổng hợp ($0.0 - 100.0$). Điểm rủi ro phải có tính giải thích minh bạch (Explainability), chỉ rõ từng yếu tố đã đóng góp bao nhiêu điểm.  
> **Thư mục làm việc**: `include/engine/risk/` và `sources/engine/risk/`.

---

## 📐 1. Thiết Kế `RiskPolicy` (Chính Sách & Trọng Số Rủi Ro)

Theo quy định kiến trúc, **không được hardcode các con số ma thuật (magic numbers)** rải rác trong code. Toàn bộ trọng số và ngưỡng phân tầng phải nằm trong `RiskPolicy`.

### File: `include/engine/risk/RiskPolicy.h`
```cpp
#pragma once
#include "core/common/Types.h"

struct RiskPolicy {
    // --- Trọng số các nhân tố (Tổng = 1.0) ---
    double weight_rules{0.45};       // 45% từ vi phạm các luật gian lận
    double weight_ml{0.45};          // 45% từ mô hình xác suất học máy
    double weight_profile{0.10};     // 10% từ hồ sơ khách hàng / phân tầng KYC

    // --- Các ngưỡng phân tầng mức độ rủi ro (RiskLevel) ---
    double threshold_very_low{20.0}; // [0, 20) -> VERY_LOW
    double threshold_low{40.0};      // [20, 40) -> LOW
    double threshold_medium{60.0};   // [40, 60) -> MEDIUM
    double threshold_high{80.0};     // [60, 80) -> HIGH
                                     // [80, 100] -> CRITICAL

    // Hàm ánh xạ điểm số [0, 100] sang enum RiskLevel
    RiskLevel determineRiskLevel(double score) const {
        if (score < threshold_very_low) return RiskLevel::VERY_LOW;
        if (score < threshold_low)      return RiskLevel::LOW;
        if (score < threshold_medium)   return RiskLevel::MEDIUM;
        if (score < threshold_high)     return RiskLevel::HIGH;
        return RiskLevel::CRITICAL;
    }
};
```

---

## 🧮 2. Thiết Kế `RiskEngine` (`RiskEngine.h` & `RiskEngine.cpp`)

`RiskEngine` nhận vào toàn bộ dữ kiện từ các bước trước và sản sinh ra đối tượng `RiskAssessment`.

### File: `include/engine/risk/RiskEngine.h`
```cpp
#pragma once
#include "RiskPolicy.h"
#include "core/domain/transaction/Transaction.h"
#include "core/domain/account/Customer.h"
#include "core/domain/alert/RiskAssessment.h"
#include "core/domain/alert/FraudAlert.h"
#include "DSA/vector.h"
#include <string>

class RiskEngine {
private:
    RiskPolicy policy;

public:
    explicit RiskEngine(const RiskPolicy& custom_policy = RiskPolicy())
        : policy(custom_policy) {}

    void setPolicy(const RiskPolicy& new_policy) { policy = new_policy; }
    const RiskPolicy& getPolicy() const { return policy; }

    /**
     * @brief Đánh giá rủi ro tổng thể và tạo đối tượng RiskAssessment.
     * @param tx Giao dịch cần đánh giá.
     * @param customer Hồ sơ khách hàng.
     * @param alerts Danh sách cảnh báo đã thu thập từ FraudRuleEngine.
     * @param raw_rule_score Tổng điểm đóng góp của các luật.
     * @param ml_probability Xác suất gian lận từ ML [0.0, 1.0].
     * @return RiskAssessment Đối tượng đánh giá rủi ro hoàn chỉnh.
     */
    RiskAssessment assessRisk(
        const Transaction& tx,
        const Customer& customer,
        const Vector<FraudAlert>& alerts,
        double raw_rule_score,
        double ml_probability
    ) const;
};
```

### File: `sources/engine/risk/RiskEngine.cpp`
```cpp
#include "engine/risk/RiskEngine.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

RiskAssessment RiskEngine::assessRisk(
    const Transaction& tx,
    const Customer& customer,
    const Vector<FraudAlert>& alerts,
    double raw_rule_score,
    double ml_probability
) const {
    // 1. Chuẩn hóa điểm từng phần
    // Điểm luật tối đa 100
    double normalized_rule_score = std::min(100.0, raw_rule_score);
    // Điểm ML quy về thang 100
    double normalized_ml_score = std::min(100.0, ml_probability * 100.0);
    // Điểm rủi ro cơ sở của khách hàng (phụ thuộc phân tầng KYC)
    double customer_profile_score = customer.getBaselineRiskScore();

    // 2. Tính điểm tổng hợp (Weighted Combination)
    double combined_score = (normalized_rule_score * policy.weight_rules) +
                            (normalized_ml_score * policy.weight_ml) +
                            (customer_profile_score * policy.weight_profile);

    // Chặn trong khoảng [0.0, 100.0]
    combined_score = std::max(0.0, std::min(100.0, combined_score));

    // 3. Xác định RiskLevel
    RiskLevel level = policy.determineRiskLevel(combined_score);

    // 4. Khởi tạo đối tượng RiskAssessment
    std::string assessment_id = "RSK_" + tx.GetTransaction_ID();
    
    // Tạm thời để DecisionAction::APPROVE, sẽ do DecisionEngine cập nhật ở Bước 5
    Vector<std::string> reasons;

    // 5. Tính minh bạch & Giải thích (Explainability Log)
    for (int i = 0; i < alerts.size(); ++i) {
        std::ostringstream oss;
        oss << "[RULE_ALERT] " << alerts[i].getRuleName() 
            << " (+" << alerts[i].getRiskContribution() << " pts): " 
            << alerts[i].getReason();
        reasons.push_back(oss.str());
    }

    if (ml_probability > 0.30) {
        std::ostringstream oss;
        oss << "[ML_MODEL] Fraud probability: " 
            << std::fixed << std::setprecision(1) << (ml_probability * 100.0) 
            << "% (+ " << (normalized_ml_score * policy.weight_ml) << " pts to total)";
        reasons.push_back(oss.str());
    }

    if (customer.getRiskTier() == CustomerRiskTier::NEW || customer.getRiskTier() == CustomerRiskTier::SUSPICIOUS) {
        std::ostringstream oss;
        oss << "[CUSTOMER_PROFILE] Tier " << toString(customer.getRiskTier()) 
            << " (+ " << customer_profile_score << " baseline pts)";
        reasons.push_back(oss.str());
    }

    // 6. Tạo đối tượng kết quả
    RiskAssessment assessment(
        assessment_id,
        tx.GetTransaction_ID(),
        normalized_rule_score,
        normalized_ml_score,
        combined_score,
        level,
        DecisionAction::APPROVE, // Sẽ override bởi DecisionEngine
        alerts,
        reasons,
        std::chrono::system_clock::now()
    );

    return assessment;
}
```

---

## 🎯 3. Tiêu Chuẩn Nghiệm Thu Bước 4
- [ ] Không có magic number: Toàn bộ công thức dùng các tham số từ `RiskPolicy`.
- [ ] Kiểm tra khả năng giải thích (Explainability): Khi in `assessment.getReasons()`, hiển thị rõ từng luật bị vi phạm và phần trăm đóng góp của mô hình học máy.
- [ ] Điểm rủi ro kết hợp luôn nằm nghiêm ngặt trong đoạn $[0.0, 100.0]$.

---
👉 **Tiếp tục với Bước 5: [05_DECISION_ENGINE_VA_TEST.md](file:///d:/Fraud-Detection---Risk-management/docs/engine/05_DECISION_ENGINE_VA_TEST.md)**
