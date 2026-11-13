#include "engine/rules/DeviceRiskRule.h"

// ============================================================================
// KHUNG SƯỜN ĐỂ BẠN TỰ CODE BƯỚC 2.1 (DeviceRiskRule)
// Hãy đọc hướng dẫn chi tiết tại: include/engine/02_STEP2_CONCRETE_RULES.md
// ============================================================================

DeviceRiskRule::DeviceRiskRule(double contribution) {
    // TODO: Gán giá trị contribution vào biến thành viên risk_contribution
}

std::string DeviceRiskRule::getId() const {
    // TODO: Trả về mã định danh duy nhất của luật (vd: "RULE_004_DEVICE_RISK")
    return "";
}

std::string DeviceRiskRule::getName() const {
    // TODO: Trả về tên mô tả của luật (vd: "Suspicious Device Environment")
    return "";
}

FraudRuleCategory DeviceRiskRule::getCategory() const {
    // TODO: Trả về enum FraudRuleCategory phù hợp trong core/common/Types.h
    // (Gợi ý: FraudRuleCategory::SUSPICIOUS_DEVICE)
    return FraudRuleCategory::SUSPICIOUS_DEVICE;
}

double DeviceRiskRule::getRiskContribution() const {
    // TODO: Trả về điểm rủi ro đóng góp (risk_contribution)
    return 0.0;
}

bool DeviceRiskRule::evaluate(
    const Transaction& tx,
    const Customer& /*customer*/,
    FraudAlert& out_alert
) {
    // TODO:
    // 1. Lấy thông tin thiết bị: const Device& dev = tx.GetDevice();
    // 2. Kiểm tra xem thiết bị có phải máy ảo không: dev.GetIs_Virtual()
    // 3. Kiểm tra xem thiết bị có bị can thiệp/root không: dev.GetIs_Hacker()
    // 4. Nếu vi phạm:
    //    - Khởi tạo đối tượng FraudAlert với đầy đủ thông tin:
    //      alert_id, transaction_id, rule_id, rule_name, reason, category, score, severity
    //    - Gán vào tham số đầu ra out_alert
    //    - return true;
    // 5. Nếu không vi phạm:
    //    - return false;

    return false;
}

