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
        Account(){
            this->account_id = "";
            this->customer_id = "";
            this->balance = 0.0;
            this->currency = "USD";
            this->status = AccountStatus::ACTIVE;
            this->created_at = std::chrono::system_clock::now();
        }

        // Constructor có tham số
        Account(const std::string& account_id,
                const std::string& customer_id,
                double balance = 0.0,
                const std::string& currency = "USD",
                Timestamp created_at = std::chrono::system_clock::now()){

            this->account_id = account_id;
            this->customer_id = customer_id;
            this->balance = balance >= 0.0 ? balance : 0.0; // Sử dụng toán tử 3 ngôi giúp đảm bảo số dư không âm
            this->currency = currency;
            this->status = AccountStatus::ACTIVE;
            this->created_at = created_at;
        }

        // Getter
        const std::string& getAccountId() const{ // lấy ID tài khoản
            return account_id;
        }

        const std::string& getCustomerId() const{ // lấy ID khách hàng
            return customer_id;
        }

        double getBalance() const{ // lấy số dư hiện tại
            return balance;
        }

        const std::string& getCurrency() const{ // lấy loại tiền tệ
            return currency;
        }

        AccountStatus getStatus() const{ // lấy trạng thái tài khoản
            return status;
        }

        Timestamp getCreatedAt() const{ // lấy thời gian tạo tài khoản
            return created_at;
        }

        // Setter
        void setCustomerId(const std::string& customer_id){ // cập nhật ID khách hàng
            this->customer_id = customer_id;
        }

        void setCurrency(const std::string& currency){ // cập nhật loại tiền tệ
            this->currency = currency;
        }

        // Account operations
        bool hasSufficientBalance(double amount) const{ // kiểm tra số dư có đủ không
            return amount >= 0.0 && balance >= amount;
        }

        void deposit(double amount){ // nạp tiền vào tài khoản
            if(status != AccountStatus::ACTIVE){ // chỉ nạp khi tài khoản đang hoạt động
                return;
            }

            if(amount <= 0.0){ // số tiền nạp phải > 0
                return;
            }

            balance += amount; // cộng tiền vào tài khoản
        }

        bool withdraw(double amount){ // rút tiền từ tài khoản
            if(status != AccountStatus::ACTIVE){ // từ chối rút nếu tài khoản không hoạt động
                return false;
            }

            if(amount <= 0.0 || amount > balance){ // từ chối nếu tiền rút <= 0 hoặc vượt quá số dư
                return false;
            }

            balance -= amount; // trừ tiền khỏi tài khoản
            return true;       // rút tiền thành công
        }

        // Account status
        void freeze(){ // đóng băng tài khoản
            status = AccountStatus::FROZEN;
        }

        void unfreeze(){ // mở đóng băng
            if(status == AccountStatus::FROZEN){ // chỉ mở lại khi đang bị đóng băng
                status = AccountStatus::ACTIVE;
            }
        }

        void close(){ // đóng tài khoản vĩnh viễn
            status = AccountStatus::CLOSED;
        }

        bool isActive() const{ // kiểm tra tài khoản có đang hoạt động không
            return status == AccountStatus::ACTIVE;
        }

        bool isFrozen() const{ // kiểm tra tài khoản có bị đóng băng không
            return status == AccountStatus::FROZEN;
        }

        bool isClosed() const{ // kiểm tra tài khoản đã đóng chưa
            return status == AccountStatus::CLOSED;
        }

        // String representation
        std::string toString() const{// Mục đích dòng này là: Trả về thông tin tài khoản dưới dạng string
            std::ostringstream oss; // Output string stream

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

        std::string summary() const{//Mục đích dòng này là: Trả về thông tin tài khoản dưới dạng string
            return toString();
        }
};

inline std::ostream& operator<<(std::ostream& os,const Account& account){// toán tử xuất
    os << account.toString();
    return os;
}

namespace epfd{
    using Account = ::Account;
    using AccountStatus = ::AccountStatus;
}
