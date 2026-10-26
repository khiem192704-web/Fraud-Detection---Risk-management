# 🛡️ Bước 2: Động Cơ Luật Gian Lận (Rule-Based Fraud Engine)

> **Mục tiêu**: Xây dựng kiến trúc luật linh hoạt (Strategy Pattern) cho phép thêm, bớt hoặc cấu hình các luật gian lận tất định. Bộ gom luật (Composite Pattern) sẽ duyệt qua tất cả các luật và tổng hợp danh sách `FraudAlert`.  
> **Thư mục làm việc**: `include/engine/rules/`, `sources/engine/rules/`, `include/engine/fraud/`, và `sources/engine/fraud/`.

---

## 🏛️ 1. Thiết Kế Giao Diện Cơ Sở `IFraudRule`

Mỗi luật gian lận là một lớp kế thừa từ `IFraudRule`, độc lập và dễ dàng viết Unit Test riêng biệt.

### File: `include/engine/rules/IFraudRule.h`
```cpp
#pragma once
#include <string>
#include "core/common/Types.h"
#include "core/domain/transaction/Transaction.h"
#include "core/domain/account/Customer.h"
#include "core/domain/alert/FraudAlert.h"
#include "engine/features/FeatureVector.h"

class IFraudRule {
public:
    virtual ~IFraudRule() = default;

    // Định danh duy nhất cho luật (vd: "RULE_001_VELOCITY")
    virtual std::string getId() const = 0;

    // Tên mô tả ngắn gọn của luật (vd: "High Velocity Detected")
    virtual std::string getName() const = 0;

    // Nhóm danh mục luật theo chuẩn PCI-DSS / Risk Management
    virtual FraudRuleCategory getCategory() const = 0;

    // Số điểm rủi ro đóng góp nếu luật này bị vi phạm (0.0 -> 100.0)
    virtual double getRiskContribution() const = 0;

    /**
     * @brief Đánh giá xem giao dịch có vi phạm luật hay không.
     * @param tx Giao dịch đang xét
     * @param customer Khách hàng thực hiện
     * @param features Vector đặc trưng đã trích xuất từ Bước 1
     * @param out_alert Cảnh báo FraudAlert được tạo ra nếu vi phạm
     * @return true nếu VI PHẠM (kích hoạt cảnh báo), false nếu an toàn
     */
    virtual bool evaluate(
        const Transaction& tx,
        const Customer& customer,
        const FeatureVector& features,
        FraudAlert& out_alert
    ) = 0;
};
```

---

## 📜 2. Cài Đặt Bộ 7 Luật Gian Lận Cốt Lõi (`sources/engine/rules/`)

Các luật được gom trong thư mục `engine/rules/`. Dưới đây là hiện thực cụ thể của các luật:

### 1. `LargeAmountRule` (Đột biến số tiền giao dịch)
- **Tiêu chí**: Số tiền $> \$3,000$ HOẶC số tiền gấp $> 5$ lần mức chi tiêu trung bình lịch sử.
- **File**: `include/engine/rules/LargeAmountRule.h`
```cpp
#pragma once
#include "IFraudRule.h"

class LargeAmountRule : public IFraudRule {
private:
    double threshold_amount{3000.0};
    double threshold_ratio{5.0};

public:
    LargeAmountRule(double threshold = 3000.0, double ratio = 5.0) 
        : threshold_amount(threshold), threshold_ratio(ratio) {}

    std::string getId() const override { return "RULE_001_LARGE_AMOUNT"; }
    std::string getName() const override { return "Large Amount Deviation"; }
    FraudRuleCategory getCategory() const override { return FraudRuleCategory::AMOUNT_DEVIATION; }
    double getRiskContribution() const override { return 20.0; }

    bool evaluate(const Transaction& tx, const Customer&, const FeatureVector& fv, FraudAlert& out_alert) override {
        if (fv.amount >= threshold_amount || (fv.amount_ratio_to_avg >= threshold_ratio && fv.amount > 500.0)) {
            out_alert = FraudAlert(
                "ALT_" + tx.GetTransaction_ID() + "_AMT",
                tx.GetTransaction_ID(),
                getId(),
                getName(),
                getCategory(),
                getRiskContribution(),
                "Transaction amount $" + std::to_string(fv.amount) + " deviates significantly from history",
                RiskLevel::HIGH,
                std::chrono::system_clock::now()
            );
            return true;
        }
        return false;
    }
};
```

### 2. `HighVelocityRule` (Tần suất giao dịch bất thường)
- **Tiêu chí**: $\ge 4$ giao dịch trong $5\text{ phút}$ HOẶC $\ge 10$ giao dịch trong $1\text{ giờ}$.
- **Điểm rủi ro**: $+25.0$
- **Category**: `FraudRuleCategory::VELOCITY`

