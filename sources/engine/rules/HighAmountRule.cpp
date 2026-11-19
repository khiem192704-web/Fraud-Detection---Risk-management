#include "engine/rules/HighAmountRule.h"
HighAmountRule::HighAmountRule(
    double max_amount,
    double multiplier,
    double contribution
){
    // TODO 1: Gán các tham số ngưỡng và điểm đóng góp vào các biến thành viên tương ứng
    // Gợi ý:
    this->max_amount_threshold = max_amount;
    this->multiplier_threshold = multiplier;
    this->risk_contribution = contribution;
}
std::string HighAmountRule::getId() const {
    // TODO 2: Trả về mã định danh duy nhất của luật ("RULE_002_HIGH_AMOUNT")
    // Gợi ý:
    return "RULE_002_HIGH_AMOUNT";
}
std::string HighAmountRule::getName() const {
    // TODO 3: Trả về tên mô tả ngắn gọn của luật
    // Gợi ý:
    return "Unusual High Transaction Amount";
}
FraudRuleCategory HighAmountRule::getCategory() const {
    // TODO 4: Trả về danh mục FraudRuleCategory::UNUSUAL_AMOUNT
    // Gợi ý:
    return FraudRuleCategory::UNUSUAL_AMOUNT;
}
double HighAmountRule::getRiskContribution() const {
    // TODO 5: Trả về điểm rủi ro đóng góp khi vi phạm luật
    // Gợi ý:
    return risk_contribution;
}
bool HighAmountRule::evaluate(
    const Transaction& tx,
    const Customer& /*customer*/,
    FraudAlert& out_alert
){
    // TODO 6: Thẩm định số tiền giao dịch:
    // 1. Lấy số tiền giao dịch: double amount = tx.GetAmount();
    // 2. Kiểm tra nếu amount >= max_amount_threshold:
    //    - Gán out_alert = FraudAlert(...) với:
    //      + alert_id: "ALT_" + tx.GetTransactionID() + "_AMT"
    //      + transaction_id: tx.GetTransactionID()
    //      + rule_id: getId()
    //      + rule_name: getName()
    //      + reason: "So tien giao dich vuot nguong tran an toan: $" + std::to_string(amount)
    //      + category: getCategory()
    //      + score_contribution: getRiskContribution()
    //      + severity: RiskLevel::HIGH
    //    - return true;
    // 3. Nếu số tiền an toàn: return false;
    //
    // Gợi ý:
    double amount = tx.GetAmount();
    if (amount >= max_amount_threshold) {
        out_alert = FraudAlert(
            "ALT_" + tx.GetTransactionID() + "_AMT",
            tx.GetTransactionID(),
            getId(),
            getName(),
            "Số tiền giao dịch vượt ngưỡng an toàn : $" + std::to_string(amount),
            getCategory(),
            getRiskContribution(),
            RiskLevel::HIGH
        );
        return true;
    }
    return false;
}
