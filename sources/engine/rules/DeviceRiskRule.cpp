#include "engine/rules/DeviceRiskRule.h"
DeviceRiskRule::DeviceRiskRule(double contribution) {
    // TODO 1: Gán giá trị contribution vào biến thành viên risk_contribution
    // Gợi ý:
    // this->risk_contribution = contribution;
    this->risk_cons


}

std::string DeviceRiskRule::getId() const {
    // TODO 2: Trả về mã định danh duy nhất của luật ("RULE_004_DEVICE_RISK")
    // Gợi ý:
    // return "RULE_004_DEVICE_RISK";


}

std::string DeviceRiskRule::getName() const {
    // TODO 3: Trả về tên mô tả ngắn gọn của luật
    // Gợi ý:
    // return "Suspicious Device Environment";


}

FraudRuleCategory DeviceRiskRule::getCategory() const {
    // TODO 4: Trả về enum FraudRuleCategory phù hợp trong core/common/Types.h
    // Gợi ý:
    // return FraudRuleCategory::SUSPICIOUS_DEVICE;


}

double DeviceRiskRule::getRiskContribution() const {
    // TODO 5: Trả về điểm rủi ro đóng góp khi luật bị vi phạm
    // Gợi ý:
    // return risk_contribution;


}

bool DeviceRiskRule::evaluate(
    const Transaction& tx,
    const Customer& /*customer*/,
    FraudAlert& out_alert
) {
    // TODO 6: Thẩm định giao dịch dựa trên môi trường thiết bị:
    // 1. Lấy đối tượng Device từ giao dịch: const Device& dev = tx.GetDevice();
    // 2. Kiểm tra cờ máy ảo: dev.GetIs_Virtual() hoặc cờ can thiệp hệ thống: dev.GetIs_Hacker()
    // 3. Nếu vi phạm:
    //    - Xác định mức độ nghiêm trọng: nếu là hacker/root thì RiskLevel::CRITICAL, nếu là máy ảo thì RiskLevel::HIGH
    //    - Gán out_alert = FraudAlert(...) đầy đủ thông tin:
    //      + alert_id: "ALT_" + tx.GetTransactionID() + "_DEV"
    //      + transaction_id: tx.GetTransactionID()
    //      + rule_id: getId()
    //      + rule_name: getName()
    //      + reason: dev.GetIs_Hacker() ? "Thiet bi bi can thiep he thong (Root/Jailbreak)" : "Giao dich tu moi truong may ao / gia lap"
    //      + category: getCategory()
    //      + score_contribution: getRiskContribution()
    //      + severity: dev.GetIs_Hacker() ? RiskLevel::CRITICAL : RiskLevel::HIGH
    //    - return true;
    // 4. Nếu thiết bị an toàn: return false;
    //
    // Gợi ý:
    // const Device& dev = tx.GetDevice();
    // if (dev.GetIs_Virtual() || dev.GetIs_Hacker()) {
    //     std::string reason = dev.GetIs_Hacker() 
    //         ? "Thiet bi bi can thiep he thong (Root/Jailbreak)" 
    //         : "Giao dich tu moi truong may ao / gia lap";
    //     RiskLevel severity = dev.GetIs_Hacker() ? RiskLevel::CRITICAL : RiskLevel::HIGH;
    //     out_alert = FraudAlert(
    //         "ALT_" + tx.GetTransactionID() + "_DEV",
    //         tx.GetTransactionID(),
    //         getId(),
    //         getName(),
    //         reason,
    //         getCategory(),
    //         getRiskContribution(),
    //         severity
    //     );
    //     return true;
    // }
    // return false;


}
