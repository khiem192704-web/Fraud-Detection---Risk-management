#pragma once

#include <string>
#include <vector>
#include "core/models/Transaction.h"
#include "core/models/Customer.h"
#include "core/models/Account.h"
#include "core/models/RiskAssessment.h"
#include "engine/RiskEngine.h"
#include "engine/DecisionEngine.h"
#include "dal/AccountDAL.h"
#include "dal/CustomerDAL.h"

struct TransactionResult {
    bool success{false};
    DecisionAction action{DecisionAction::BLOCK};
    std::string message{""};
    RiskAssessment assessment;
};

// TransactionBLL: Lớp thực thi nghiệp vụ thanh toán (Business Logic Layer)
class TransactionBLL {
private:
    RiskEngine risk_engine;
    DecisionEngine decision_engine;
    AccountDAL account_dal;
    CustomerDAL customer_dal;
    std::vector<Transaction> transaction_history;

public:
    TransactionBLL();

    // Xử lý một giao dịch hoàn chỉnh từ đầu đến cuối
    TransactionResult processTransaction(Transaction& tx);

    // Đăng ký một luật gian lận mới vào hệ thống
    void addRule(IFraudRule* rule);

    // Truy cập các đối tượng tầng DAL
    AccountDAL& getAccountDAL() { return account_dal; }
    CustomerDAL& getCustomerDAL() { return customer_dal; }

    const std::vector<Transaction>& getHistory() const { return transaction_history; }
};

// Alias tương thích nếu cần
using TransactionService = TransactionBLL;
