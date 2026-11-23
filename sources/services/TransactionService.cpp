#include "services/TransactionService.h"
#include "engine/rules/HighAmountRule.h"
#include "engine/rules/VelocityRule.h"
#include "engine/rules/DeviceRiskRule.h"
#include "engine/rules/BlacklistRule.h"

TransactionService::TransactionService() {
    // Nạp dữ liệu tài khoản và khách hàng từ file ổ đĩa
    account_repo.loadFromFile("data/accounts.txt");
    customer_repo.loadFromFile("data/customers.txt");

    // Đăng ký bộ 4 luật mặc định vào RiskEngine
    risk_engine.registerRule(new HighAmountRule(3000.0, 3.0, 25.0));
    risk_engine.registerRule(new VelocityRule(5, std::chrono::seconds(60), 30.0));
    risk_engine.registerRule(new DeviceRiskRule(30.0));
    risk_engine.registerRule(new BlacklistRule(50.0));

    // Nạp trọng số mô hình ML nếu có file
    risk_engine.getMLPredictor().loadModel("data/model_weights.json");
}

void TransactionService::addRule(IFraudRule* rule) {
    risk_engine.registerRule(rule);
}

TransactionResult TransactionService::processTransaction(Transaction& tx) {
    TransactionResult res;

    // 1. Kiểm tra sự tồn tại của Tài khoản
    Account* acc = account_repo.findById(tx.GetAccountID());
    if (!acc) {
        res.success = false;
        res.action = DecisionAction::BLOCK;
        res.message = "Lỗi: Không tìm thấy tài khoản nguồn ID: " + tx.GetAccountID();
        tx.reject();
        return res;
    }

    // 2. Kiểm tra số dư tài khoản
    if (acc->getBalance() < tx.GetAmount()) {
        res.success = false;
        res.action = DecisionAction::BLOCK;
        res.message = "Lỗi: Số dư không đủ để thực hiện giao dịch!";
        tx.reject();
        return res;
    }

    // 3. Tìm thông tin khách hàng tương ứng (hoặc dùng profile mặc định)
    Customer* cust = customer_repo.findById(tx.GetCustomerID());
    Customer current_cust;
    if (cust) {
        current_cust = *cust;
    } else {
        current_cust = Customer(tx.GetCustomerID(), "Standard Customer", "user@bank.com", "0900000000", tx.GetLocation());
    }

    // 4. Thẩm định rủi ro tự chủ (RiskEngine tự gọi FeatureExtractor & NativeMLPredictor)
    RiskAssessment assessment = risk_engine.assessRisk(tx, current_cust);
    res.assessment = assessment;

    // 5. Ra quyết định nghiệp vụ (DecisionEngine)
    DecisionAction action = decision_engine.decide(tx, current_cust, assessment);
    res.action = action;

    // 6. Thực thi kết quả tài chính
    if (action == DecisionAction::APPROVE) {
        acc->deduct(tx.GetAmount()); // Trừ tiền tài khoản
        account_repo.saveToFile("data/accounts.txt");
        res.success = true;
        res.message = "Giao dịch được CHẤP THUẬN (APPROVE). Đã trừ tiền thành công!";
    } else if (action == DecisionAction::CHALLENGE) {
        res.success = false;
        res.message = "Giao dịch có nghi vấn (CHALLENGE). Yêu cầu xác thực thêm OTP!";
    } else if (action == DecisionAction::REVIEW) {
        res.success = false;
        res.message = "Giao dịch bị TẠM GIỮ (REVIEW). Đã chuyển sang hàng đợi thẩm định!";
    } else {
        res.success = false;
        res.message = "Giao dịch bị TỪ CHỐI (BLOCK) do rủi ro vượt ngưỡng an toàn!";
    }

    // 7. Lưu vào lịch sử giao dịch
    transaction_history.push_back(tx);
    return res;
}