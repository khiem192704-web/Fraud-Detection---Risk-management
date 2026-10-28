# ⚖️ Bước 4: Động Cơ Thẩm Định Rủi Ro `RiskEngine`

> **File cần code**:  
> - Header: [include/engine/RiskEngine.h](file:///d:/Fraud-Detection---Risk-management/include/engine/RiskEngine.h)  
> - Source: [sources/engine/RiskEngine.cpp](file:///d:/Fraud-Detection---Risk-management/sources/engine/RiskEngine.cpp)  
> **Khái niệm OOP cốt lõi**: Lớp Điều phối (Coordinator / Aggregator), Quản lý bộ nhớ con trỏ động, Tính giải thích minh bạch (Explainable AI/Risk).  
> **Thời gian dự kiến**: 30 - 45 phút.

---

## 🎯 1. Bản chất: Vai trò của `RiskEngine`

`RiskEngine` đóng vai trò là **"Nhạc trưởng"** của toàn bộ phân hệ:
1. Lưu trữ và quản lý tập hợp các luật gian lận (`Vector<IFraudRule*> rules;`).
2. Nhận vào một giao dịch `Transaction`, cho chạy qua lần lượt từng luật con.
3. Thu thập toàn bộ danh sách cảnh báo vi phạm `FraudAlert`.
4. Kết hợp điểm từ **Luật (Rule Score)** + điểm từ **Học máy (ML Score)** + **Hồ sơ khách hàng (Profile)** thành một điểm số tổng hợp duy nhất từ **$0.0 \to 100.0$**.
5. Đóng gói kết quả thành đối tượng `RiskAssessment` có tính giải thích minh bạch (chỉ rõ vì sao bị trừ điểm).

---

## 📐 2. Cấu Trúc `RiskPolicy` (Chính Sách Trọng Số)

Tuyệt đối **không hardcode các con số ma thuật (magic numbers)** rải rác trong code. Tất cả trọng số được gom trong struct `RiskPolicy`:

```cpp
struct RiskPolicy {
    // Tổng trọng số = 1.0 (100%)
    double weight_rules{0.50};      // 50% đến từ vi phạm các luật tất định
    double weight_ml{0.40};         // 40% đến từ xác suất mô hình học máy
    double weight_profile{0.10};    // 10% đến từ rủi ro nền tảng của khách hàng

    // Các ngưỡng phân tầng rủi ro (RiskLevel)
    double threshold_very_low{20.0}; // [0, 20)  -> VERY_LOW
    double threshold_low{40.0};      // [20, 40) -> LOW
    double threshold_medium{60.0};   // [40, 60) -> MEDIUM
    double threshold_high{80.0};     // [60, 80) -> HIGH
                                     // >= 80    -> CRITICAL

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

## 🧮 3. Thuật Toán `assessRisk(...)`

Công thức tính điểm tổng hợp:
$$\text{Combined Score} = (w_{rules} \times \text{RuleScore}) + (w_{ml} \times \text{MLScore}) + (w_{profile} \times \text{CustomerBaseline})$$

### Các bước thực hiện trong `RiskEngine.cpp`:

```cpp
RiskAssessment RiskEngine::assessRisk(
    const Transaction& tx,
    const Customer& customer,
    double ml_score
) {
    Vector<FraudAlert> alerts;
    Vector<std::string> reasons;
    double raw_rule_score = 0.0;

    // 1. Duyệt qua từng luật đã đăng ký (Polymorphism)
    for (int i = 0; i < rules.size(); ++i) {
        FraudAlert alert;
        if (rules[i]->evaluate(tx, customer, alert)) {
            alerts.push_back(alert);
            raw_rule_score += alert.getRiskContribution();
            reasons.push_back(alert.getReason());
        }
    }

    // 2. Chuẩn hóa điểm rule (không để vượt quá 100.0)
    double normalized_rule_score = (raw_rule_score > 100.0) ? 100.0 : raw_rule_score;

    // 3. Tính điểm tổng hợp theo trọng số của RiskPolicy
    double profile_score = customer.getBaselineRiskScore(); // vd: Khách hàng mới = 20, Bị cảnh báo = 50
    double combined_score = (policy.weight_rules * normalized_rule_score)
                          + (policy.weight_ml * ml_score)
                          + (policy.weight_profile * profile_score);

    // Giới hạn trong đoạn [0.0, 100.0]
    if (combined_score > 100.0) combined_score = 100.0;
    if (combined_score < 0.0)   combined_score = 0.0;

    // 4. Xác định RiskLevel
    RiskLevel level = policy.determineRiskLevel(combined_score);

    // 5. Khởi tạo đối tượng RiskAssessment và trả về
    std::string assessment_id = "ASM_" + tx.GetTransactionID();
    Timestamp now = std::chrono::system_clock::now();

    return RiskAssessment(
        assessment_id,
        tx.GetTransactionID(),
        normalized_rule_score,
        ml_score,
        combined_score,
        level,
        DecisionAction::APPROVE, // Sẽ được DecisionEngine quyết định ở Bước 5
        alerts,
        reasons,
        now
    );
}
```

---

## ⚠️ 4. Quản Lý Bộ Nhớ (Memory Management)

Khi `RiskEngine` kết thúc vòng đời, ai sẽ xóa các con trỏ `IFraudRule*` được `new` ra?
$\to$ Phải giải phóng trong **Destructor của `RiskEngine`**:

```cpp
RiskEngine::~RiskEngine() {
    for (int i = 0; i < rules.size(); ++i) {
        delete rules[i]; // Giải phóng bộ nhớ của từng rule con
    }
}
```

---

## 🎓 Câu Hỏi Vấn Đáp Thầy Cô Hay Hỏi Bảo Vệ PBL2

> **Hỏi**: *"Tại sao hệ thống của em không lấy trực tiếp xác suất của Machine Learning để chặn luôn mà lại phải tính điểm tổng hợp qua `RiskEngine`?"*  
> **Đáp**: *"Dạ thưa thầy/cô, trong ngành ngân hàng và thanh toán quốc tế (chuẩn PCI-DSS), mô hình Machine Learning là dạng hộp đen (Black-box), rất khó giải trình cho khách hàng và cơ quan quản lý lý do tại sao một giao dịch bị từ chối. Vì vậy, hệ thống sử dụng `RiskEngine` theo mô hình **Explainable Hybrid Risk**:
> 1. Điểm số kết hợp cả vi phạm luật tất định (Rules) và xác suất học máy (ML).
> 2. Đối tượng `RiskAssessment` lưu vết cụ thể từng cảnh báo `FraudAlert` và danh sách lý do `reasons`, giúp nhân viên thẩm định biết chính xác giao dịch bị chặn vì quẹt thẻ quá nhanh hay vì thiết bị lạ, đảm bảo tính minh bạch 100%."*

