#pragma once

#include <string>
#include <chrono>
#include <ostream>

using Timestamp = std::chrono::system_clock::time_point;

enum class AccountStatus {
    ACTIVE,
    FROZEN,
    CLOSED
};

class Account {
private:
    std::string account_id;
    std::string customer_id;
    double balance{0.0};
    std::string currency{"USD"};
    AccountStatus status{AccountStatus::ACTIVE};
    Timestamp created_at{std::chrono::system_clock::now()};

public:
    // Constructor
    Account();

    Account(const std::string& account_id,
            const std::string& customer_id,
            double balance = 0.0,
            const std::string& currency = "USD",
            Timestamp created_at = std::chrono::system_clock::now());

    // Getter
    const std::string& getAccountId() const;
    const std::string& getCustomerId() const;
    double getBalance() const;
    const std::string& getCurrency() const;
    AccountStatus getStatus() const;
    Timestamp getCreatedAt() const;

    // Setter
    void setCustomerId(const std::string& customer_id);
    void setCurrency(const std::string& currency);

    // Account operations
    bool hasSufficientBalance(double amount) const;
    void deposit(double amount);
    bool withdraw(double amount);

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

// Operator
std::ostream& operator<<(std::ostream& os, const Account& account);

namespace epfd {
    using Account = ::Account;
    using AccountStatus = ::AccountStatus;
}