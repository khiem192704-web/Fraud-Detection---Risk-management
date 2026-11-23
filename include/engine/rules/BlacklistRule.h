#pragma once
#include <unordered_map>
#include "engine/rules/IFraudRule.h"
#include <string>
class BlacklistRule:public IFraudRule{
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
    void addBlacklistedIp(const std::string& ip, const std::string& reason="IP bị chặn") { addBlacklistedIP(ip, reason); }
    void addBlacklistedCard(const std::string& card, const std::string& reason="Thẻ bị chặn") { addBlacklistedAccount(card, reason); }
    void loadBlacklistFromFile(const std::string& filepath);
    void appendBlacklistToFile(const std::string& type, const std::string& value, const std::string& filepath = "data/blacklist.txt");
    bool isIPBlacklisted(const std::string& ip) const;
    bool isAccountBlacklisted(const std::string& account_id) const;
    bool evaluate(const Transaction& tx, const Customer& customer, FraudAlert& out_alert) override;
};
