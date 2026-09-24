#pragma once
#include <string>
#include <chrono>
#include <sstream>
#include <ostream>

using Timestamp = std::chrono::system_clock::time_point;

enum class AccountStatus{
    ACTIVE,
    FROZEN,
    CLOSED
};

class Account{
    private:
        std::string account_id;
        std::string customer_id;
        double balance{0.0};
        std::string currency{"USD"};
        AccountStatus status{AccountStatus::ACTIVE};
        Timestamp created_at{std::chrono::system_clock::now()};

    public:
        Account(){
            this->account_id = "";
            this->customer_id = "";
            this->balance = 0.0;
            this->currency = "USD";
            this->status = AccountStatus::ACTIVE;
            this->created_at = std::chrono::system_clock::now();
        }

        Account(const std::string& account_id,
                const std::string& customer_id,
                double balance = 0.0,
                const std::string& currency = "USD",
                Timestamp created_at = std::chrono::system_clock::now()){

            this->account_id = account_id;
            this->customer_id = customer_id;
            this->balance = balance >= 0.0 ? balance : 0.0;
            this->currency = currency;
            this->status = AccountStatus::ACTIVE;
            this->created_at = created_at;
        }

        // Getter
        const std::string& getAccountId() const{
            return account_id;
        }

        const std::string& getCustomerId() const{
            return customer_id;
        }

        double getBalance() const{
            return balance;
        }

        const std::string& getCurrency() const{
            return currency;
        }

        AccountStatus getStatus() const{
            return status;
        }

        Timestamp getCreatedAt() const{
            return created_at;
        }

        // Setter
        void setCustomerId(const std::string& customer_id){
            this->customer_id = customer_id;
        }

        void setCurrency(const std::string& currency){
            this->currency = currency;
        }

        // Account operations
        bool hasSufficientBalance(double amount) const{
            return amount >= 0.0 && balance >= amount;
        }

        void deposit(double amount){
            if(status != AccountStatus::ACTIVE){
                return;
            }

            if(amount <= 0.0){
                return;
            }

            balance += amount;
        }

        bool withdraw(double amount){
            if(status != AccountStatus::ACTIVE){
                return false;
            }

            if(amount <= 0.0 || amount > balance){
                return false;
            }

            balance -= amount;
            return true;
        }

        // Account status
        void freeze(){
            status = AccountStatus::FROZEN;
        }

        void unfreeze(){
            if(status == AccountStatus::FROZEN){
                status = AccountStatus::ACTIVE;
            }
        }

        void close(){
            status = AccountStatus::CLOSED;
        }

        bool isActive() const{
            return status == AccountStatus::ACTIVE;
        }

        bool isFrozen() const{
            return status == AccountStatus::FROZEN;
        }

        bool isClosed() const{
            return status == AccountStatus::CLOSED;
        }

        // String representation
        std::string toString() const{
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

        std::string summary() const{
            return toString();
        }
};

inline std::ostream& operator<<(std::ostream& os,const Account& account){
    os << account.toString();
    return os;
}

namespace epfd{
    using Account = ::Account;
    using AccountStatus = ::AccountStatus;
}
