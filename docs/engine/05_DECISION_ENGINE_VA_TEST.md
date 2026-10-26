# 🏁 Bước 5: Động Cơ Ra Quyết Định & Kiểm Thử Toàn Diện (Decision Engine & End-to-End Test)

> **Mục tiêu**: Hoàn thiện mắt xích cuối cùng của Engine: `DecisionEngine` chuyển đổi điểm rủi ro từ `RiskAssessment` thành hành động thực tế (`APPROVE`, `REVIEW`, `CHALLENGE_3DS`, `BLOCK`), cập nhật trạng thái `Transaction`, đồng thời sửa hệ thống build (`Makefile` / `CMakeLists.txt`) và viết kịch bản demo mẫu trên `main.cpp`.  
> **Thư mục làm việc**: `include/core/decision/`, `sources/core/decision/`, `CMakeLists.txt`, `Makefile`, và `sources/main.cpp`.

---

## 🚦 1. Thiết Kế `DecisionEngine` (`DecisionEngine.h` & `DecisionEngine.cpp`)

### File: `include/core/decision/DecisionEngine.h`
```cpp
#pragma once
#include "core/common/Types.h"
#include "core/domain/transaction/Transaction.h"
#include "core/domain/alert/RiskAssessment.h"
#include "core/domain/account/Customer.h"

struct DecisionPolicy {
    // Ngưỡng điểm để đưa ra hành động tương ứng
    double approve_max_score{39.99};    // [0, 40)   -> APPROVE
    double challenge_max_score{59.99};  // [40, 60)  -> CHALLENGE_3DS (OTP)
    double review_max_score{79.99};     // [60, 80)  -> REVIEW (Điều tra viên)
                                        // >= 80.0   -> BLOCK
};

class DecisionEngine {
private:
    DecisionPolicy policy;

public:
    explicit DecisionEngine(const DecisionPolicy& custom_policy = DecisionPolicy())
        : policy(custom_policy) {}

    /**
     * @brief Đưa ra quyết định cuối cùng và tự động cập nhật trạng thái của Transaction.
     * @param tx Giao dịch cần cập nhật trạng thái
     * @param customer Khách hàng thực hiện
     * @param assessment Đánh giá rủi ro từ RiskEngine
     * @return DecisionAction Hành động được áp dụng
     */
    DecisionAction decide(
        Transaction& tx,
        const Customer& customer,
        RiskAssessment& assessment
    ) const;

private:
    // Kiểm tra các luật phủ quyết khẩn cấp (Override Rules)
    bool checkEmergencyBlock(
        const Customer& customer,
        const RiskAssessment& assessment
    ) const;
};
```

### File: `sources/core/decision/DecisionEngine.cpp`
```cpp
#include "core/decision/DecisionEngine.h"

bool DecisionEngine::checkEmergencyBlock(
    const Customer& customer,
    const RiskAssessment& assessment
) const {
    // 1. Khách hàng đã bị cấm trên toàn hệ thống
    if (customer.getRiskTier() == CustomerRiskTier::BANNED) {
        return true;
    }

    // 2. Chặn ngay nếu có bất kỳ cảnh báo mức độ CRITICAL (vd: Blacklist, Thiết bị Hacker)
    const auto& alerts = assessment.getAlerts();
    for (int i = 0; i < alerts.size(); ++i) {
        if (alerts[i].isCritical()) {
            return true;
        }
    }

    return false;
}

DecisionAction DecisionEngine::decide(
    Transaction& tx,
    const Customer& customer,
    RiskAssessment& assessment
) const {
    DecisionAction action = DecisionAction::APPROVE;

    // Bước 1: Ưu tiên kiểm tra luật phủ quyết khẩn cấp
    if (checkEmergencyBlock(customer, assessment)) {
        action = DecisionAction::BLOCK;
        assessment.addReason("[DECISION_OVERRIDE] Emergency Block triggered due to Critical Alert or Banned Customer");
    } else {
        // Bước 2: Phân luồng dựa trên điểm rủi ro tổng hợp (Combined Score)
        double score = assessment.getCombinedScore();

        if (score <= policy.approve_max_score) {
            action = DecisionAction::APPROVE;
        } else if (score <= policy.challenge_max_score) {
            action = DecisionAction::CHALLENGE_3DS; // Yêu cầu xác thực OTP / 3D-Secure
        } else if (score <= policy.review_max_score) {
            action = DecisionAction::REVIEW;        // Đẩy vào hàng đợi điều tra viên
        } else {
            action = DecisionAction::BLOCK;         // Chặn tự động
        }
    }

    // Bước 3: Đồng bộ trạng thái vào Transaction
    switch (action) {
        case DecisionAction::APPROVE:
            tx.approve();
            break;
        case DecisionAction::CHALLENGE_3DS:
            tx.challenge();
            break;
        case DecisionAction::REVIEW:
            tx.sendToReview();
            break;
        case DecisionAction::BLOCK:
            tx.reject();
            break;
    }

    return action;
}
```

