#pragma once
#include<string_view>
#include<chrono>
#include <ostream>

enum class PaymentType{
    CARD,       // Thẻ ngân hàng 
    E_WALLET,   // Ví điện tử
    BANK_TRANSFER, // Chuyển khoản ngân hàng
    CASH,       // Tiền mặt
    CRYPTO // Tiền điện tử
};
using Timestamp = std::chrono::system_clock::time_point;

std::string_view toString(PaymentType type);
std::ostream& operator<<(std::ostream& os, PaymentType type);
