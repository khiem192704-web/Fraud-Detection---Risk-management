#pragma once

#include <string>
#include <chrono>
#include <ostream>
#include "core/common/Types.h"
#include "core/models/Location.h"

class Merchant {
private:
    std::string merchant_id;
    std::string name;
    std::string mcc;

    MerchantCategory category;
    Location location;

    MerchantStatus status;
    MerchantRiskTier risk_tier;

    double baseline_risk_score;

    Timestamp created_at;

public:
    // Constructor
    Merchant();

    Merchant(
        const std::string& merchant_id,
        const std::string& name,
        const std::string& mcc,
        double baseline_risk = 10.0,
        bool blacklisted = false,
        bool whitelisted = false
    );

    Merchant(
        const std::string& merchant_id,
        const std::string& name,
        const std::string& mcc,
        MerchantCategory category,
        const Location& location,
        double baseline_risk_score = 10.0,
        MerchantRiskTier risk_tier = MerchantRiskTier::STANDARD,
        MerchantStatus status = MerchantStatus::ACTIVE,
        Timestamp created_at = std::chrono::system_clock::now()
    );

    // Getter
    const std::string& getMerchantId() const;
    const std::string& getName() const;
    const std::string& getMcc() const;
    double getBaselineRisk() const { return baseline_risk_score; }
    bool isBlacklisted() const;
    bool isWhitelisted() const;
    bool isHighRiskMcc() const;
    std::string getCategoryDescription() const;

    MerchantCategory getCategory() const;
    const Location& getLocation() const;

    MerchantStatus getStatus() const;
    MerchantRiskTier getRiskTier() const;

    double getBaselineRiskScore() const;
    Timestamp getCreatedAt() const;

    // Setter
    void setName(const std::string& name);
    void setMcc(const std::string& mcc);
    void setCategory(MerchantCategory category);
    void setLocation(const Location& location);
    void setBaselineRiskScore(double score);

    // Business operations
    void activate();
    void suspend();
    void blacklist();

    // State checking
    bool isActive() const;
    bool isSuspended() const;
    bool isHighRisk() const;

    // String representation
    std::string toString() const;
};