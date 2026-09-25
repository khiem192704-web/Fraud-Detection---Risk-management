#include "Account.h"
#include <sstream>

// Constructor mặc định
Account::Account() {
    this->account_id = "";
    this->customer_id = "";
    this->balance = 0.0;
    this->currency = "USD";
    this->status = AccountStatus::ACTIVE;
    this->created_at = std::chrono::system_clock::now();
}

// Constructor có tham số
Account::Account(const std::string& account_id,
                 const std::string& customer_id,
                 double balance,
                 const std::string& currency,
                 Timestamp created_at) {

    this->account_id = account_id;
    this->customer_id = customer_id;
    this->balance = balance >= 0.0 ? balance : 0.0;
    this->currency = currency;
    this->status = AccountStatus::ACTIVE;
    this->created_at = created_at;
}

// Getter
const std::string& Account::getAccountId() const {
    return account_id;
}

const std::string& Account::getCustomerId() const {
    return customer_id;
}

double Account::getBalance() const {
    return balance;
}

const std::string& Account::getCurrency() const {
    return currency;
}

AccountStatus Account::getStatus() const {
    return status;
}

Timestamp Account::getCreatedAt() const {
    return created_at;
}

// Setter
void Account::setCustomerId(const std::string& customer_id) {
    this->customer_id = customer_id;
}

void Account::setCurrency(const std::string& currency) {
    this->currency = currency;
}

// Account operations
bool Account::hasSufficientBalance(double amount) const {
    return amount >= 0.0 && balance >= amount;
}

void Account::deposit(double amount) {
    if (status != AccountStatus::ACTIVE) {
        return;
    }

    if (amount <= 0.0) {
        return;
    }

    balance += amount;
}

bool Account::withdraw(double amount) {
    if (status != AccountStatus::ACTIVE) {
        return false;
    }

    if (amount <= 0.0 || amount > balance) {
        return false;
    }

    balance -= amount;
    return true;
}

// Account status
void Account::freeze() {
    status = AccountStatus::FROZEN;
}

void Account::unfreeze() {
    if (status == AccountStatus::FROZEN) {
        status = AccountStatus::ACTIVE;
    }
}

void Account::close() {
    status = AccountStatus::CLOSED;
}

bool Account::isActive() const {
    return status == AccountStatus::ACTIVE;
}

bool Account::isFrozen() const {
    return status == AccountStatus::FROZEN;
}

bool Account::isClosed() const {
    return status == AccountStatus::CLOSED;
}

// String representation
std::string Account::toString() const {
    std::ostringstream oss;

    oss << "Account{"
        << "account_id='" << account_id << '\''
        << ", customer_id='" << customer_id << '\''
        << ", balance=" << balance
        << ", currency='" << currency << '\''
        << ", status="
        << (status == AccountStatus::ACTIVE ? "ACTIVE" :
           (status == AccountStatus::FROZEN ? "FROZEN" : "CLOSED"))
        << '}';

    return oss.str();
}

std::string Account::summary() const {
    return toString();
}

// Operator <<
std::ostream& operator<<(std::ostream& os, const Account& account) {
    os << account.toString();
    return os;
}