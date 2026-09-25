<<<<<<< HEAD
#include"core/domain/account/Account.h"

Account::Account(){
=======
#include "Account.h"
#include <sstream>

// Constructor mặc định
Account::Account() {
>>>>>>> main
    this->account_id = "";
    this->customer_id = "";
    this->balance = 0.0;
    this->currency = "USD";
    this->status = AccountStatus::ACTIVE;
    this->created_at = std::chrono::system_clock::now();
}

// Constructor có tham số
<<<<<<< HEAD
Account::Account(const std::string& account_id, const std::string& customer_id, double balance = 0.0, const std::string& currency = "USD", Timestamp created_at = std::chrono::system_clock::now()){
    this->account_id = account_id;
    this->customer_id = customer_id;
    this->balance = balance >= 0.0 ? balance : 0.0; // đảm bảo số dư không âm
=======
Account::Account(const std::string& account_id,
                 const std::string& customer_id,
                 double balance,
                 const std::string& currency,
                 Timestamp created_at) {

    this->account_id = account_id;
    this->customer_id = customer_id;
    this->balance = balance >= 0.0 ? balance : 0.0;
>>>>>>> main
    this->currency = currency;
    this->status = AccountStatus::ACTIVE;
    this->created_at = created_at;
}

// Getter
<<<<<<< HEAD
const std::string& Account::getAccountId() const{ // lấy ID tài khoản
    return account_id;
}

const std::string& Account::getCustomerId() const{ // lấy ID khách hàng
    return customer_id;
}

double Account::getBalance() const{ // lấy số dư hiện tại
    return balance;
}

const std::string& Account::getCurrency() const{ // lấy loại tiền tệ
    return currency;
}

AccountStatus Account::getStatus() const{ // lấy trạng thái tài khoản
    return status;
}

Timestamp Account::getCreatedAt() const{ // lấy thời gian tạo tài khoản
=======
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
>>>>>>> main
    return created_at;
}

// Setter
<<<<<<< HEAD
void Account::setCustomerId(const std::string& customer_id){ // cập nhật ID khách hàng
    this->customer_id = customer_id;
}

void Account::setCurrency(const std::string& currency){ // cập nhật loại tiền tệ
=======
void Account::setCustomerId(const std::string& customer_id) {
    this->customer_id = customer_id;
}

void Account::setCurrency(const std::string& currency) {
>>>>>>> main
    this->currency = currency;
}

// Account operations
<<<<<<< HEAD
bool Account::hasSufficientBalance(double amount) const{ // kiểm tra số dư có đủ không
    return amount >= 0.0 && balance >= amount;
}

void Account::deposit(double amount){ // nạp tiền vào tài khoản
    if(status != AccountStatus::ACTIVE){ // chỉ nạp khi tài khoản đang hoạt động
        return;
    }

    if(amount <= 0.0){ // số tiền nạp phải > 0
        return;
    }

    balance += amount; // cộng tiền vào tài khoản
}

bool Account::withdraw(double amount){ // rút tiền từ tài khoản
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
void Account::freeze(){ // đóng băng tài khoản
    status = AccountStatus::FROZEN;
}

void Account::unfreeze(){ // mở đóng băng
    if(status == AccountStatus::FROZEN){ // chỉ mở lại khi đang bị đóng băng
=======
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
>>>>>>> main
        status = AccountStatus::ACTIVE;
    }
}

<<<<<<< HEAD
void Account::close(){ // đóng tài khoản vĩnh viễn
    status = AccountStatus::CLOSED;
}

bool Account::isActive() const{ // kiểm tra tài khoản có đang hoạt động không
    return status == AccountStatus::ACTIVE;
}

bool Account::isFrozen() const{ // kiểm tra tài khoản có bị đóng băng không
    return status == AccountStatus::FROZEN;
}

bool Account::isClosed() const{ // kiểm tra tài khoản đã đóng chưa
=======
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
>>>>>>> main
    return status == AccountStatus::CLOSED;
}

// String representation
<<<<<<< HEAD
std::string Account::toString() const{// Mục đích dòng này là: Trả về thông tin tài khoản dưới dạng string
    std::ostringstream oss; // Output string stream
=======
std::string Account::toString() const {
    std::ostringstream oss;
>>>>>>> main

    oss << "Account{"
        << "account_id='" << account_id << '\''
        << ", customer_id='" << customer_id << '\''
        << ", balance=" << balance
        << ", currency='" << currency << '\''
        << ", status="
        << (status == AccountStatus::ACTIVE ? "ACTIVE" :
<<<<<<< HEAD
        (status == AccountStatus::FROZEN ? "FROZEN" : "CLOSED"))
=======
           (status == AccountStatus::FROZEN ? "FROZEN" : "CLOSED"))
>>>>>>> main
        << '}';

    return oss.str();
}

<<<<<<< HEAD
std::string Account::summary() const{//Mục đích dòng này là: Trả về thông tin tài khoản dưới dạng string
    return toString();
}


inline std::ostream& operator<<(std::ostream& os,const Account& account){// toán tử xuất
    os << account.toString();
    return os;
}

namespace epfd{
    using Account = ::Account;
    using AccountStatus = ::AccountStatus;
}
=======
std::string Account::summary() const {
    return toString();
}

// Operator <<
std::ostream& operator<<(std::ostream& os, const Account& account) {
    os << account.toString();
    return os;
}
>>>>>>> main
