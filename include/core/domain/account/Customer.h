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
        bool is_kyc_verified{false};// KYC: Know your customer ->use for identify infor of user  

        Vector<std::string>linked_account_ids;
        Vector<std::string>trusted_device_ids;
    public:
        // Constructor mặc định
        Customer(){
            this->customer_id = "";
            this->fullname = "";
            this->email = "";
            this->phone_number = "";
            this->home_location = Location();
            this->risk_tier = CustomerRiskTier::NEW;
            this->baseline_risk_score = 10.0;
            this->is_kyc_verified = false;
        }
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
};