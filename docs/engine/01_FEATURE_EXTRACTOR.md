# 📐 Bước 1: Trích Xuất Đặc Trưng (Feature Engineering)

> **Mục tiêu**: Xây dựng module biến đổi dữ liệu thô của giao dịch, thông tin khách hàng và lịch sử gần đây thành vector đặc trưng số học chuẩn hóa (`FeatureVector`), phục vụ cho cả **Rule Engine** và **ML Predictor**.  
> **Thư mục làm việc**: `include/engine/features/` và `sources/engine/features/`.

---

## 🧩 1. Thiết Kế `FeatureVector` (`FeatureVector.h`)

`FeatureVector` là cấu trúc dữ liệu nhẹ chứa toàn bộ các chỉ số đã được trích xuất.

### File: `include/engine/features/FeatureVector.h`
```cpp
#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include "core/common/Types.h"

struct FeatureVector {
    // --- 1. Đặc trưng về giá trị (Amount Features) ---
    double amount{0.0};                     // Số tiền giao dịch hiện tại
    double avg_amount_historical{0.0};      // Số tiền trung bình trong lịch sử khách hàng
    double amount_ratio_to_avg{1.0};        // Tỷ số: amount / avg_amount_historical (đo độ đột biến)

    // --- 2. Đặc trưng về tần suất / Cửa sổ trượt (Velocity Features) ---
    int tx_count_last_5min{0};              // Số giao dịch trong 5 phút gần nhất
    int tx_count_last_1hour{0};             // Số giao dịch trong 1 giờ gần nhất
    double total_spent_last_1hour{0.0};     // Tổng tiền đã tiêu trong 1 giờ qua

    // --- 3. Đặc trưng về thiết bị & môi trường (Device Features) ---
    bool is_new_device{false};              // Thiết bị chưa từng nằm trong danh sách tin cậy
    bool is_device_virtual{false};          // Môi trường giả lập / Emulator
    bool is_device_compromised{false};      // Thiết bị đã root / jailbreak / có dấu hiệu hacker

    // --- 4. Đặc trưng về không gian & thời gian (Geo & Temporal Features) ---
    bool is_foreign_country{false};         // Khác quốc gia đăng ký của khách hàng
    double distance_from_last_tx_km{0.0};   // Khoảng cách từ giao dịch gần nhất (km)
    double speed_from_last_tx_kmh{0.0};     // Vận tốc di chuyển tương đối (km/h)
    bool is_unusual_hour{false};            // Giao dịch vào khung giờ nhạy cảm (0h - 5h sáng)

    // --- 5. Đặc trưng từ Merchant & Customer Profile ---
    double merchant_risk_score{10.0};       // Điểm rủi ro cơ sở của Merchant (0 - 100)
    bool is_high_risk_merchant_mcc{false};  // MCC thuộc loại rủi ro cao (Gambling, Crypto, ...)
    CustomerRiskTier customer_tier{CustomerRiskTier::NEW}; // Phân tầng rủi ro KYC của khách

    // Hàm chuyển đổi thành chuỗi phục vụ debug/ghi log
    std::string toString() const {
        std::ostringstream oss;
        oss << "[FeatureVector] Amt: " << amount 
            << " | RatioAvg: " << std::fixed << std::setprecision(2) << amount_ratio_to_avg
            << " | Velo5m: " << tx_count_last_5min
            << " | Velo1h: " << tx_count_last_1hour
            << " | NewDev: " << (is_new_device ? "YES" : "NO")
            << " | Foreign: " << (is_foreign_country ? "YES" : "NO")
            << " | Speed: " << speed_from_last_tx_kmh << " km/h";
        return oss.str();
    }
};
```

---

## ⚙️ 2. Thiết Kế `FeatureExtractor` (`FeatureExtractor.h` & `.cpp`)

`FeatureExtractor` nhận vào giao dịch hiện tại, thông tin khách hàng, thông tin Merchant, và danh sách các giao dịch gần đây (được lưu trong `Vector<Transaction>` hoặc `Deque<Transaction>`), sau đó tính toán để trả về một `FeatureVector`.

### File: `include/engine/features/FeatureExtractor.h`
```cpp
#pragma once
#include "FeatureVector.h"
#include "core/domain/transaction/Transaction.h"
#include "core/domain/account/Customer.h"
#include "core/domain/merchant/Merchant.h"
#include "DSA/vector.h"
#include "DSA/Deque.h"

class FeatureExtractor {
public:
    FeatureExtractor() = default;

    /**
     * @brief Trích xuất toàn bộ vector đặc trưng từ thông tin giao dịch và ngữ cảnh.
     * @param current_tx Giao dịch hiện tại đang xét.
     * @param customer Hồ sơ khách hàng thực hiện giao dịch.
     * @param merchant Thông tin đơn vị chấp nhận thanh toán.
     * @param recent_history Danh sách các giao dịch trước đó của khách hàng (sắp xếp theo thời gian).
     * @return FeatureVector chứa các đặc trưng đã tính toán.
     */
    FeatureVector extract(
        const Transaction& current_tx,
        const Customer& customer,
        const Merchant& merchant,
        const Vector<Transaction>& recent_history
    ) const;

private:
    // Helper tính khoảng cách và vận tốc
    void calculateGeoFeatures(
        const Transaction& current_tx,
        const Vector<Transaction>& history,
        FeatureVector& out_features
    ) const;

    // Helper tính tần suất (Velocity) trong 5 phút và 1 giờ
    void calculateVelocityFeatures(
        const Transaction& current_tx,
        const Vector<Transaction>& history,
        FeatureVector& out_features
    ) const;

    // Helper kiểm tra giờ giao dịch nhạy cảm (0h - 5h sáng)
    bool checkUnusualHour(Timestamp ts) const;
};
```

