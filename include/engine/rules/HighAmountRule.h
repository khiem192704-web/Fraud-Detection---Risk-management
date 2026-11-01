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
    // TODO 1: Khai báo các thuộc tính ngưỡng cấu hình luật:
    // - max_amount_threshold: Ngưỡng trần số tiền tuyệt đối
    // - multiplier_threshold: Hệ số đột biến so với mức trung bình của khách hàng
    // - risk_contribution: Điểm rủi ro đóng góp khi vi phạm (mặc định 25.0)
    // Gợi ý:
    // double max_amount_threshold;
    // double multiplier_threshold;
    // double risk_contribution;


public:
    // TODO 2: Khai báo Constructor có tham số mặc định (max_amount = 5000.0, multiplier = 5.0, contribution = 25.0)
    // Gợi ý:
    // explicit HighAmountRule(
    //     double max_amount = 5000.0,
    //     double multiplier = 5.0,
    //     double contribution = 25.0
    // );


    // TODO 3: Ghi đè (override) 4 phương thức định danh từ IFraudRule:
    // - getId() -> "RULE_002_HIGH_AMOUNT"
    // - getName() -> "Unusual High Transaction Amount"
    // - getCategory() -> FraudRuleCategory::UNUSUAL_AMOUNT
    // - getRiskContribution()
    // Gợi ý:
    // std::string getId() const override;
    // std::string getName() const override;
    // FraudRuleCategory getCategory() const override;
    // double getRiskContribution() const override;


    // TODO 4: Khai báo hàm thẩm định giao dịch ghi đè (override) từ IFraudRule:
    // Gợi ý: bool evaluate(const Transaction& tx, const Customer& customer, FraudAlert& out_alert) override;

};
