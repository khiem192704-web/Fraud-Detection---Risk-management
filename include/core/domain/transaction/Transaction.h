#pragma once
#include <string>
#include <chrono>
#include "common/Types.h"
#include "Location.h"
#include "Device.h"

namespace epfd {

class Transaction {
    private:
        std::string transaction_id;
        TransactionType type{TransactionType::BUY};
        std::string customer_id;
        std::string recipient_id;
        std::string account_id;
        double amount{0.0};
        std::string currency{"USD"};
        Timestamp timestamp{std::chrono::system_clock::now()};
        Location location;
        std::string ip_address;
        Device device;
        std::string merchant_id;
        PaymentMethod payment_method;
        TransactionStatus status{TransactionStatus::PENDING};
        Timestamp created_at{std::chrono::system_clock::now()};
        Timestamp updated_at{std::chrono::system_clock::now()};

    public:
        Transaction(){};
        Transaction(const std::string&, TransactionType, const std::string&, const std::string&, const std::string&, double, 
            const std::string&, Timestamp, const Location&, const std::string&, const Device&, const std::string&, const PaymentMethod&);

        // Setters / State updates
        void SetStatus(TransactionStatus);
        void SetAmount(double);
        void SetCurrency(const std::string&);
        void SetLocation(const Location&);
        void SetDevice(const Device&);

        // Getters
        const std::string& GetTransaction_ID() const;
        TransactionType GetType() const;
        const std::string& GetCustomer_ID() const;
        const std::string& GetRecipient_ID() const;
        const std::string& GetAccount_ID() const;
        double GetAmount() const;
        const std::string& GetCurrency() const;
        Timestamp GetTimestamp() const;
        const Location& GetLocation() const;
        const std::string& GetIP_Address() const;
        const Device& GetDevice() const;
        const std::string& GetMerchant_ID() const;
        const PaymentMethod& GetPayment_Method() const;
        TransactionStatus GetStatus() const;
        Timestamp GetCreated_At() const;
        Timestamp GetUpdated_At() const;

        // Lifecycle state transitions
        bool approve();      
        bool sendToReview(); 
        bool challenge(); 
        bool reject();
        bool settle(); 
        bool chargeback();
        bool dispute();  

        // --- Business Checks (Kiểm tra nghiệp vụ) ---
        bool isCrossBorder(const std::string&) const;
        bool isHighValue(double) const;
        std::string getSummary() const;
};

} 