#pragma once
#include <string>
#include <vector>
#include "engine/rules/IFraudRule.h"
#include "engine/FeatureExtractor.h"
#include "engine/NativeMLPredictor.h"
#include "core/common/Types.h"
#include "core/models/Transaction.h"
#include "core/models/Customer.h"
#include "core/models/RiskAssessment.h"
#include "core/models/FraudAlert.h"

struct RiskPolicy {
    double weight_rules{0.5};
    double weight_ml{0.4};
    double weight_profile{0.1};

    double threshold_very_low{20.0};
    double threshold_low{40.0};
    double threshold_medium{60.0};
    double threshold_high{80.0};

    RiskLevel determineRiskLevel(double score) const {
        if (score < threshold_very_low) return RiskLevel::VERY_LOW;
        if (score < threshold_low)      return RiskLevel::LOW;
        if (score < threshold_medium)   return RiskLevel::MEDIUM;
        if (score < threshold_high)     return RiskLevel::HIGH;
        return RiskLevel::CRITICAL;
    }
};

class RiskEngine {
private:
    std::vector<IFraudRule*> rules;
    RiskPolicy policy;
    FeatureExtractor feature_extractor;
    NativeMLPredictor ml_predictor;

public:
    explicit RiskEngine(const RiskPolicy& custom_policy = RiskPolicy());
    ~RiskEngine();

    void registerRule(IFraudRule* rule);
    int getRuleCount() const;
    void setPolicy(const RiskPolicy& new_policy);
    const RiskPolicy& getPolicy() const;

    NativeMLPredictor& getMLPredictor() { return ml_predictor; }
    FeatureExtractor& getFeatureExtractor() { return feature_extractor; }

    // Thẩm định rủi ro tự chủ: Tự trích xuất 4 đặc trưng và suy luận điểm ML
    RiskAssessment assessRisk(const Transaction& tx, const Customer& customer);

    // Thẩm định rủi ro với điểm ML chỉ định thủ công
    RiskAssessment assessRisk(const Transaction& tx, const Customer& customer, double ml_score);
};
