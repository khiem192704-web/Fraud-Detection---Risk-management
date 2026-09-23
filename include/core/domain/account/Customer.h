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

        std::vector<std::string>linked_account_ids;
        std::vector<std::string>trusted_device_ids;
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

        const std::vector<std::string>& getLinkedAccountIds() const;
        const std::vector<std::string>& getTrustedDeviceIds() const;

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
}