### 3. `ImpossibleTravelRule` (Vận tốc di chuyển bất khả thi)
- **Tiêu chí**: Vận tốc di chuyển tương đối $> 800\text{ km/h}$ giữa hai địa điểm giao dịch liên tiếp.
- **Điểm rủi ro**: $+35.0$
- **Category**: `FraudRuleCategory::GEO_LOCATION`

### 4. `DeviceRiskRule` (Thiết bị nghi vấn / Giả lập)
- **Tiêu chí**: `fv.is_device_virtual == true` HOẶC `fv.is_device_compromised == true`.
- **Điểm rủi ro**: $+30.0$
- **Category**: `FraudRuleCategory::DEVICE_INTEGRITY`

### 5. `SuspiciousMerchantRule` (Merchant rủi ro cao)
- **Tiêu chí**: Merchant bị Blacklist hoặc danh mục Cờ bạc/Tiền điện tử trái phép.
- **Điểm rủi ro**: $+25.0$
- **Category**: `FraudRuleCategory::LIST_MATCHING`

### 6. `CardTestingRule` (Tấn công thử thẻ - Card Testing)
- **Tiêu chí**: Số tiền cực nhỏ ($< \$3.0$) lặp đi lặp lại $\ge 3$ lần trong $5\text{ phút}$.
- **Điểm rủi ro**: $+20.0$
- **Category**: `FraudRuleCategory::CARD_TESTING`

### 7. `BlacklistRule` (Danh sách đen IP / Thiết bị)
- **Tiêu chí**: IP hoặc Device Fingerprint khớp với cơ sở dữ liệu Blacklist toàn cục.
- **Điểm rủi ro**: $+40.0$ (Mức độ `RiskLevel::CRITICAL`)
- **Category**: `FraudRuleCategory::LIST_MATCHING`

---

## 🎛️ 3. Thiết Kế `FraudRuleEngine` (Composite / Runner)

`FraudRuleEngine` chịu trách nhiệm đăng ký (register) các luật và chạy duyệt tuần tự trên từng giao dịch.

### File: `include/engine/fraud/FraudRuleEngine.h`
```cpp
#pragma once
#include "DSA/vector.h"
#include "engine/rules/IFraudRule.h"
#include "core/domain/alert/FraudAlert.h"

class FraudRuleEngine {
private:
    Vector<IFraudRule*> rules;

public:
    FraudRuleEngine() = default;
    ~FraudRuleEngine();

    // Đăng ký một luật mới vào Engine
    void registerRule(IFraudRule* rule);

    /**
     * @brief Thực thi toàn bộ các luật đã đăng ký.
     * @return Tổng điểm rủi ro do các luật đóng góp (rule_score)
     */
    double evaluate(
        const Transaction& tx,
        const Customer& customer,
        const FeatureVector& features,
        Vector<FraudAlert>& out_alerts
    ) const;

    int getRuleCount() const { return rules.size(); }
};
```

### File: `sources/engine/fraud/FraudRuleEngine.cpp`
```cpp
#include "engine/fraud/FraudRuleEngine.h"

FraudRuleEngine::~FraudRuleEngine() {
    for (int i = 0; i < rules.size(); ++i) {
        delete rules[i];
    }
}

void FraudRuleEngine::registerRule(IFraudRule* rule) {
    if (rule != nullptr) {
        rules.push_back(rule);
    }
}

double FraudRuleEngine::evaluate(
    const Transaction& tx,
    const Customer& customer,
    const FeatureVector& features,
    Vector<FraudAlert>& out_alerts
) const {
    double total_rule_score = 0.0;

    for (int i = 0; i < rules.size(); ++i) {
        FraudAlert alert;
        if (rules[i]->evaluate(tx, customer, features, alert)) {
            out_alerts.push_back(alert);
            total_rule_score += alert.getRiskContribution();
        }
    }

    return total_rule_score;
}
```

---

## 🎯 4. Tiêu Chuẩn Nghiệm Thu Bước 2
- [ ] Giao diện `IFraudRule.h` độc lập, áp dụng tính đa hình C++ (Polymorphism).
- [ ] Khởi tạo `FraudRuleEngine`, nạp 7 luật và truyền vào một giao dịch gian lận (vd: Vận tốc $3000\text{ km/h}$).
- [ ] `out_alerts` nhận đúng cảnh báo `Impossible Travel` và điểm `rule_score` trả về $> 0$.

---
👉 **Tiếp tục với Bước 3: [03_ML_INTEGRATION.md](file:///d:/Fraud-Detection---Risk-management/docs/engine/03_ML_INTEGRATION.md)**
