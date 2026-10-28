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
    double risk_contribution;

    // TODO 2: Khai báo cấu trúc lưu trữ danh sách đen (Key: IP/AccountID -> Value: Lý do cấm)
    // Gợi ý: Có thể dùng cấu trúc HashMap tự viết từ include/DSA/ hoặc mảng động nếu đang thử nghiệm

public:
    // TODO 3: Constructor khởi tạo điểm đóng góp
    explicit BlacklistRule(double contribution = 40.0);

    // TODO 4: Ghi đè các phương thức từ IFraudRule:
    // - getId() -> "RULE_003_BLACKLIST_MATCH"
    // - getName() -> "Blacklisted IP or Account Detection"
    // - getCategory() -> FraudRuleCategory::BLACKLIST_MATCH
    // - getRiskContribution()
    std::string getId() const override;
    std::string getName() const override;
    FraudRuleCategory getCategory() const override;
    double getRiskContribution() const override;

    // TODO 5: Khai báo các hàm tiện ích quản lý danh sách đen:
    // - void addBlacklistedIP(const std::string& ip, const std::string& reason);
    // - void addBlacklistedAccount(const std::string& account_id, const std::string& reason);
    // - bool isIPBlacklisted(const std::string& ip) const;
    // - bool isAccountBlacklisted(const std::string& account_id) const;
    void addBlacklistedIP(const std::string& ip, const std::string& reason = "Suspicious IP");
    void addBlacklistedAccount(const std::string& account_id, const std::string& reason = "Stolen account");
    bool isIPBlacklisted(const std::string& ip) const;
    bool isAccountBlacklisted(const std::string& account_id) const;

    // TODO 6: Khai báo hàm thẩm định giao dịch:
    bool evaluate(
        const Transaction& tx,
        const Customer& customer,
        FraudAlert& out_alert
    ) override;
};