---

## 🛠️ 2. Cập Nhật Cấu Hình Biên Dịch (`Makefile` & `CMakeLists.txt`)

Cần sửa đường dẫn từ thư mục cũ `src/` thành `sources/` và bổ sung các file của Engine vào danh sách nguồn biên dịch.

### Sửa `Makefile`:
```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

SRCS_CORE = sources/core/common/Types.cpp \
            sources/core/domain/location/location.cpp \
            sources/core/domain/device/device.cpp \
            sources/core/domain/merchant/Merchant.cpp \
            sources/core/domain/account/Account.cpp \
            sources/core/domain/account/Customer.cpp \
            sources/core/domain/transaction/Transaction.cpp \
            sources/core/domain/alert/FraudAlert.cpp \
            sources/core/domain/alert/RiskAssessment.cpp \
            sources/engine/features/FeatureExtractor.cpp \
            sources/engine/fraud/FraudRuleEngine.cpp \
            sources/engine/ml/NativeMLPredictor.cpp \
            sources/engine/risk/RiskEngine.cpp \
            sources/core/decision/DecisionEngine.cpp

SRCS_APP = sources/main.cpp
...
```

---

## 🚀 3. Kịch Bản Test End-to-End Trọn Vẹn Tại `sources/main.cpp`

File `main.cpp` sẽ ghép nối toàn bộ luồng Engine để kiểm tra thực tế:

