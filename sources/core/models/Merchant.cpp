#include "core/models/Merchant.h"

#include <sstream>

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
    double baseline_risk,
    bool blacklisted,
    bool /*whitelisted*/
)
    : merchant_id(merchant_id),
      name(name),
      mcc(mcc),
      category(MerchantCategory::RETAIL),
      location(),
      status(MerchantStatus::ACTIVE),
      risk_tier(blacklisted ? MerchantRiskTier::BLACKLISTED : MerchantRiskTier::STANDARD),
      baseline_risk_score(baseline_risk),
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

bool Merchant::isWhitelisted() const {
    return risk_tier == MerchantRiskTier::TRUSTED;
}

bool Merchant::isHighRiskMcc() const {
    return (mcc == "7995" || mcc == "6051" || mcc == "5967" || mcc == "7273");
}

std::string Merchant::getCategoryDescription() const {
    if (mcc == "5411") return "Siêu thị / Bách hóa thực phẩm (Grocery/Supermarket)";
    if (mcc == "5812") return "Nhà hàng / Dịch vụ ăn uống (Dining/Restaurant)";
    if (mcc == "5311") return "Sàn thương mại điện tử / Bán lẻ (E-Commerce)";
    if (mcc == "5732") return "Cửa hàng Điện máy / Thiết bị điện tử (Electronics)";
    if (mcc == "7995") return "Sòng bạc / Cá cược trực tuyến (Casino/Betting)";
    if (mcc == "6051") return "Mua bán Tiền ảo / Ngoại hối (Crypto/Quasi-Cash)";
    if (mcc == "5967") return "Bán hàng qua điện thoại (Direct Marketing)";
    return "Cửa hàng kinh doanh tổng hợp";
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
        << ", location=" << location.toString()
        << ", status=" << ::toString(status)
        << ", risk_tier=" << ::toString(risk_tier)
        << ", baseline_risk_score=" << baseline_risk_score
        << ", created_at=<timestamp>"
        << '}';

    return oss.str();
}