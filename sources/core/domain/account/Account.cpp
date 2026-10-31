#include "core/domain/account/Account.h"
#include <sstream>

// Constructor mặc định
Account::Account()
    : account_id(""),
      customer_id(""),
      balance(0.0),
      currency("USD"),
      status(AccountStatus::ACTIVE),
      created_at(std::chrono::system_clock::now()) {
}

// Constructor có tham số
Account::Account(const std::string& account_id,
                 const std::string& customer_id,
                 double balance,
                 const std::string& currency,
                 Timestamp created_at)
    : account_id(account_id),
      customer_id(customer_id),
      balance(balance >= 0.0 ? balance : 0.0),
      currency(currency),
      status(AccountStatus::ACTIVE),
      created_at(created_at) {
}


// Getter
const std::string& Account::getAccountId() const {
    return this->account_id;
}

const std::string& Account::getCustomerId() const {
    return this->customer_id;
}

double Account::getBalance() const {
    return this->balance;
}

const std::string& Account::getCurrency() const {
    return this->currency;
}

AccountStatus Account::getStatus() const {
    return this->status;
}

Timestamp Account::getCreatedAt() const {
    return this->created_at;
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
    return amount >= 0.0 && this->balance >= amount;
}

void Account::deposit(double amount) {
    if (this->status != AccountStatus::ACTIVE) {
        return;
    }

    if (amount <= 0.0) {
        return;
    }

    this->balance += amount;
}

bool Account::withdraw(double amount) {
    if (this->status != AccountStatus::ACTIVE) {
        return false;
    }

    if (amount <= 0.0 || amount > this->balance) {
        return false;
    }

    this->balance -= amount;
    return true;
}

// Account status
void Account::freeze() {
    this->status = AccountStatus::FROZEN;
}

void Account::unfreeze() {
    if (this->status == AccountStatus::FROZEN) {
        this->status = AccountStatus::ACTIVE;
    }
}

void Account::close() {
    this->status = AccountStatus::CLOSED;
}

bool Account::isActive() const {
    return this->status == AccountStatus::ACTIVE;
}

bool Account::isFrozen() const {
    return this->status == AccountStatus::FROZEN;
}

bool Account::isClosed() const {
    return this->status == AccountStatus::CLOSED;
}

// String representation
std::string Account::toString() const {
    std::ostringstream oss;

    oss << "Account{"
        << "account_id='" << this->account_id << '\''
        << ", customer_id='" << this->customer_id << '\''
        << ", balance=" << this->balance
        << ", currency='" << this->currency << '\''
        << ", status="
        << (this->status == AccountStatus::ACTIVE ? "ACTIVE" :
           (this->status == AccountStatus::FROZEN ? "FROZEN" : "CLOSED"))
        << '}';

    return oss.str();
}