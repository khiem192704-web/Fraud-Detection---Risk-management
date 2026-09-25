#include "core/domain/account/Customer.h"

// ==================== Constructor ====================

Customer::Customer()
    : customer_id(""),
      fullname(""),
      email(""),
      phone_number(""),
      home_location(),
      risk_tier(CustomerRiskTier::NEW),
      baseline_risk_score(10.0),
      is_kyc_verified(false) {
}

Customer::Customer(
    const std::string& id,
    const std::string& name,
    const std::string& mail,
    const std::string& phone,
    const Location& location
)
    : customer_id(id),
      fullname(name),
      email(mail),
      phone_number(phone),
      home_location(location),
      risk_tier(CustomerRiskTier::NEW),
      baseline_risk_score(10.0),
      is_kyc_verified(false) {
}

// ==================== Getter ====================

const std::string& Customer::getCustomerId() const {
    return customer_id;
}

const std::string& Customer::getFullName() const {
    return fullname;
}

const std::string& Customer::getEmail() const {
    return email;
}

const std::string& Customer::getPhoneNumber() const {
    return phone_number;
}

const Location& Customer::getHomeLocation() const {
    return home_location;
}

CustomerRiskTier Customer::getRiskTier() const {
    return risk_tier;
}

double Customer::getBaselineRiskScore() const {
    return baseline_risk_score;
}

bool Customer::isKycVerified() const {
    return is_kyc_verified;
}

const Vector<std::string>& Customer::getLinkedAccountIds() const {
    return linked_account_ids;
}

const Vector<std::string>& Customer::getTrustedDeviceIds() const {
    return trusted_device_ids;
}

// ==================== Setter ====================

void Customer::setFullName(const std::string& name) {
    this->fullname = name;
}

void Customer::setEmail(const std::string& mail) {
    this->email = mail;
}

void Customer::setPhoneNumber(const std::string& phone) {
    this->phone_number = phone;
}

void Customer::setHomeLocation(const Location& location) {
    this->home_location = location;
}

void Customer::setRiskTier(CustomerRiskTier tier) {
    this->risk_tier = tier;
}

void Customer::setBaselineRiskScore(double score) {
    this->baseline_risk_score = score;
}

void Customer::setKycVerified(bool verified) {
    this->is_kyc_verified = verified;
}

// ==================== Account / Device ====================

void Customer::addLinkedAccount(const std::string& account_id) {
    linked_account_ids.push_back(account_id);
}

void Customer::addTrustedDevice(const std::string& device_id) {
    trusted_device_ids.push_back(device_id);
}

bool Customer::isDeviceTrusted(const std::string& device_id) const {
    for (int i = 0; i < trusted_device_ids.size(); ++i) {
        if (trusted_device_ids[i] == device_id) {
            return true;
        }
    }

    return false;
}