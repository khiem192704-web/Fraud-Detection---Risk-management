#pragma once
#include <string>
#include "core/common/Types.h"
#include "core/models/Transaction.h"
#include "core/models/Customer.h"
#include "core/models/FraudAlert.h"
class IFraudRule{
protected:
    bool is_enabled{true};

public:
    virtual ~IFraudRule()=default;
    virtual std::string getId() const=0;
    virtual std::string getName() const=0;
    virtual FraudRuleCategory getCategory() const=0;
    virtual double getRiskContribution() const=0;
    virtual bool evaluate(const Transaction& tx, const Customer& customer,FraudAlert& out_alert)=0;

    bool isEnabled() const { return is_enabled; }
    void setEnabled(bool enabled) { is_enabled = enabled; }
    std::string getRuleName() const { return getName(); }
    std::string getRuleId() const { return getId(); }
};
