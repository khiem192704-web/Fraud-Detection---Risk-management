#pragma once

#include "engine/rules/IFraudRule.h"

/**
 * ============================================================================
 * [BƯỚC 2.2] TODO: KHAI BÁO LỚP HIGHAMOUNTRULE (LUẬT SỐ TIỀN ĐỘT BIẾN)
 * Tài liệu hướng dẫn: include/engine/02_STEP2_CONCRETE_RULES.md (Mục 2.2)
 * File hiện thực tương ứng: sources/engine/rules/HighAmountRule.cpp
 * ============================================================================
 * 
 * Mục tiêu:
 * - Kế thừa từ IFraudRule.
 * - Bắt số tiền vượt trần tuyệt đối (vd: 5,000 USD) hoặc gấp nhiều lần bình quân.
 */
class HighAmountRule : public IFraudRule {
private:
    // TODO 1: Khai báo các thuộc tính ngưỡng:
    // - double max_amount_threshold;  // Ngưỡng trần số tiền tuyệt đối
    // - double multiplier_threshold;  // Hệ số đột biến so với bình quân
    // - double risk_contribution;     // Điểm đóng góp (mặc định 25.0)
    double max_amount_threshold;
    double multiplier_threshold;
    double risk_contribution;

public:
    // TODO 2: Khai báo Constructor có tham số mặc định:
    // explicit HighAmountRule(double max_amount = 5000.0, double multiplier = 5.0, double contribution = 25.0);
    explicit HighAmountRule(
        double max_amount = 5000.0,
        double multiplier = 5.0,
        double contribution = 25.0
    );

    // TODO 3: Ghi đè các phương thức định danh:
    // - std::string getId() const override;           // "RULE_002_HIGH_AMOUNT"
    // - std::string getName() const override;         // "Unusual High Transaction Amount"
    // - FraudRuleCategory getCategory() const override; // FraudRuleCategory::UNUSUAL_AMOUNT
    // - double getRiskContribution() const override;
    std::string getId() const override;
    std::string getName() const override;
    FraudRuleCategory getCategory() const override;
    double getRiskContribution() const override;

    // TODO 4: Khai báo hàm thẩm định giao dịch:
    // bool evaluate(const Transaction& tx, const Customer& customer, FraudAlert& out_alert) override;
    bool evaluate(
        const Transaction& tx,
        const Customer& customer,
        FraudAlert& out_alert
    ) override;
};
