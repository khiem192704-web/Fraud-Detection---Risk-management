#pragma once
#include <string>
#include <chrono>
#include "core/common/Types.h"

class FraudAlert {
    private:
        std::string alert_id;
        std::string transaction_id;
        std::string rule_id;
        std::string rule_name;
        FraudRuleCategory category{FraudRuleCategory::RAPID_TRANSACTIONS};
        double score_contribution{0.0};
        std::string reason;
        RiskLevel severity{RiskLevel::MEDIUM};
        Timestamp triggered_at{std::chrono::system_clock::now()};

    public:
        FraudAlert();
        FraudAlert(const std::string& alert_id,
                   const std::string& transaction_id,
                   const std::string& rule_id,
                   const std::string& rule_name,
                   const std::string& reason,
                   FraudRuleCategory category = FraudRuleCategory::RAPID_TRANSACTIONS,
                   double score_contribution = 0.0,
                   RiskLevel severity = RiskLevel::MEDIUM,
                   Timestamp triggered_at = Timestamp{std::chrono::system_clock::now()});

        // --- Getters ---
        const std::string& getAlertId() const;
        const std::string& getTransactionId() const;
        const std::string& getRuleId() const;
        const std::string& getRuleName() const;
        FraudRuleCategory getCategory() const;
        double getRiskContribution() const;    // Đổi scoreContribution -> getRiskContribution (mức độ đóng góp rủi ro)
        const std::string& getReason() const;
        RiskLevel getSeverity() const;
        Timestamp getTriggeredAt() const;

        // --- Summary & Inspection ---
        bool isCritical() const;               // Kiểm tra nhanh cảnh báo có ở mức nghiêm trọng hay không
        std::string getSummary() const;        // Đồng bộ thống nhất với getSummary() của Transaction và RiskAssessment
};

