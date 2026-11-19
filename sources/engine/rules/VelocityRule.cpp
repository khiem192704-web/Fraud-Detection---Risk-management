#include "engine/rules/VelocityRule.h"
VelocityRule::VelocityRule(
    int max_txs,
    std::chrono::seconds duration,
    double contribution
){
    // TODO 1: Gán các tham số cấu hình cửa sổ trượt vào biến thành viên
    // Gợi ý:
    this->max_transactions = max_txs;
    this->window_duration = duration;
    this->risk_contribution = contribution;
}
std::string VelocityRule::getId() const {
    // TODO 2: Trả về mã định danh duy nhất của luật ("RULE_001_VELOCITY")
    // Gợi ý:
    return "RULE_001_VELOCITY";
}
std::string VelocityRule::getName() const {
    // TODO 3: Trả về tên mô tả ngắn gọn của luật
    // Gợi ý:
    return "High Transaction Frequency (Velocity)";
}
FraudRuleCategory VelocityRule::getCategory() const {
    // TODO 4: Trả về danh mục FraudRuleCategory::RAPID_TRANSACTIONS
    // Gợi ý:
    return FraudRuleCategory::RAPID_TRANSACTIONS;
}
double VelocityRule::getRiskContribution() const {
    // TODO 5: Trả về điểm rủi ro đóng góp khi luật bị vi phạm
    // Gợi ý:
    return risk_contribution;
}

void VelocityRule::reset() {
    // TODO 6: Làm rỗng cửa sổ trượt (dùng khi bắt đầu phiên kiểm thử mới)
    // Gợi ý:
    while (!sliding_window.empty()) {
        sliding_window.pop_front();
    }
}
bool VelocityRule::evaluate(
    const Transaction& tx,
    const Customer& /*customer*/,
    FraudAlert& out_alert
){
    // TODO 7: Cài đặt thuật toán Cửa sổ trượt thời gian (Sliding Time Window với Deque):
    // 1. Lấy mốc thời gian của giao dịch hiện tại: Timestamp now = tx.GetTimestamp();
    // 2. Trượt cửa sổ: Loại bỏ các giao dịch cũ nằm ngoài khoảng [now - window_duration, now]:
    //    while (!sliding_window.empty()) {
    //        auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - sliding_window.front()).count();
    //        if (diff > window_duration.count()) {
    //            sliding_window.pop_front();
    //        } else {
    //            break;
    //        }
    //    }
    // 3. Đưa mốc thời gian hiện tại vào cuối Deque: sliding_window.push_back(now);
    // 4. Kiểm tra tần suất: Nếu sliding_window.size() > max_transactions:
    //    - Gán out_alert = FraudAlert(...) với mức độ HIGH (RiskLevel::HIGH)
    //    - return true;
    // 5. Nếu chưa vượt ngưỡng: return false;
    //
    // Gợi ý:
    Timestamp now=tx.GetTimestamp();
    while (!sliding_window.empty()) {
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - sliding_window.front()).count();
        if (diff > window_duration.count()) {
            sliding_window.pop_front();
        } else {
            break;
        }
    }
    sliding_window.push_back(now);
    if (sliding_window.size() > max_transactions) {
        out_alert = FraudAlert(
            "ALT_" + tx.GetTransactionID() + "_VEL",
            tx.GetTransactionID(),
            getId(),
            getName(),
            "Tần suất giao dịch bất thường: "+std::to_string(sliding_window.size())+" giao dịch trong " + std::to_string(window_duration.count())+"s",
            getCategory(),
            getRiskContribution(),
            RiskLevel::HIGH
        );
        return true;
    }
    return false;
}
