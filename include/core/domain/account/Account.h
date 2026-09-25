#pragma once
#include <string>
#include <chrono>
#include <sstream>
#include <ostream>

using Timestamp = std::chrono::system_clock::time_point; // kiểu thời gian

enum class AccountStatus{// trạng thái tài khoản
    ACTIVE,// hoạt động
    FROZEN,// đóng băng
    CLOSED// đã đóng
};

class Account{
    private:
        std::string account_id;// ID tài khoản
        std::string customer_id;// ID khách hàng
        double balance{0.0}; // số dư tài khoản
        std::string currency{"USD"};// loại tiền tệ
        AccountStatus status{AccountStatus::ACTIVE};// trạng thái tài khoản
        Timestamp created_at{std::chrono::system_clock::now()}; // thời gian tạo tài khoản

    public:
        // Constructor mặc định
        Account(){};

        // Constructor có tham số
        Account(const std::string&, const std::string&, double, const std::string&, Timestamp created_at = std::chrono::system_clock::now());

        // Getter
        const std::string& getAccountId() const;
        const std::string& getCustomerId() const;
        double getBalance() const;
        const std::string& getCurrency() const;
        AccountStatus getStatus() const;
        Timestamp getCreatedAt() const;

        // Setter
        void setCustomerId(const std::string&);
        void setCurrency(const std::string&);
        // Account operations
        bool hasSufficientBalance(double) const;;
        void deposit(double);
        bool withdraw(double);
        // Account status
        void freeze();
        void unfreeze();
        void close();
        
        bool isActive() const;
        bool isFrozen() const;
        bool isClosed() const;
        // String representation
        std::string toString() const;
        std::string summary() const;
};

inline std::ostream& operator<<(std::ostream& os,const Account& account);
