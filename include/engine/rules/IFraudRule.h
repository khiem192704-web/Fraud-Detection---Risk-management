#pragma once
#include <string>
#include "core/common/Types.h"
#include "core/domain/Transaction.h"
#include "core/domain/Customer.h"
#include "core/domain/FraudAlert.h"
class IFraudRule{
public:
    virtual ~IFraudRule()=default;
    virtual std::string getId() const=0;
    virtual std::string getName() const=0;
    virtual FraudRuleCategory getCategory() const=0;
    virtual double getRiskContribution() const=0;
    virtual bool evaluate(const Transaction& tx, const Customer& customer,FraudAlert& out_alert)=0;
};
