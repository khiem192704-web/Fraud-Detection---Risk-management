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

using Timestamp = std::chrono::system_clock::time_point;

std::string_view toString(TransactionType type);
std::string_view toString(TransactionStatus status);

std::ostream& operator<<(std::ostream& os, TransactionType type);
std::ostream& operator<<(std::ostream& os, TransactionStatus status);