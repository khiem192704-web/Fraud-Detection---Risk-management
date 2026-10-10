#pragma once
#include<string_view>
#include<chrono>
#include <ostream>

enum class TransactionType { // Loại giao dịch
    BUY,        // Mua sắm (quẹt thẻ POS, mua hàng qua web/app)
    TRANSFER,   // Chuyển tiền 
    WITHDRAW,   // Rút tiền 
    TOPUP,      // Nạp tiền
    REFUND,     // Hoàn trả
    PAY_BILL    // Thanh toán hóa đơn (điện, nước, internet, ...)
};

enum class TransactionStatus{
    PENDING, // Đang chờ xử lí
    APPROVED, // Đã duyệt 
    WAIT_REVIEW, // Nghi vấn 
    VERIFY_OTP, // Nghi vấn nhẹ 
    BLOCKED, // Bị chặn 
    FAILED,  // Thất bại 
    SUCCESS,  // Thành công 
    DISPUTED, // Bị khiếu nại
    REFUNDED_FRAUD  // Bồi hoàn gian lận
};

enum class PaymentType{
    CARD,       // Thẻ ngân hàng 
    E_WALLET,   // Ví điện tử
    BANK_TRANSFER, // Chuyển khoản ngân hàng
    CASH,       // Tiền mặt
    CRYPTO // Tiền điện tử
};

enum class MerchantCategory {
    RETAIL,
    GROCERY,
    ELECTRONICS,
    RESTAURANT,
    TRAVEL,
    ENTERTAINMENT,
    GAMBLING,
    CRYPTO,
    FINANCIAL_SERVICES,
    OTHER
};

enum class MerchantRiskTier {
    TRUSTED,
    STANDARD,
    ELEVATED,
    SUSPICIOUS,
    BLACKLISTED
};

enum class MerchantStatus {
    ACTIVE,
    SUSPENDED
};

enum class FraudRuleCategory {
    VELOCITY,
    AMOUNT_DEVIATION,
    GEO_LOCATION,
    DEVICE_INTEGRITY,
    CARD_TESTING,
    ACCOUNT_TAKEOVER,
    LIST_MATCHING,
    BEHAVIORAL
};
enum class RiskLevel {
    VERY_LOW,   // [0, 20)
    LOW,        // [20, 40)
    MEDIUM,     // [40, 60)
    HIGH,       // [60, 80)
    CRITICAL    // [80, 100]
};
enum class DecisionAction {
    APPROVE,
    REVIEW,
    CHALLENGE_3DS,
    BLOCK
};

enum class CustomerRiskTier {
    NEW,
    ESTABLISHED,
    TRUSTED,
    SUSPICIOUS,
    BANNED
};

enum class AccountStatus {
    ACTIVE,
    FROZEN,
    CLOSED
};

using Timestamp = std::chrono::system_clock::time_point;

std::string_view toString(PaymentType type);
std::ostream& operator<<(std::ostream& os, PaymentType type);


std::string_view toString(TransactionType type);
std::ostream& operator<<(std::ostream& os, TransactionType type);

std::string_view toString(TransactionStatus status);
std::ostream& operator<<(std::ostream& os, TransactionStatus status);

std::string_view toString(RiskLevel level);
std::ostream& operator<<(std::ostream& os, RiskLevel level);

std::string_view toString(DecisionAction action);
std::ostream& operator<<(std::ostream& os, DecisionAction action);

std::string_view toString(CustomerRiskTier tier);
std::ostream& operator<<(std::ostream& os, CustomerRiskTier tier);

std::string_view toString(FraudRuleCategory category);
std::ostream& operator<<(std::ostream& os, FraudRuleCategory category);

std::string toString(MerchantCategory category);
std::string toString(MerchantRiskTier tier);
std::string toString(MerchantStatus status);