```cpp
#include <iostream>
#include <vector>
#include "core/common/Types.h"
#include "core/domain/account/Customer.h"
#include "core/domain/merchant/Merchant.h"
#include "core/domain/transaction/Transaction.h"
#include "engine/features/FeatureExtractor.h"
#include "engine/fraud/FraudRuleEngine.h"
#include "engine/rules/LargeAmountRule.h"
#include "engine/ml/NativeMLPredictor.h"
#include "engine/risk/RiskEngine.h"
#include "core/decision/DecisionEngine.h"

int main() {
    std::cout << "========================================================\n";
    std::cout << "  EPFD-RAS: PAYMENT FRAUD DETECTION ENGINE SHOWCASE     \n";
    std::cout << "========================================================\n\n";

    // 1. Khởi tạo các thành phần của Engine
    FeatureExtractor feature_extractor;
    FraudRuleEngine rule_engine;
    rule_engine.registerRule(new LargeAmountRule(3000.0, 5.0));
    
    NativeMLPredictor ml_predictor;
    RiskEngine risk_engine;
    DecisionEngine decision_engine;

    // 2. Tạo dữ liệu giả lập: Khách hàng Alice
    Location hcm_location("VN", "Ho Chi Minh", "70000", 10.8231, 106.6297);
    Customer alice("CUST_001", "Alice Nguyen", "alice@gmail.com", "0901234567", hcm_location);
    alice.addTrustedDevice("DEV_ALICE_PHONE");

    // Tạo Merchant
    Merchant grocery("MERCH_01", "VinMart", "5411", MerchantCategory::GROCERY, hcm_location);

    // Kịch bản 1: Alice thanh toán $45.00 thông thường
    Device alice_dev("DEV_ALICE_PHONE", "14.232.208.1", "fp_alice", hcm_location, false, false);
    Transaction tx1("TX_001", TransactionType::BUY, "CUST_001", "", "ACC_001", 45.0, 
                    "USD", std::chrono::system_clock::now(), hcm_location, "14.232.208.1", 
                    alice_dev, "MERCH_01", PaymentType::CARD);

    Vector<Transaction> history;
    FeatureVector fv1 = feature_extractor.extract(tx1, alice, grocery, history);
    
    Vector<FraudAlert> alerts1;
    double rule_score1 = rule_engine.evaluate(tx1, alice, fv1, alerts1);
    double ml_score1 = ml_predictor.predictProbability(fv1);
    RiskAssessment assessment1 = risk_engine.assessRisk(tx1, alice, alerts1, rule_score1, ml_score1);
    DecisionAction action1 = decision_engine.decide(tx1, alice, assessment1);

    std::cout << "[SCENARIO 1] Alice Mua Tap Hoa $45.00\n";
    std::cout << "  - Diem rui ro: " << assessment1.getCombinedScore() << " / 100 (" << toString(assessment1.getRiskLevel()) << ")\n";
    std::cout << "  - Quyet dinh:  " << toString(action1) << "\n\n";

    // Kịch bản 2: Tấn công gian lận $4,500.00 từ Paris với máy ảo hacker
    Location paris("FR", "Paris", "75000", 48.8566, 2.3522);
    Device hacker_dev("DEV_UNKNOWN", "185.220.101.5", "fp_hacker", paris, true, true);
    Transaction tx2("TX_002", TransactionType::BUY, "CUST_001", "", "ACC_001", 4500.0, 
                    "USD", std::chrono::system_clock::now(), paris, "185.220.101.5", 
                    hacker_dev, "MERCH_01", PaymentType::CARD);

    history.push_back(tx1); // Alice vừa mới giao dịch ở HCM 1 phút trước!
    FeatureVector fv2 = feature_extractor.extract(tx2, alice, grocery, history);
    
    Vector<FraudAlert> alerts2;
    double rule_score2 = rule_engine.evaluate(tx2, alice, fv2, alerts2);
    double ml_score2 = ml_predictor.predictProbability(fv2);
    RiskAssessment assessment2 = risk_engine.assessRisk(tx2, alice, alerts2, rule_score2, ml_score2);
    DecisionAction action2 = decision_engine.decide(tx2, alice, assessment2);

    std::cout << "[SCENARIO 2] Dot Tan Cong Gian Lan $4,500.00 Tu Paris\n";
    std::cout << "  - Diem rui ro: " << assessment2.getCombinedScore() << " / 100 (" << toString(assessment2.getRiskLevel()) << ")\n";
    std::cout << "  - Quyet dinh:  " << toString(action2) << "\n";
    std::cout << "  - Cac ly do vi pham (Explainability):\n";
    for (int i = 0; i < assessment2.getReasons().size(); ++i) {
        std::cout << "     * " << assessment2.getReasons()[i] << "\n";
    }

    return 0;
}
```

---

## 🎯 4. Tiêu Chuẩn Nghiệm Thu Bước 5
- [ ] Chạy lệnh `mingw32-make` biên dịch ra `bin/epfd_app.exe` thành công mà không có lỗi.
- [ ] Khởi chạy `./bin/epfd_app.exe`, màn hình hiển thị chính xác:
  - Scenario 1 đạt `APPROVE` với điểm rủi ro $< 20.0$.
  - Scenario 2 đạt `BLOCK` với điểm rủi ro $\ge 80.0$, in ra đầy đủ các dòng giải thích vi phạm luật và xác suất học máy.
- [ ] Nhánh `Hoang_engine` đã có thể tạo Pull Request hợp nhất vào `main`!
