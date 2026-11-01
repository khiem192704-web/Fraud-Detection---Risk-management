#pragma once

#include "engine/rules/IFraudRule.h"

class HighAmountRule : public IFraudRule {
private:
    double max_amount_threshold;
    double multiplier_threshold;
    double risk_contribution;
public:
    explicit HighAmountRule(double max_amount=5000.0,double multiplier=5.0,double contribution=25.0);
    std::string getId() const override;
    std::string getName() const override;
    FraudRuleCategory getCategory() const override;
    double getRiskContribution() const override;
    bool evaluate(const Transaction &tx, const Customer& customer,FraudAlert& out_alert) override;

};
