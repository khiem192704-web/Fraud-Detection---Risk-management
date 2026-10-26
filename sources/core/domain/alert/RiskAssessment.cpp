#include"core/domain/alert/RiskAssessment.h"

RiskAssessment::RiskAssessment(const std::string& assessment_id, const std::string& transaction_id, double rule_score, double ml_score, double combined_score,
                               RiskLevel risk_level, DecisionAction decision, const Vector<FraudAlert>& alerts, const Vector<std::string>& reasons, Timestamp evaluated_at):
        assessment_id(assessment_id), transaction_id(transaction_id), rule_score(rule_score), ml_score(ml_score), combined_score(combined_score),
        risk_level(risk_level), decision(decision), alerts(alerts), reasons(reasons), evaluated_at(evaluated_at) {}

// --- Getters ---
const std::string& RiskAssessment::getAssessmentId() const {
    return assessment_id;
}

const std::string& RiskAssessment::getTransactionId() const {
    return transaction_id;
}

double RiskAssessment::getRuleScore() const {
    return rule_score;
}

double RiskAssessment::getMlScore() const {
    return ml_score;
}

double RiskAssessment::getCombinedScore() const {
    return combined_score;
}

RiskLevel RiskAssessment::getRiskLevel() const {
    return risk_level;
}

DecisionAction RiskAssessment::getDecision() const {
    return decision;
}

const Vector<FraudAlert>& RiskAssessment::getAlerts() const {
    return alerts;
}

const Vector<std::string>& RiskAssessment::getReasons() const {
    return reasons;
}

Timestamp RiskAssessment::getEvaluatedAt() const {
    return evaluated_at;
}

// --- Domain Actions ---
void RiskAssessment::addAlert(const FraudAlert& alert) {
    alerts.push_back(alert);
}

void RiskAssessment::addReason(const std::string& reason) {
    reasons.push_back(reason);
}

// --- Decision Checks ---
bool RiskAssessment::isApproved() const {
    return decision == DecisionAction::APPROVE;
}

bool RiskAssessment::isReview() const {
    return decision == DecisionAction::REVIEW;
}

bool RiskAssessment::isChallenge() const {
    return decision == DecisionAction::CHALLENGE_3DS;
}

bool RiskAssessment::isBlocked() const {
    return decision == DecisionAction::BLOCK;
}

std::string RiskAssessment::getSummary() const {
    return "Assessment ID: " + assessment_id + ", Transaction ID: " + transaction_id + ", Rule Score: " + std::to_string(rule_score) +
           ", ML Score: " + std::to_string(ml_score) + ", Combined Score: " + std::to_string(combined_score) +
           ", Risk Level: " + std::to_string(static_cast<int>(risk_level)) +
           ", Decision: " + std::to_string(static_cast<int>(decision)) +
           ", Evaluated At: " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(evaluated_at.time_since_epoch()).count());
}