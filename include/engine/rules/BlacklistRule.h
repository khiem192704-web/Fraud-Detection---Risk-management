#pragma once

#include "engine/rules/IFraudRule.h"
#include <string>

/**
 * ============================================================================
 * [BƯỚC 2.3] TODO: KHAI BÁO LỚP BLACKLISTRULE (LUẬT DANH SÁCH ĐEN - TRA CỨU O(1))
 * Tài liệu hướng dẫn: include/engine/02_STEP2_CONCRETE_RULES.md (Mục 2.3)
 * File hiện thực tương ứng: sources/engine/rules/BlacklistRule.cpp
 * ============================================================================
 * 
 * Mục tiêu:
 * - Tra cứu IP hoặc mã tài khoản nằm trong danh sách đen với tốc độ tức thì O(1).
 * - Sử dụng Bảng băm (HashMap hoặc std::unordered_map).
 */
class BlacklistRule : public IFraudRule {
private:
    // TODO 1: Khai báo biến lưu điểm rủi ro đóng góp (mặc định 40.0 - mức nghiêm trọng)
    // Gợi ý: double risk_contribution;


    // TODO 2: Khai báo cấu trúc lưu trữ danh sách đen (Key: IP/AccountID -> Value: Lý do cấm)
    // Gợi ý: Có thể dùng cấu trúc HashMap tự viết từ include/DSA/ hoặc std::unordered_map<std::string, std::string>


public:
    // TODO 3: Khai báo Constructor khởi tạo điểm đóng góp (mặc định 40.0)
    // Gợi ý: explicit BlacklistRule(double contribution = 40.0);


    // TODO 4: Ghi đè (override) 4 phương thức định danh từ IFraudRule:
    // - getId() -> "RULE_003_BLACKLIST_MATCH"
    // - getName() -> "Blacklisted IP or Account Detection"
    // - getCategory() -> FraudRuleCategory::BLACKLIST_MATCH
    // - getRiskContribution()
    // Gợi ý:
    // std::string getId() const override;
    // std::string getName() const override;
    // FraudRuleCategory getCategory() const override;
    // double getRiskContribution() const override;


    // TODO 5: Khai báo các hàm tiện ích quản lý danh sách đen:
    // - addBlacklistedIP: Thêm IP vào danh sách đen kèm lý do
    // - addBlacklistedAccount: Thêm Account ID vào danh sách đen kèm lý do
    // - isIPBlacklisted: Kiểm tra IP có trong danh sách đen không (O(1))
    // - isAccountBlacklisted: Kiểm tra Account ID có trong danh sách đen không (O(1))
    // Gợi ý:
    // void addBlacklistedIP(const std::string& ip, const std::string& reason = "Suspicious IP");
    // void addBlacklistedAccount(const std::string& account_id, const std::string& reason = "Stolen account");
    // bool isIPBlacklisted(const std::string& ip) const;
    // bool isAccountBlacklisted(const std::string& account_id) const;


    // TODO 6: Khai báo hàm thẩm định giao dịch ghi đè (override) từ IFraudRule:
    // Gợi ý: bool evaluate(const Transaction& tx, const Customer& customer, FraudAlert& out_alert) override;

};
