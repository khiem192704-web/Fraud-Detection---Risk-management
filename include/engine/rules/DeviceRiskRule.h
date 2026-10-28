#pragma once

#include "engine/rules/IFraudRule.h"

/**
 * ============================================================================
 * [BƯỚC 2.1] TODO: KHAI BÁO LỚP DEVICERISKRULE (LUẬT MÔI TRƯỜNG THIẾT BỊ)
 * Tài liệu hướng dẫn: include/engine/02_STEP2_CONCRETE_RULES.md (Mục 2.1)
 * File hiện thực tương ứng: sources/engine/rules/DeviceRiskRule.cpp
 * ============================================================================
 * 
 * Mục tiêu:
 * - Kế thừa công khai từ IFraudRule: class DeviceRiskRule : public IFraudRule
 * - Bắt các giao dịch từ máy ảo (emulator) hoặc thiết bị đã root/jailbreak.
 */
class DeviceRiskRule : public IFraudRule {
private:
    // TODO 1: Khai báo thuộc tính lưu điểm rủi ro đóng góp khi vi phạm
    // Gợi ý: double risk_contribution;
    double risk_contribution;

public:
    // TODO 2: Khai báo Constructor với điểm đóng góp mặc định là 30.0
    // Gợi ý dùng từ khóa explicit: explicit DeviceRiskRule(double contribution = 30.0);
    explicit DeviceRiskRule(double contribution = 30.0);

    // TODO 3: Khai báo ghi đè (override) các hàm thuần ảo từ IFraudRule:
    // - std::string getId() const override;
    // - std::string getName() const override;
    // - FraudRuleCategory getCategory() const override;
    // - double getRiskContribution() const override;
    std::string getId() const override;
    std::string getName() const override;
    FraudRuleCategory getCategory() const override;
    double getRiskContribution() const override;

    // TODO 4: Khai báo hàm thẩm định giao dịch ghi đè từ IFraudRule:
    // bool evaluate(const Transaction& tx, const Customer& customer, FraudAlert& out_alert) override;
    bool evaluate(
        const Transaction& tx,
        const Customer& customer,
        FraudAlert& out_alert
    ) override;
};
