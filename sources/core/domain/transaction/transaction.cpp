#include "core/domain/transaction/Transaction.h"
#include <stdexcept>

Transaction::Transaction() {}

Transaction::Transaction(const std::string& transaction_id,
                         TransactionType type,
                         const std::string& customer_id,
                         const std::string& recipient_id,
                         const std::string& account_id,
                         double amount,
                         const std::string& currency,
                         Timestamp timestamp,
                         const Location& location,
                         const std::string& ip_address,
                         const Device& device,
                         const std::string& merchant_id,
                         const PaymentType& payment_method)
    : transaction_id(transaction_id),
      type(type),
      customer_id(customer_id),
      recipient_id(recipient_id),
      account_id(account_id),
      currency(currency),
      timestamp(timestamp),
      location(location),
      ip_address(ip_address),
      device(device),
      merchant_id(merchant_id),
      payment_method(payment_method),
      status(TransactionStatus::PENDING),
      created_at(std::chrono::system_clock::now()),
      updated_at(std::chrono::system_clock::now()) {
    SetAmount(amount);
}

// ==================== Setters / State updates ====================

void Transaction::SetStatus(TransactionStatus status) {
    this->status = status;
    this->updated_at = std::chrono::system_clock::now();
}

void Transaction::SetAmount(double amount) {
    if (amount <= 0.0) {
        throw std::invalid_argument("Transaction amount must be strictly positive");
    }
    this->amount = amount;
    this->updated_at = std::chrono::system_clock::now();
}

void Transaction::SetCurrency(const std::string& currency) {
    this->currency = currency;
    this->updated_at = std::chrono::system_clock::now();
}

void Transaction::SetLocation(const Location& location) {
    this->location = location;
    this->updated_at = std::chrono::system_clock::now();
}

void Transaction::SetDevice(const Device& device) {
    this->device = device;
    this->updated_at = std::chrono::system_clock::now();
}

// ==================== Getters ====================

const std::string& Transaction::GetTransaction_ID() const { return transaction_id; }
TransactionType Transaction::GetType() const { return type; }
const std::string& Transaction::GetCustomer_ID() const { return customer_id; }
const std::string& Transaction::GetRecipient_ID() const { return recipient_id; }
const std::string& Transaction::GetAccount_ID() const { return account_id; }
double Transaction::GetAmount() const { return amount; }
const std::string& Transaction::GetCurrency() const { return currency; }
Timestamp Transaction::GetTimestamp() const { return timestamp; }
const Location& Transaction::GetLocation() const { return location; }
const std::string& Transaction::GetIP_Address() const { return ip_address; }
const Device& Transaction::GetDevice() const { return device; }
const std::string& Transaction::GetMerchant_ID() const { return merchant_id; }
const PaymentType& Transaction::GetPayment_Method() const { return payment_method; }
TransactionStatus Transaction::GetStatus() const { return status; }
Timestamp Transaction::GetCreated_At() const { return created_at; }
Timestamp Transaction::GetUpdated_At() const { return updated_at; }

// ==================== Lifecycle state transitions ====================

bool Transaction::approve() {
    if (status == TransactionStatus::PENDING || status == TransactionStatus::WAIT_REVIEW || status == TransactionStatus::VERIFY_OTP) {
        SetStatus(TransactionStatus::APPROVED);
        return true;
    }
    return false;
}

bool Transaction::sendToReview() {
    if (status == TransactionStatus::PENDING || status == TransactionStatus::VERIFY_OTP) {
        SetStatus(TransactionStatus::WAIT_REVIEW);
        return true;
    }
    return false;
}

bool Transaction::challenge() {
    if (status == TransactionStatus::PENDING) {
        SetStatus(TransactionStatus::VERIFY_OTP);
        return true;
    }
    return false;
}

bool Transaction::reject() {
    if (status == TransactionStatus::PENDING || status == TransactionStatus::WAIT_REVIEW || status == TransactionStatus::VERIFY_OTP) {
        SetStatus(TransactionStatus::BLOCKED);
        return true;
    }
    return false;
}

bool Transaction::settle() {
    if (status == TransactionStatus::APPROVED) {
        SetStatus(TransactionStatus::SUCCESS);
        return true;
    }
    return false;
}

bool Transaction::chargeback() {
    if (status == TransactionStatus::SUCCESS || status == TransactionStatus::APPROVED) {
        SetStatus(TransactionStatus::REFUNDED_FRAUD);
        return true;
    }
    return false;
}

bool Transaction::dispute() {
    if (status == TransactionStatus::SUCCESS || status == TransactionStatus::APPROVED) {
        SetStatus(TransactionStatus::DISPUTED);
        return true;
    }
    return false;
}

// ==================== Business Checks ====================

bool Transaction::isCrossBorder(const std::string& home_country) const {
    if (home_country.empty() || location.getCountry().empty()) return false;
    return location.getCountry() != home_country;
}

bool Transaction::isHighValue(double threshold) const {
    return amount >= threshold;
}

std::string Transaction::getSummary() const {
    return "Transaction[ID: " + transaction_id
        + ", Type: " + std::string(toString(type))
        + ", Customer ID: " + customer_id
        + ", Recipient ID: " + recipient_id
        + ", Amount: " + std::to_string(amount) + " " + currency
        + ", Status: " + std::string(toString(status))
        + ", Merchant ID: " + merchant_id
        + "]";
}