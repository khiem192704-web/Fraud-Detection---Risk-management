#pragma once
#include <string>
#include "dsa/vector.h"
#include "FraudAlert.h"
#include <chrono>
#include "common/Types.h"

class RiskAssessment {
    private:
        std::string assessment_id;
        std::string transaction_id;
        double rule_score{0.0};
        double ml_score{0.0};
        double combined_score{0.0};
        RiskLevel risk_level{RiskLevel::VERY_LOW};
        DecisionAction decision{DecisionAction::APPROVE};
        Vector<FraudAlert> alerts;
        Vector<std::string> reasons;
        Timestamp evaluated_at{std::chrono::system_clock::now()};

    public:
        RiskAssessment(){};
        RiskAssessment(const std::string&, const std::string&, double, double, double, RiskLevel, DecisionAction, 
                    const Vector<FraudAlert>&, const Vector<std::string>&, Timestamp);

        // --- Getters ---
        const std::string& getAssessmentId() const;
        const std::string& getTransactionId() const;
        double getRuleScore() const;
        double getMlScore() const;
        double getCombinedScore() const;
        RiskLevel getRiskLevel() const;
        DecisionAction getDecision() const;
        const Vector<FraudAlert>& getAlerts() const;
        const Vector<std::string>& getReasons() const;
        Timestamp getEvaluatedAt() const;

        // --- Domain Actions ---
        void addAlert(const FraudAlert&);
        void addReason(const std::string&);

        // --- Decision Checks ---
        bool isApproved() const;
        bool isUnderReview() const;         // Đổi từ isReviewed -> isUnderReview (đang chờ duyệt)
        bool isChallengeRequired() const;   // Đổi từ isChallenged -> isChallengeRequired (cần xác thực 3D Secure/OTP)
        bool isBlocked() const;

        std::string getSummary() const;     // Đồng bộ tên hàm tóm tắt dữ liệu
};

