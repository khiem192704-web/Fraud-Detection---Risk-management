#pragma once

#include "engine/rules/IFraudRule.h"
#include "DSA/Deque.h"
#include <chrono>

class VelocityRule : public IFraudRule {
private:
    Deque<Timestamp>sliding_window;
    int max_transactions;
    std::chrono::seconds window_duration;
    double risk_contribution;
public:
    explicit VelocityRule(int max_txs=5,std::chrono::seconds duration=std::chrono::seconds(60),double contribution=30.0);
    std::string getId() const override;
    std::string getName() const override;
    FraudRuleCategory getCategory() const override;
    double getRiskContribution() const override;
    bool evaluate(const Transaction& tx, const Customer& customer, FraudAlert& out_alert) override;
    void reset();
};
