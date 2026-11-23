#pragma once

#include <string>
#include <vector>
#include "core/models/Transaction.h"
#include "core/models/Customer.h"
#include "core/models/Account.h"
#include "core/models/RiskAssessment.h"
#include "dsa/priorityqueue.h"
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
    PriorityQueue<RiskAssessment> review_queue;

public:
    TransactionBLL();

    // Xử lý một giao dịch với thông tin tài khoản và khách hàng truyền trực tiếp
    TransactionResult processTransaction(Transaction& tx, Account& acc, const Customer& cust);

    // Xử lý một giao dịch hoàn chỉnh qua tra cứu tầng DAL
    TransactionResult processTransaction(Transaction& tx);

    // Đăng ký một luật gian lận mới vào hệ thống
    void addRule(IFraudRule* rule);

    RiskEngine& getRiskEngine() { return risk_engine; }
    const RiskEngine& getRiskEngine() const { return risk_engine; }

    // Hàng đợi rà soát rủi ro ưu tiên (PriorityQueue)
    const PriorityQueue<RiskAssessment>& getReviewQueue() const { return review_queue; }
    PriorityQueue<RiskAssessment>& getReviewQueue() { return review_queue; }
    int getReviewQueueSize() const { return review_queue.size(); }

    // Truy cập các đối tượng tầng DAL
    AccountDAL& getAccountDAL() { return account_dal; }
    CustomerDAL& getCustomerDAL() { return customer_dal; }

    const std::vector<Transaction>& getHistory() const { return transaction_history; }
};

// Alias tương thích nếu cần
using TransactionService = TransactionBLL;
