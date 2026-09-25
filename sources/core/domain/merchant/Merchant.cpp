#include "core/domain/merchant/Merchant.h"

#include <sstream>

// ==================== Enum Helpers ====================

std::string toString(MerchantCategory category) {
    switch (category) {
        case MerchantCategory::RETAIL:
            return "RETAIL";

        case MerchantCategory::GROCERY:
            return "GROCERY";

        case MerchantCategory::ELECTRONICS:
            return "ELECTRONICS";

        case MerchantCategory::RESTAURANT:
            return "RESTAURANT";

        case MerchantCategory::TRAVEL:
            return "TRAVEL";

        case MerchantCategory::ENTERTAINMENT:
            return "ENTERTAINMENT";

        case MerchantCategory::GAMBLING:
            return "GAMBLING";

        case MerchantCategory::CRYPTO:
            return "CRYPTO";

        case MerchantCategory::FINANCIAL_SERVICES:
            return "FINANCIAL_SERVICES";

        case MerchantCategory::OTHER:
            return "OTHER";

        default:
            return "UNKNOWN";
    }
}

std::string toString(MerchantRiskTier tier) {
    switch (tier) {
        case MerchantRiskTier::TRUSTED:
            return "TRUSTED";

        case MerchantRiskTier::STANDARD:
            return "STANDARD";

        case MerchantRiskTier::ELEVATED:
            return "ELEVATED";

        case MerchantRiskTier::SUSPICIOUS:
            return "SUSPICIOUS";

        case MerchantRiskTier::BLACKLISTED:
            return "BLACKLISTED";

        default:
            return "UNKNOWN";
    }
}

std::string toString(MerchantStatus status) {
    switch (status) {
        case MerchantStatus::ACTIVE:
            return "ACTIVE";

        case MerchantStatus::SUSPENDED:
            return "SUSPENDED";

        default:
            return "UNKNOWN";
    }
}

// ==================== Constructor ====================

Merchant::Merchant()
    : merchant_id(""),
      name(""),
      mcc(""),
      category(MerchantCategory::RETAIL),
      location(),
      status(MerchantStatus::ACTIVE),
      risk_tier(MerchantRiskTier::STANDARD),
      baseline_risk_score(10.0),
      created_at(std::chrono::system_clock::now()) {
}

Merchant::Merchant(
    const std::string& merchant_id,
    const std::string& name,
    const std::string& mcc,
    MerchantCategory category,
    const Location& location,
    double baseline_risk_score,
    MerchantRiskTier risk_tier,
    MerchantStatus status,
    Timestamp created_at
)
    : merchant_id(merchant_id),
      name(name),
      mcc(mcc),
      category(category),
      location(location),
      status(status),
      risk_tier(risk_tier),
      baseline_risk_score(baseline_risk_score),
      created_at(created_at) {

    // Đảm bảo risk score luôn nằm trong [0, 100]
    setBaselineRiskScore(baseline_risk_score);
}

// ==================== Getter ====================

const std::string& Merchant::getMerchantId() const {
    return merchant_id;
}

const std::string& Merchant::getName() const {
    return name;
}

const std::string& Merchant::getMcc() const {
    return mcc;
}

MerchantCategory Merchant::getCategory() const {
    return category;
}

const Location& Merchant::getLocation() const {
    return location;
}

MerchantStatus Merchant::getStatus() const {
    return status;
}

MerchantRiskTier Merchant::getRiskTier() const {
    return risk_tier;
}

double Merchant::getBaselineRiskScore() const {
    return baseline_risk_score;
}

Timestamp Merchant::getCreatedAt() const {
    return created_at;
}

// ==================== Setter ====================

void Merchant::setName(const std::string& name) {
    this->name = name;
}

void Merchant::setMcc(const std::string& mcc) {
    this->mcc = mcc;
}

void Merchant::setCategory(MerchantCategory category) {
    this->category = category;
}

void Merchant::setLocation(const Location& location) {
    this->location = location;
}

void Merchant::setBaselineRiskScore(double score) {
    if (score < 0.0) {
        baseline_risk_score = 0.0;
    }
    else if (score > 100.0) {
        baseline_risk_score = 100.0;
    }
    else {
        baseline_risk_score = score;
    }
}

// ==================== Business Operations ====================

void Merchant::activate() {
    // Merchant bị blacklist không thể tự activate.
    if (risk_tier == MerchantRiskTier::BLACKLISTED) {
        return;
    }

    status = MerchantStatus::ACTIVE;
}

void Merchant::suspend() {
    status = MerchantStatus::SUSPENDED;
}

void Merchant::blacklist() {
    status = MerchantStatus::SUSPENDED;
    risk_tier = MerchantRiskTier::BLACKLISTED;
    baseline_risk_score = 100.0;
}

// ==================== State Checking ====================

bool Merchant::isActive() const {
    return status == MerchantStatus::ACTIVE;
}

bool Merchant::isSuspended() const {
    return status == MerchantStatus::SUSPENDED;
}

bool Merchant::isBlacklisted() const {
    return risk_tier == MerchantRiskTier::BLACKLISTED;
}

bool Merchant::isHighRisk() const {
    return risk_tier == MerchantRiskTier::SUSPICIOUS ||
           risk_tier == MerchantRiskTier::BLACKLISTED ||
           baseline_risk_score >= 60.0;
}

// ==================== String Representation ====================

std::string Merchant::toString() const {
    std::ostringstream oss;

    oss << "Merchant{"
        << "merchant_id='" << merchant_id << '\''
        << ", name='" << name << '\''
        << ", mcc='" << mcc << '\''
        << ", category=" << ::toString(category)
        << ", location=" << location.summary()
        << ", status=" << ::toString(status)
        << ", risk_tier=" << ::toString(risk_tier)
        << ", baseline_risk_score=" << baseline_risk_score
        << ", created_at=<timestamp>"
        << '}';

    return oss.str();
}

// ==================== Operator ====================

std::ostream& operator<<(std::ostream& os, const Merchant& merchant) {
    os << merchant.toString();
    return os;
}