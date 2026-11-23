#pragma once

#include <string>
#include "core/domain/Transaction.h"
#include "core/domain/Customer.h"
#include "core/domain/Account.h"
#include "core/domain/RiskAssessment.h"
#include "engine/RiskEngine.h"
#include "engine/DecisionEngine.h"
#include "storage/AccountRepo.h"
#include "storage/CustomerRepo.h"
#include <vector>

struct TransactionResult {
    bool success{false};
    DecisionAction action{DecisionAction::BLOCK};
    std::string message{""};
    RiskAssessment assessment;
};

class TransactionService {
private:
    RiskEngine risk_engine;
    DecisionEngine decision_engine;
    AccountRepo account_repo;
    CustomerRepo customer_repo;
    std::vector<Transaction> transaction_history;

public:
    TransactionService();
    // Xử lý một giao dịch hoàn chỉnh từ đầu đến cuối
    TransactionResult processTransaction(Transaction& tx);
    // Đăng ký một luật gian lận mới vào hệ thống
    void addRule(IFraudRule* rule);
    // Truy cập các Repository
    AccountRepo& getAccountRepo() { return account_repo; }
    CustomerRepo& getCustomerRepo() { return customer_repo; }
    const std::vector<Transaction>& getHistory() const { return transaction_history; }
};