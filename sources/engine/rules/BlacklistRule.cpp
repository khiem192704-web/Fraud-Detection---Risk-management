#include "engine/rules/BlacklistRule.h"

BlacklistRule::BlacklistRule(double contribution) {
    // TODO 1: Gán giá trị contribution vào biến thành viên risk_contribution
    // Gợi ý:
    this->risk_contribution = contribution;
}
std::string BlacklistRule::getId() const {
    // TODO 2: Trả về mã định danh duy nhất của luật ("RULE_003_BLACKLIST_MATCH")
    // Gợi ý:
    return "RULE_003_BLACKLIST_MATCH";
}
std::string BlacklistRule::getName() const {
    // TODO 3: Trả về tên mô tả ngắn gọn của luật
    // Gợi ý:
    return "Blacklisted IP or Account Detection";
}
FraudRuleCategory BlacklistRule::getCategory() const {
    // TODO 4: Trả về danh mục FraudRuleCategory::BLACKLIST_MATCH
    // Gợi ý:
    return FraudRuleCategory::BLACKLIST_MATCH;
}
double BlacklistRule::getRiskContribution() const {
    // TODO 5: Trả về điểm rủi ro đóng góp khi vi phạm luật
    // Gợi ý:
    return risk_contribution;
}
void BlacklistRule::addBlacklistedIP(const std::string& ip, const std::string& reason) {
    // TODO 6: Thêm IP và lý do vào bảng băm blacklisted_ips (Thao tác O(1))
    // Gợi ý:
    blacklisted_ips[ip]=reason;
}
void BlacklistRule::addBlacklistedAccount(const std::string& account_id, const std::string& reason) {
    // TODO 7: Thêm mã tài khoản và lý do vào bảng băm blacklisted_accounts (Thao tác O(1))
    // Gợi ý:
    blacklisted_accounts[account_id]=reason;
}
bool BlacklistRule::isIPBlacklisted(const std::string& ip) const {
    // TODO 8: Tra cứu IP trong bảng băm xem có tồn tại hay không (Độ phức tạp O(1))
    // Gợi ý:
    return blacklisted_ips.find(ip)!=blacklisted_ips.end();
}
bool BlacklistRule::isAccountBlacklisted(const std::string& account_id) const {
    // TODO 9: Tra cứu mã tài khoản trong bảng băm xem có tồn tại hay không (Độ phức tạp O(1))
    // Gợi ý:
    return blacklisted_accounts.find(account_id)!=blacklisted_accounts.end();
}
bool BlacklistRule::evaluate(
    const Transaction& tx,
    const Customer& /*customer*/,
    FraudAlert& out_alert
) {
    // TODO 10: Thẩm định xem giao dịch có vi phạm danh sách đen hay không:
    // 1. Kiểm tra isIPBlacklisted(tx.GetIP_Address())
    // 2. Kiểm tra isAccountBlacklisted(tx.GetAccountID())
    // 3. Nếu vi phạm:
    //    - Gán out_alert = FraudAlert(...) với mức độ nghiêm trọng CRITICAL (RiskLevel::CRITICAL)
    //    - return true;
    // 4. Nếu không nằm trong danh sách đen: return false;
    //
    // Gợi ý:
    if (isIPBlacklisted(tx.GetIP_Address())) {
        out_alert = FraudAlert(
            "ALT_" + tx.GetTransactionID() + "_BLK_IP",
            tx.GetTransactionID(),
            getId(),
            getName(),
            "Địa chỉ IP nằm trong danh sách đen (Blacklist): " + tx.GetIP_Address(),
            getCategory(),
            getRiskContribution(),
            RiskLevel::CRITICAL
        );
        return true;
    }
    if (isAccountBlacklisted(tx.GetAccountID())) {
        out_alert = FraudAlert(
            "ALT_" + tx.GetTransactionID() + "_BLK_ACC",
            tx.GetTransactionID(),
            getId(),
            getName(),
            "Tài khoản nằm trong danh sách đen (Blacklist): " + tx.GetAccountID(),
            getCategory(),
            getRiskContribution(),
            RiskLevel::CRITICAL
        );
        return true;
    }
    return false;
}
