#include "engine/RiskEngine.h"

RiskEngine::RiskEngine(const RiskPolicy& custom_policy) {
    this->policy = custom_policy;
    this->ml_predictor.loadModel("data/model_weights.json");
}

RiskEngine::~RiskEngine() {
    for (size_t i = 0; i < rules.size(); ++i) {
        delete rules[i];
    }
    rules.clear();
}

void RiskEngine::registerRule(IFraudRule* rule) {
    if (rule != nullptr) {
        rules.push_back(rule);
    }
}

int RiskEngine::getRuleCount() const {
    return static_cast<int>(rules.size());
}

void RiskEngine::setPolicy(const RiskPolicy& new_policy) {
    this->policy = new_policy;
}

const RiskPolicy& RiskEngine::getPolicy() const {
    return policy;
}

RiskAssessment RiskEngine::assessRisk(const Transaction& tx, const Customer& customer) {
    // 1. Trích xuất vector 4 đặc trưng từ transaction và customer
    std::vector<double> features = feature_extractor.extract(tx, customer);

    // 2. Dự đoán điểm rủi ro ML Score [0.0 -> 100.0]
    double computed_ml_score = ml_predictor.predictScore(features);

    // 3. Tính toán điểm thẩm định tổng hợp
    return assessRisk(tx, customer, computed_ml_score);
}

RiskAssessment RiskEngine::assessRisk(
    const Transaction& tx,
    const Customer& customer,
    double ml_score
) {
    std::vector<FraudAlert> alerts;
    std::vector<std::string> reasons;
    double raw_rule_score = 0.0;

    for (size_t i = 0; i < rules.size(); ++i) {
        FraudAlert alert;
        if (rules[i]->evaluate(tx, customer, alert)) {
            alerts.push_back(alert);
            raw_rule_score += alert.getRiskContribution();
            reasons.push_back(alert.getReason());
        }
    }

    double normalized_rule_score = (raw_rule_score > 100.0) ? 100.0 : raw_rule_score;
    double profile_score = customer.getBaselineRiskScore();
    double combined_score = (policy.weight_rules * normalized_rule_score)
                          + (policy.weight_ml * ml_score)
                          + (policy.weight_profile * profile_score);

    if (combined_score > 100.0) combined_score = 100.0;
    if (combined_score < 0.0)   combined_score = 0.0;

    RiskLevel level = policy.determineRiskLevel(combined_score);
    std::string assessment_id = "ASM_" + tx.GetTransactionID();
    Timestamp now = std::chrono::system_clock::now();

    return RiskAssessment(
        assessment_id,
        tx.GetTransactionID(),
        normalized_rule_score,
        ml_score,
        combined_score,
        level,
        DecisionAction::APPROVE,
        alerts,
        reasons,
        now
    );
}
