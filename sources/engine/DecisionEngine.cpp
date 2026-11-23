#include "engine/DecisionEngine.h"
DecisionEngine::DecisionEngine(const DecisionPolicy& custom_policy) {
    // TODO 1: Gán chính sách quyết định vào biến thành viên policy
    // Gợi ý:
    this->policy = custom_policy;
}
void DecisionEngine::setPolicy(const DecisionPolicy& new_policy) {
    // TODO 2: Cập nhật chính sách quyết định mới
    // Gợi ý:
    this->policy = new_policy;
}
const DecisionPolicy& DecisionEngine::getPolicy() const {
    // TODO 3: Trả về tham chiếu hằng đến chính sách hiện tại
    // Gợi ý:
    return policy;
}
bool DecisionEngine::checkEmergencyBlock(
    const Customer& customer,
    const RiskAssessment& assessment
) const {
    // TODO 4: Kiểm tra cơ chế Phủ quyết khẩn cấp (Emergency Override):
    // 1. Kiểm tra nếu khách hàng đã bị cấm trên toàn hệ thống:
    //    if (customer.getRiskTier() == CustomerRiskTier::BANNED) return true;
    // 2. Kiểm tra nếu trong danh sách alerts có cảnh báo mức CRITICAL:
    //    const auto& alerts = assessment.getAlerts();
    //    for (size_t i = 0; i < alerts.size(); ++i) {
    //        if (alerts[i].isCritical()) return true;
    //    }
    // 3. Nếu không có điều kiện khẩn cấp: return false;
    //
    // Gợi ý:
    if (customer.getRiskTier() == CustomerRiskTier::BANNED) {
        return true;
    }
    const auto& alerts = assessment.getAlerts();
    for (size_t i = 0; i < alerts.size(); ++i) {
        if (alerts[i].isCritical()) {
            return true;
        }
    }
    return false;
}

DecisionAction DecisionEngine::decide(
    Transaction& tx,
    const Customer& customer,
    RiskAssessment& assessment
) const {
    // TODO 5: Ra quyết định nghiệp vụ và cập nhật trực tiếp trạng thái Transaction:
    // BƯỚC 1: Kiểm tra Phủ quyết khẩn cấp trước tiên:
    //   if (checkEmergencyBlock(customer, assessment)) {
    //       tx.reject(); // Chuyển trạng thái giao dịch sang BLOCKED
    //       return DecisionAction::BLOCK;
    //   }
    // BƯỚC 2: So sánh điểm rủi ro tổng hợp với DecisionPolicy:
    //   double score = assessment.getCombinedScore();
    //   - Nếu score <= policy.approve_max_score:
    //       tx.approve();
    //       return DecisionAction::APPROVE;
    //   - Nếu score <= policy.challenge_max_score:
    //       tx.challenge();
    //       return DecisionAction::CHALLENGE;
    //   - Nếu score <= policy.review_max_score:
    //       tx.sendToReview();
    //       return DecisionAction::REVIEW;
    //   - Ngược lại (> review_max_score):
    //       tx.reject();
    //       return DecisionAction::BLOCK;
    //
    // Gợi ý:
    if (checkEmergencyBlock(customer, assessment)) {
        tx.reject();
        return DecisionAction::BLOCK;
    }
    double score = assessment.getCombinedScore();
    if (score <= policy.approve_max_score) {
        tx.approve();
        return DecisionAction::APPROVE;
    } else if (score <= policy.challenge_max_score) {
        tx.challenge();
        return DecisionAction::CHALLENGE;
    } else if (score <= policy.review_max_score) {
        tx.sendToReview();
        return DecisionAction::REVIEW;
    } else {
        tx.reject();
        return DecisionAction::BLOCK;
    }
}
