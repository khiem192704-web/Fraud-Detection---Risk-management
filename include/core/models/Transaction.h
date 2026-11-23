#pragma once
#include <string>
#include <chrono>
#include "core/common/Types.h"
#include "core/models/Location.h"
#include "core/models/Device.h"
#include "core/models/Merchant.h"

class Transaction {
    private:
        std::string transactionid;
        TransactionType type{TransactionType::BUY};
        std::string customerid;
        std::string recipientid;
        std::string accountid;
        double amount{0.0};
        std::string currency{"USD"};
        Timestamp timestamp{std::chrono::system_clock::now()};
        Location location;
        std::string ip_address;
        Device device;
        std::string raw_card_number;
        Merchant merchant{"M_DEFAULT", "Default Retailer", "5411", 5.0};
        std::string merchantid;
        PaymentType payment_method;
        TransactionStatus status{TransactionStatus::PENDING};
        Timestamp created_at{std::chrono::system_clock::now()};
        Timestamp updated_at{std::chrono::system_clock::now()};

    public:
        Transaction();
        Transaction(const std::string&, TransactionType, const std::string&, const std::string&, const std::string&, double, 
            const std::string&, Timestamp, const Location&, const std::string&, const Device&, const std::string&, const PaymentType&);
        Transaction(const std::string& txId, TransactionType type, const std::string& customerId,
                    const std::string& accountId, double amount, const std::string& currency,
                    const Location& loc, const Device& dev, const std::string& cardNum = "",
                    const Merchant& mer = Merchant("M_DEFAULT", "Default Retailer", "5411", 5.0),
                    Timestamp time = std::chrono::system_clock::now());

        // Setters / State updates
        void SetStatus(TransactionStatus);
        void SetAmount(double);
        void SetCurrency(const std::string&);
        void SetLocation(const Location&);
        void SetDevice(const Device&);
        void setStatus(TransactionStatus s) { SetStatus(s); }

        // Getters
        const std::string& GetTransactionID() const;
        TransactionType GetType() const;
        const std::string& GetCustomerID() const;
        const std::string& GetRecipientID() const;
        const std::string& GetAccountID() const;
        double GetAmount() const;
        const std::string& GetCurrency() const;
        Timestamp GetTimestamp() const;
        const Location& GetLocation() const;
        const std::string& GetIP_Address() const;
        const Device& GetDevice() const;
        const std::string& GetMerchantID() const;
        const PaymentType& GetPayment_Method() const;
        TransactionStatus GetStatus() const;
        Timestamp GetCreated_At() const;
        Timestamp GetUpdated_At() const;

        // Alias Getters (camelCase)
        const std::string& getTransactionId() const { return transactionid; }
        TransactionType getType() const { return type; }
        const std::string& getCustomerId() const { return customerid; }
        const std::string& getAccountId() const { return accountid; }
        double getAmount() const { return amount; }
        const std::string& getCurrency() const { return currency; }
        Timestamp getTimestamp() const { return timestamp; }
        const Location& getLocation() const { return location; }
        const Device& getDevice() const { return device; }
        const Merchant& getMerchant() const { return merchant; }
        void setMerchant(const Merchant& m) { merchant = m; }
        TransactionStatus getStatus() const { return status; }
        const std::string& getRawCardNumber() const { return raw_card_number; }

        // Lifecycle state transitions
        bool approve();      
        bool sendToReview(); 
        bool challenge(); 
        bool reject();
        bool block() { return reject(); }
        bool settle(); 
        bool chargeback();
        bool dispute();  

        // --- Business Checks (Kiểm tra nghiệp vụ) ---
        bool isCrossBorder(const std::string&) const;
        bool isHighValue(double) const;
        std::string toString() const;
};
