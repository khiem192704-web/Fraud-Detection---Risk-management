#pragma once
#include <string>
#include <chrono>
#include "common/Types.h"

class FraudAlert {
    private:
        std::string alert_id;
        std::string transaction_id;
        std::string rule_id;
        std::string rule_name;
        FraudRuleCategory category{FraudRuleCategory::VELOCITY};
        double score_contribution{0.0};
        std::string reason;
        RiskLevel severity{RiskLevel::MEDIUM};
        Timestamp triggered_at{std::chrono::system_clock::now()};

    public:
        FraudAlert(){};
        FraudAlert(const std::string&,
                   const std::string&,
                   const std::string&,
                   const std::string&,
                   FraudRuleCategory,
                   double,
                   const std::string&,
                   RiskLevel,
                   Timestamp);

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