---

## 📝 3. Hướng Dẫn Cài Đặt Chi Tiết `FeatureExtractor.cpp`

### File: `sources/engine/features/FeatureExtractor.cpp`
Các bước xử lý cụ thể trong hàm `extract`:

```cpp
#include "engine/features/FeatureExtractor.h"
#include <ctime>
#include <chrono>

FeatureVector FeatureExtractor::extract(
    const Transaction& current_tx,
    const Customer& customer,
    const Merchant& merchant,
    const Vector<Transaction>& recent_history
) const {
    FeatureVector fv;

    // 1. Amount & Ratio
    fv.amount = current_tx.GetAmount();
    double sum_hist = 0.0;
    int valid_hist_count = 0;
    for (int i = 0; i < recent_history.size(); ++i) {
        sum_hist += recent_history[i].GetAmount();
        valid_hist_count++;
    }
    fv.avg_amount_historical = (valid_hist_count > 0) ? (sum_hist / valid_hist_count) : fv.amount;
    fv.amount_ratio_to_avg = (fv.avg_amount_historical > 0.0) 
                             ? (fv.amount / fv.avg_amount_historical) 
                             : 1.0;

    // 2. Velocity
    calculateVelocityFeatures(current_tx, recent_history, fv);

    // 3. Device
    fv.is_new_device = !customer.isDeviceTrusted(current_tx.GetDevice().GetDevice_ID());
    fv.is_device_virtual = current_tx.GetDevice().GetIs_Virtual();
    fv.is_device_compromised = current_tx.GetDevice().GetIs_Hacker();

    // 4. Location & Travel
    fv.is_foreign_country = (current_tx.GetLocation().getCountry() != customer.getHomeLocation().getCountry());
    calculateGeoFeatures(current_tx, recent_history, fv);
    fv.is_unusual_hour = checkUnusualHour(current_tx.GetTimestamp());

    // 5. Merchant & Customer Risk
    fv.merchant_risk_score = merchant.getBaselineRiskScore();
    fv.is_high_risk_merchant_mcc = (merchant.getCategory() == MerchantCategory::GAMBLING || 
                                    merchant.getCategory() == MerchantCategory::CRYPTO);
    fv.customer_tier = customer.getRiskTier();

    return fv;
}

void FeatureExtractor::calculateVelocityFeatures(
    const Transaction& current_tx,
    const Vector<Transaction>& history,
    FeatureVector& fv
) const {
    fv.tx_count_last_5min = 0;
    fv.tx_count_last_1hour = 0;
    fv.total_spent_last_1hour = 0.0;

    auto current_time = current_tx.GetTimestamp();

    for (int i = 0; i < history.size(); ++i) {
        auto diff_seconds = std::chrono::duration_cast<std::chrono::seconds>(
            current_time - history[i].GetTimestamp()
        ).count();

        // Chỉ xét các giao dịch quá khứ hợp lệ
        if (diff_seconds >= 0 && diff_seconds <= 300) { // 5 phút = 300s
            fv.tx_count_last_5min++;
        }
        if (diff_seconds >= 0 && diff_seconds <= 3600) { // 1 giờ = 3600s
            fv.tx_count_last_1hour++;
            fv.total_spent_last_1hour += history[i].GetAmount();
        }
    }
}

void FeatureExtractor::calculateGeoFeatures(
    const Transaction& current_tx,
    const Vector<Transaction>& history,
    FeatureVector& fv
) const {
    if (history.empty()) {
        fv.distance_from_last_tx_km = 0.0;
        fv.speed_from_last_tx_kmh = 0.0;
        return;
    }

    // Lấy giao dịch gần nhất (phần tử cuối của history)
    const Transaction& last_tx = history[history.size() - 1];
    fv.distance_from_last_tx_km = current_tx.GetLocation().distanceTo(last_tx.GetLocation());

    auto diff_seconds = std::chrono::duration_cast<std::chrono::seconds>(
        current_tx.GetTimestamp() - last_tx.GetTimestamp()
    ).count();

    if (diff_seconds > 0) {
        double hours = static_cast<double>(diff_seconds) / 3600.0;
        fv.speed_from_last_tx_kmh = fv.distance_from_last_tx_km / hours;
    } else {
        // Cùng thời điểm nhưng khoảng cách > 0 -> Tốc độ vô cực
        fv.speed_from_last_tx_kmh = (fv.distance_from_last_tx_km > 0.0) ? 99999.0 : 0.0;
    }
}

bool FeatureExtractor::checkUnusualHour(Timestamp ts) const {
    std::time_t tt = std::chrono::system_clock::to_time_t(ts);
    std::tm* local = std::localtime(&tt);
    if (!local) return false;
    return (local->tm_hour >= 0 && local->tm_hour < 5); // 00:00 -> 04:59
}
```

---

## 🎯 4. Tiêu Chuẩn Nghiệm Thu Bước 1
- [ ] Header `FeatureVector.h` được đặt tại `include/engine/features/`.
- [ ] Header `FeatureExtractor.h` và file `FeatureExtractor.cpp` biên dịch không có cảnh báo.
- [ ] Viết test nhỏ truyền 2 giao dịch cách nhau $2000\text{ km}$ trong $10\text{ phút}$, kiểm tra `speed_from_last_tx_kmh` tính ra $> 1000\text{ km/h}$.

---
👉 **Tiếp tục với Bước 2: [02_RULE_BASED_ENGINE.md](file:///d:/Fraud-Detection---Risk-management/docs/engine/02_RULE_BASED_ENGINE.md)**
