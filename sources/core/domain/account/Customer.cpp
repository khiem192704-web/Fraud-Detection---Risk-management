<<<<<<< HEAD
#pragma once
#include <string>
#include "DSA/vector.h"
#include "core/domain/location/location.h"
enum class CustomerRiskTier{
    NEW,
    ESTABLISHED,
    TRUSTED,
    SUSPICIOUS,
    BANNED
};
class Customer{
    private:
        std::string customer_id;
        std::string fullname;
        std::string email;
        std::string phone_number;

        Location home_location;
        CustomerRiskTier risk_tier{CustomerRiskTier::NEW};
        double baseline_risk_score{10.0};
        bool is_kyc_verified{false};

        Vector<std::string>linked_account_ids;
        Vector<std::string>trusted_device_ids;
    public:
        Customer()=default;
        Customer(
            const std::string& id,
            const std::string& name,
            const std::string& mail,
            const std::string& phone,
            const Location& location
        );

        const std::string& getCustomerId() const;
        const std::string& getFullName() const;
        const std::string& getEmail() const;
        const std::string& getPhoneNumber() const;
        const Location& getHomeLocation() const;

        CustomerRiskTier getRiskTier() const;
        double getBaselineRiskScore() const;
        bool isKycVerified() const;

        const Vector<std::string>& getLinkedAccountIds() const;
        const Vector<std::string>& getTrustedDeviceIds() const;

        void setFullName(const std::string& name);
        void setEmail(const std::string& mail);
        void setPhoneNumber(const std::string& phone);
        void setHomeLocation(const Location& location);
        void setRiskTier(CustomerRiskTier tier);
        void setBaselineRiskScore(double score);
        void setKycVerified(bool verified);
        
        void addLinkedAccount(const std::string& account_id);
        void addTrustedDevice(const std::string& device_id);
        bool isDeviceTrusted(const std::string& device_id) const;
=======
#include "Customer.h"

// ==================== Constructor ====================

Customer::Customer() {
    this->customer_id = "";
    this->fullname = "";
    this->email = "";
    this->phone_number = "";
    this->home_location = Location();
    this->risk_tier = CustomerRiskTier::NEW;
    this->baseline_risk_score = 10.0;
    this->is_kyc_verified = false;
}

Customer::Customer(
    const std::string& id,
    const std::string& name,
    const std::string& mail,
    const std::string& phone,
    const Location& location
) {
    this->customer_id = id;
    this->fullname = name;
    this->email = mail;
    this->phone_number = phone;
    this->home_location = location;

    this->risk_tier = CustomerRiskTier::NEW;
    this->baseline_risk_score = 10.0;
    this->is_kyc_verified = false;
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
>>>>>>> main
}