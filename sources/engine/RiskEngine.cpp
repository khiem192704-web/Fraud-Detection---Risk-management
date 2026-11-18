#include "engine/RiskEngine.h"

// ============================================================================
// [BƯỚC 4] HIỆN THỰC LỚP RISKENGINE (NHẠC TRƯỞNG CHẤM ĐIỂM RỦI RO)
// Hướng dẫn chi tiết: include/engine/04_STEP4_RISK_ENGINE.md
// ============================================================================

RiskEngine::RiskEngine(const RiskPolicy& custom_policy) {
    // TODO 1: Gán policy tùy chỉnh vào biến thành viên policy
    // Gợi ý:
    // this->policy = custom_policy;


}

RiskEngine::~RiskEngine() {
    // TODO 2: Destructor quan trọng: Duyệt qua mảng rules và gọi `delete rules[i]`
    // để giải phóng bộ nhớ của từng đối tượng luật con (tránh Memory Leak)!
    // Gợi ý:
    // for (int i = 0; i < rules.size(); ++i) {
    //     delete rules[i];
    // }
    // rules.clear();


}

void RiskEngine::registerRule(IFraudRule* rule) {
    // TODO 3: Thêm một luật mới vào danh sách rules nếu con trỏ rule khác nullptr
    // Gợi ý:
    // if (rule != nullptr) {
    //     rules.push_back(rule);
    // }


}

int RiskEngine::getRuleCount() const {
    // TODO 4: Trả về số lượng luật hiện có trong Engine
    // Gợi ý:
    // return rules.size();


}

void RiskEngine::setPolicy(const RiskPolicy& new_policy) {
    // TODO 5: Cập nhật chính sách trọng số mới
    // Gợi ý:
    // this->policy = new_policy;


}

const RiskPolicy& RiskEngine::getPolicy() const {
    // TODO 6: Trả về tham chiếu hằng đến chính sách hiện tại
    // Gợi ý:
    // return policy;


}

RiskAssessment RiskEngine::assessRisk(
    const Transaction& tx,
    const Customer& customer,
    double ml_score
) {
    // TODO 7: Thuật toán thẩm định rủi ro tổng hợp:
    // 1. Khởi tạo std::vector<FraudAlert> alerts và std::vector<std::string> reasons
    // 2. Duyệt qua từng luật trong rules (Áp dụng tính Đa hình - Polymorphism):
    //    FraudAlert alert;
    //    if (rules[i]->evaluate(tx, customer, alert)) {
    //        alerts.push_back(alert);
    //        raw_rule_score += alert.getRiskContribution();
    //        reasons.push_back(alert.getReason());
    //    }
    // 3. Chuẩn hóa điểm rule (không để vượt quá 100.0):
    //    double normalized_rule_score = (raw_rule_score > 100.0) ? 100.0 : raw_rule_score;
    // 4. Lấy điểm rủi ro hồ sơ khách hàng: double profile_score = customer.getBaselineRiskScore();
    // 5. Tính điểm tổng hợp theo trọng số của policy:
    //    combined_score = (policy.weight_rules * normalized_rule_score)
    //                   + (policy.weight_ml * ml_score)
    //                   + (policy.weight_profile * profile_score);
    // 6. Giới hạn combined_score trong đoạn [0.0, 100.0]
    // 7. Xác định RiskLevel: RiskLevel level = policy.determineRiskLevel(combined_score);
    // 8. Khởi tạo đối tượng RiskAssessment và trả về.
    //
    // Gợi ý:
    // std::vector<FraudAlert> alerts;
    // std::vector<std::string> reasons;
    // double raw_rule_score = 0.0;
    // for (int i = 0; i < rules.size(); ++i) {
    //     FraudAlert alert;
    //     if (rules[i]->evaluate(tx, customer, alert)) {
    //         alerts.push_back(alert);
    //         raw_rule_score += alert.getRiskContribution();
    //         reasons.push_back(alert.getReason());
    //     }
    // }
    // double normalized_rule_score = (raw_rule_score > 100.0) ? 100.0 : raw_rule_score;
    // double profile_score = customer.getBaselineRiskScore();
    // double combined_score = (policy.weight_rules * normalized_rule_score)
    //                       + (policy.weight_ml * ml_score)
    //                       + (policy.weight_profile * profile_score);
    // if (combined_score > 100.0) combined_score = 100.0;
    // if (combined_score < 0.0)   combined_score = 0.0;
    // RiskLevel level = policy.determineRiskLevel(combined_score);
    // std::string assessment_id = "ASM_" + tx.GetTransactionID();
    // Timestamp now = std::chrono::system_clock::now();
    // return RiskAssessment(
    //     assessment_id,
    //     tx.GetTransactionID(),
    //     normalized_rule_score,
    //     ml_score,
    //     combined_score,
    //     level,
    //     DecisionAction::APPROVE,
    //     alerts,
    //     reasons,
    //     now
    // );


}
