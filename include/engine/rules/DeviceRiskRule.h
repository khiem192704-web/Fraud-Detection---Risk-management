#pragma once
#include "engine/rules/IFraudRule.h"
class DeviceRiskRule:public IFraudRule{
private:
    double risk_contribution;
public:
    //Khởi tạo tường minh
    explicit DeviceRiskRule(double contribution=30.0);
    std::string getId() const override;
    std::string getName() const override;
    FraudRuleCategory getCategory() const override;
    double getRiskContribution() const override;
    bool evaluate(const Transaction &tx, const Customer& customer,FraudAlert& out_alert) override;
};
