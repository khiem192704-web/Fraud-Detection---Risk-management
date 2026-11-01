#pragma once
#include <unordered_map>
#include "engine/rules/IFraudRule.h"
#include <string>
class BlacklistRule : public IFraudRule {
private:
    double risk_contribution;
    std::unordered_map<std::string, std::string> blacklisted_ips;
    std::unordered_map<std::string, std::string> blacklisted_accounts;
public:
    explicit BlacklistRule(double contribution=40.0);
    std::string getId() const override;
    std::string getName() const override;
    FraudRuleCategory getCategory() const override;
    double getRiskContribution() const override;
    void addBlacklistedIP(const std::string& ip,const std::string& reason="IP lạ Khả nghi");
    void addBlacklistedAccount(const std::string& account_id,const std::string& reason="Tài khoản nghi bị đánh cắp");
    bool isIPBlacklisted(const std::string& ip) const;
    bool isAccountBlacklisted(const std::string& account_id) const;
    bool evaluate(const Transaction& tx, const Customer& customer, FraudAlert& out_alert) override;
};
