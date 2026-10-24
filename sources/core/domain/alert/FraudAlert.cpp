#include"core/domain/alert/FraudAlert.h"

FraudAlert::FraudAlert(){};
FraudAlert::FraudAlert(const std::string& alert_id, const std::string& transaction_id,
                    const std::string& rule_id, const std::string& rule_name,
                    FraudRuleCategory category = FraudRuleCategory::RAPID_TRANSACTIONS,
                    double score_contribution = 0.0, const std::string& reason, RiskLevel severity = RiskLevel::MEDIUM,
                    Timestamp triggered_at = Timestamp{std::chrono::system_clock::now()}):
                            alert_id(alert_id), transaction_id(transaction_id), rule_id(rule_id), rule_name(rule_name),
                            category(category), score_contribution(score_contribution), reason(reason), severity(severity),
                            triggered_at(triggered_at) {}

// --- Getters ---
const std::string& FraudAlert::getAlertId() const {
    return alert_id;
}

const std::string& FraudAlert::getTransactionId() const {
    return transaction_id;
}

const std::string& FraudAlert::getRuleId() const {
    return rule_id;
}

const std::string& FraudAlert::getRuleName() const {
    return rule_name;
}

FraudRuleCategory FraudAlert::getCategory() const {
    return category;
}

double FraudAlert::getRiskContribution() const {
    return score_contribution;
}

const std::string& FraudAlert::getReason() const {
    return reason;
}

RiskLevel FraudAlert::getSeverity() const {
    return severity;
}

Timestamp FraudAlert::getTriggeredAt() const {
    return triggered_at;
}

// --- Summary & Inspection ---
bool FraudAlert::isCritical() const {
    return severity == RiskLevel::HIGH;
}
std::string FraudAlert::getSummary() const {
    return "Alert ID: " + alert_id + ", Transaction ID: " + transaction_id + ", Rule ID: " + rule_id +
           ", Rule Name: " + rule_name + ", Category: " + std::to_string(static_cast<int>(category)) +
           ", Score Contribution: " + std::to_string(score_contribution) + ", Reason: " + reason +
           ", Severity: " + std::to_string(static_cast<int>(severity)) +
           ", Triggered At: " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(triggered_at.time_since_epoch()).count());
}