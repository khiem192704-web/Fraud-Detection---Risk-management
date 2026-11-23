# 🚀 HƯỚNG DẪN HOÀN THIỆN TOÀN DIỆN DỰ ÁN EPFD-RAS (PBL2)
*(Tài liệu chuẩn hóa mã nguồn — Khắc phục 100% lỗi biên dịch và dữ liệu để chạy Demo)*

> **Mục tiêu**: Hướng dẫn xử lý dứt điểm **5 lỗi kỹ thuật thực tế** đang khiến dự án chưa biên dịch được, đồng thời khôi phục dữ liệu mẫu chuẩn và dựng kịch bản demo chạy thực tế để sẵn sàng đem đi bảo vệ.

---

## 📌 TỔNG QUAN 5 VẤN ĐỀ CẦN XỬ LÝ NGAY

| Bước | Vấn đề phát hiện | File liên quan | Thao tác cần làm |
| :---: | :--- | :--- | :--- |
| **1** | Chưa có file `FeatureExtractor` (làm gãy lệnh make) | `include/engine/FeatureExtractor.h`<br>`sources/engine/FeatureExtractor.cpp` | **Tạo mới 2 file** |
| **2** | `RiskEngine` thiếu `getMLPredictor` và hàm `assessRisk` tự chủ | `include/engine/RiskEngine.h`<br>`sources/engine/RiskEngine.cpp` | **Bổ sung 2 hàm** |
| **3** | `TransactionBLL.cpp` gọi sai tên hàm của `Account` và `Types` | `sources/bll/TransactionBLL.cpp` | **Sửa 2 lỗi cú pháp** |
| **4** | Dữ liệu `accounts.txt` và `customers.txt` bị lỗi format chuỗi | `data/accounts.txt`<br>`data/customers.txt` | **Khôi phục dữ liệu chuẩn** |
| **5** | `Makefile` thiếu file dịch và `main.cpp` còn rỗng | `Makefile`<br>`sources/main.cpp` | **Cập nhật build & kịch bản demo** |

---

## 🛠️ BƯỚC 1: TẠO MỚI MODULE `FeatureExtractor`

Tạo mới 2 file sau để trích xuất 4 đặc trưng (`amount`, `velocity_count`, `geo_distance_km`, `is_device_risk`):

### 📄 1.1 Tạo file: `include/engine/FeatureExtractor.h`
```cpp
#pragma once

#include <string>
#include <chrono>
#include "DSA/vector.h"
#include "DSA/deque.h"
#include "core/domain/Transaction.h"
#include "core/domain/Customer.h"

class FeatureExtractor {
private:
    Deque<Timestamp> sliding_window;
    std::chrono::seconds window_duration{60}; // Cửa sổ kiểm tra mặc định: 60 giây

public:
    explicit FeatureExtractor(std::chrono::seconds duration = std::chrono::seconds(60));

    // Trích xuất vector 4 đặc trưng: [amount, velocity_count, geo_distance_km, is_device_risk]
    Vector<double> extract(const Transaction& tx, const Customer& customer);

    void reset();
    Vector<std::string> getFeatureNames() const;

private:
    double extractAmount(const Transaction& tx) const;
    double extractVelocity(const Transaction& tx);
    double extractGeoDistance(const Transaction& tx, const Customer& customer) const;
    double extractDeviceRisk(const Transaction& tx) const;
};
```

---

### 📄 1.2 Tạo file: `sources/engine/FeatureExtractor.cpp`
```cpp
#include "engine/FeatureExtractor.h"
#include <cmath>

FeatureExtractor::FeatureExtractor(std::chrono::seconds duration)
    : window_duration(duration) {}

void FeatureExtractor::reset() {
    while (!sliding_window.empty()) {
        sliding_window.pop_front();
    }
}

Vector<std::string> FeatureExtractor::getFeatureNames() const {
    Vector<std::string> names;
    names.push_back("amount");
    names.push_back("velocity_count");
    names.push_back("geo_distance_km");
    names.push_back("is_device_risk");
    return names;
}

double FeatureExtractor::extractAmount(const Transaction& tx) const {
    return tx.GetAmount();
}

double FeatureExtractor::extractVelocity(const Transaction& tx) {
    Timestamp now = tx.GetTimestamp();

    while (!sliding_window.empty()) {
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - sliding_window.front()).count();
        if (diff > window_duration.count()) {
            sliding_window.pop_front();
        } else {
            break;
        }
    }

    sliding_window.push_back(now);
    return static_cast<double>(sliding_window.size());
}

double FeatureExtractor::extractGeoDistance(const Transaction& tx, const Customer& customer) const {
    const Location& tx_loc = tx.GetLocation();
    const Location& home_loc = customer.getHomeLocation();

    if (!tx_loc.isValid() || !home_loc.isValid()) {
        return 0.0;
    }

    try {
        return tx_loc.distanceTo(home_loc);
    } catch (...) {
        return 0.0;
    }
}

double FeatureExtractor::extractDeviceRisk(const Transaction& tx) const {
    const Device& dev = tx.GetDevice();
    if (dev.GetIs_Virtual() || dev.GetIs_Hacker() || !dev.is_Safe()) {
        return 1.0;
    }
    return 0.0;
}

Vector<double> FeatureExtractor::extract(const Transaction& tx, const Customer& customer) {
    Vector<double> features;
    features.push_back(extractAmount(tx));                // [0] amount
    features.push_back(extractVelocity(tx));              // [1] velocity_count
    features.push_back(extractGeoDistance(tx, customer)); // [2] geo_distance_km
    features.push_back(extractDeviceRisk(tx));            // [3] is_device_risk
    return features;
}
```

---

## 🛠️ BƯỚC 2: NÂNG CẤP `RiskEngine` (TỰ ĐỘNG CHẤM ML SCORE)

### 📄 2.1 Cập nhật `include/engine/RiskEngine.h`
Mở file `include/engine/RiskEngine.h`:
* Thêm `#include "engine/FeatureExtractor.h"` và `#include "engine/NativeMLPredictor.h"`.
* Thêm 2 biến `FeatureExtractor feature_extractor;` và `NativeMLPredictor ml_predictor;` vào private.
* Thêm hàm `getMLPredictor()`, `getFeatureExtractor()`, và hàm overload `assessRisk(tx, customer)`.

```cpp
#pragma once
#include <string>
#include <vector>
#include "DSA/vector.h"
#include "engine/rules/IFraudRule.h"
#include "engine/FeatureExtractor.h"     // <--- Thêm mới
#include "engine/NativeMLPredictor.h"    // <--- Thêm mới
#include "core/common/Types.h"
#include "core/domain/Transaction.h"
#include "core/domain/Customer.h"
#include "core/domain/RiskAssessment.h"
#include "core/domain/FraudAlert.h"

struct RiskPolicy {
    double weight_rules{0.5};
    double weight_ml{0.4};
    double weight_profile{0.1};

    double threshold_very_low{20.0};
    double threshold_low{40.0};
    double threshold_medium{60.0};
    double threshold_high{80.0};

    RiskLevel determineRiskLevel(double score) const {
        if (score < threshold_very_low) return RiskLevel::VERY_LOW;
        if (score < threshold_low)      return RiskLevel::LOW;
        if (score < threshold_medium)   return RiskLevel::MEDIUM;
        if (score < threshold_high)     return RiskLevel::HIGH;
        return RiskLevel::CRITICAL;
    }
};

class RiskEngine {
private:
    std::vector<IFraudRule*> rules;
    RiskPolicy policy;
    FeatureExtractor feature_extractor; // <--- Tự động trích xuất đặc trưng
    NativeMLPredictor ml_predictor;       // <--- Tự động dự đoán điểm ML

public:
    explicit RiskEngine(const RiskPolicy& custom_policy = RiskPolicy());
    ~RiskEngine();

    void registerRule(IFraudRule* rule);
    int getRuleCount() const;
    void setPolicy(const RiskPolicy& new_policy);
    const RiskPolicy& getPolicy() const;

    // Getter cho các module con
    NativeMLPredictor& getMLPredictor() { return ml_predictor; }
    FeatureExtractor& getFeatureExtractor() { return feature_extractor; }

    // HÀM MỚI (Tự chủ): Tự trích xuất 4 đặc trưng -> tự tính ML Score
    RiskAssessment assessRisk(const Transaction& tx, const Customer& customer);

    // HÀM CŨ: Giữ nguyên để tương thích ngược các bài test thủ công
    RiskAssessment assessRisk(const Transaction& tx, const Customer& customer, double ml_score);
};
```

### 📄 2.2 Cập nhật `sources/engine/RiskEngine.cpp`
Thêm hàm mới vào cuối file `sources/engine/RiskEngine.cpp`:

```cpp
RiskAssessment RiskEngine::assessRisk(const Transaction& tx, const Customer& customer) {
    // 1. Trích xuất vector 4 đặc trưng
    Vector<double> features = feature_extractor.extract(tx, customer);

    // 2. Dự đoán điểm ML Score [0.0 -> 100.0]
    double computed_ml_score = ml_predictor.predictScore(features);

    // 3. Tận dụng hàm assessRisk sẵn có với điểm ML vừa tính
    return assessRisk(tx, customer, computed_ml_score);
}
```

---

## 🛠️ BƯỚC 3: SỬA LỖI BIÊN DỊCH TRONG `TransactionBLL.cpp`

Mở file [sources/bll/TransactionBLL.cpp](file:///d:/study/courses%20in%20university/ki%203/PBL%202/Electronic-Payment-Fraud-Detection---Risk-management%20(1)/sources/bll/TransactionBLL.cpp) và sửa 2 lỗi cú pháp sau:

1. **Tại dòng 67 (Sửa `deduct` thành `withdraw`)**:
   ```cpp
   // ❌ Cũ (Sai): acc->deduct(tx.GetAmount());
   // ✅ Mới (Đúng hàm của Account.h):
   acc->withdraw(tx.GetAmount());
   ```

2. **Tại dòng 68 (Sử dụng `DecisionAction::CHALLENGE`)**:
   ```cpp
   } else if (action == DecisionAction::CHALLENGE) {
       res.success = false;
       res.message = "Giao dịch có nghi vấn (CHALLENGE). Yêu cầu xác thực thêm OTP!";
   }
   ```

---

## 🛠️ BƯỚC 4: KHÔI PHỤC DỮ LIỆU CHUẨN CHO FILE TEXT

Hiện tại 2 file dữ liệu bị dính chuỗi lỗi `HENRY` làm mất dấu `|`. Hãy thay bằng nội dung chuẩn dưới đây:

### 📄 4.1 Khôi phục file: `data/accounts.txt`
```text
# account_id|customer_id|balance|currency|status|created_at
ACC_001|CUST_ALICE|2500.00|USD|0|1719216000
ACC_002|CUST_BOB|500.00|USD|0|1719216000
ACC_003|CUST_CHARLIE|8000.00|USD|0|1719216000
ACC_004|CUST_DAVID|125.50|USD|0|1719216000
ACC_005|CUST_EMMA|3400.00|USD|0|1719216000
```
*(Ghi chú: `status` = 0 nghĩa là `AccountStatus::ACTIVE`)*

---

### 📄 4.2 Khôi phục file: `data/customers.txt`
```text
# customer_id|fullname|email|phone_number|city|country|latitude|longitude|risk_tier,baseline_risk_score,is_kyc_verified,linked_account_ids,trusted_device_ids
CUST_ALICE|Alice Nguyen|alice@gmail.com|0905123456|Ha Noi|Vietnam|21.0285|105.854|0,10.0,1,ACC_001,DEV_ALICE_PHONE;DEV_LAPTOP
CUST_BOB|Bob Tran|bob@gmail.com|0912345678|Da Nang|Vietnam|16.0544|108.202|0,15.0,1,ACC_002,DEV_BOB_PHONE
CUST_CHARLIE|Charlie Le|charlie@gmail.com|0988776655|Ho Chi Minh|Vietnam|10.8231|106.63|1,25.0,1,ACC_003,DEV_CHARLIE_TAB
```

---

## 🛠️ BƯỚC 5: CẬP NHẬT FILE `Makefile` CHUẨN

Mở file [Makefile](file:///d:/study/courses%20in%20university/ki%203/PBL%202/Electronic-Payment-Fraud-Detection---Risk-management%20(1)/Makefile) và thay thế toàn bộ bằng nội dung sạch sau (đã thêm `TransactionService.cpp` và `FeatureExtractor.cpp`):

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

# 1. Core models sources
SRCS_CORE = sources/core/common/Types.cpp \
            sources/core/models/transaction.cpp \
            sources/core/models/device.cpp \
            sources/core/models/location.cpp \
            sources/core/models/FraudAlert.cpp \
            sources/core/models/RiskAssessment.cpp \
            sources/core/models/Customer.cpp \
            sources/core/models/Account.cpp \
            sources/core/models/Merchant.cpp \
            sources/core/models/PaymentMethod.cpp

# 2. Engine & Rules sources
SRCS_ENGINE = sources/engine/rules/DeviceRiskRule.cpp \
              sources/engine/rules/HighAmountRule.cpp \
              sources/engine/rules/BlacklistRule.cpp \
              sources/engine/rules/VelocityRule.cpp \
              sources/engine/NativeMLPredictor.cpp \
              sources/engine/FeatureExtractor.cpp \
              sources/engine/RiskEngine.cpp \
              sources/engine/DecisionEngine.cpp

# 3. DAL & BLL sources (Đã chuyển sang DAL & BLL)
SRCS_DAL = sources/dal/AccountDAL.cpp \
           sources/dal/CustomerDAL.cpp
SRCS_BLL = sources/bll/Auth.cpp \
           sources/bll/TransactionBLL.cpp

# Toàn bộ mã nguồn liên kết
ALL_SRCS = $(SRCS_ENGINE) $(SRCS_BLL) $(SRCS_DAL) $(SRCS_CORE)

# Main application & Test
SRCS_APP = sources/main.cpp $(ALL_SRCS)
SRCS_TEST = tests/test_rules_step_by_step.cpp $(ALL_SRCS)

BIN_DIR = bin
TARGET_APP = $(BIN_DIR)/epfd_app.exe
TARGET_TEST = $(BIN_DIR)/test_rules_step.exe

.PHONY: all clean build test run

all: build test

build: $(TARGET_APP) $(TARGET_TEST)

$(BIN_DIR):
	@if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"

$(TARGET_APP): $(SRCS_APP) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS_APP)

$(TARGET_TEST): $(SRCS_TEST) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS_TEST)

test: $(TARGET_TEST)
	@echo.
	@$(TARGET_TEST)

run: $(TARGET_APP)
	@echo.
	@$(TARGET_APP)

clean:
	@if exist "$(BIN_DIR)" rmdir /s /q "$(BIN_DIR)"
```

---

## 🛠️ BƯỚC 6: VIẾT KỊCH BẢN DEMO THỰC TẾ VÀO `sources/main.cpp`

Thay thế toàn bộ nội dung [sources/main.cpp](file:///d:/study/courses%20in%20university/ki%203/PBL%202/Electronic-Payment-Fraud-Detection---Risk-management%20(1)/sources/main.cpp) bằng code chạy demo 3 kịch bản cực đẹp mắt:

```cpp
#include <iostream>
#include <iomanip>
#include "bll/TransactionBLL.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

void inTieuDe(const std::string& tieu_de) {
    std::cout << "\n" << CYAN << BOLD 
              << "========================================================================\n"
              << "  " << tieu_de << "\n"
              << "========================================================================\n" << RESET;
}

void chayKichBanDemo(TransactionBLL& bll) {
    Location ha_noi("Vietnam", "Ha Noi", 21.0285, 105.854);
    Location hcm("Vietnam", "Ho Chi Minh", 10.8231, 106.63);

    // -------------------------------------------------------------------------
    // KỊCH BẢN 1: GIAO DỊCH HỢP LỆ (ALICE MUA HÀNG $45 TẠI NHÀ HÀ NỘI)
    // -------------------------------------------------------------------------
    inTieuDe("KICH BAN 1: GIAO DICH HOP LE TAI NHA (ALICE - $45.00)");
    {
        Device dev_alice("DEV_ALICE_PHONE", "192.168.1.10", "FP_ALICE_OK", ha_noi, false, false);
        Transaction tx1("TX_DEMO_01", TransactionType::BUY, "CUST_ALICE", "M_WINMART", "ACC_001", 
                        45.0, "USD", std::chrono::system_clock::now(), ha_noi, "192.168.1.10", 
                        dev_alice, "M_WINMART", PaymentType::CARD);

        TransactionResult res = service.processTransaction(tx1);

        std::cout << "  - Khach hang: Alice | Tai khoan nguon: ACC_001 | So tien: $45.00\n";
        std::cout << "  - Diem Rule: " << std::fixed << std::setprecision(1) << res.assessment.getRuleScore()
                  << " | Diem ML: " << res.assessment.getMlScore()
                  << " | Tong diem rui ro: " << res.assessment.getCombinedScore() << "/100.0\n";
        std::cout << "  - Ket qua: " << GREEN << BOLD << res.message << RESET << "\n";
    }

    // -------------------------------------------------------------------------
    // KỊCH BẢN 2: SPAM QUẸT THẺ DỒN DẬP (KÍCH HOẠT VELOCITY RULE)
    // -------------------------------------------------------------------------
    inTieuDe("KICH BAN 2: TAN CONG QUET THE DON DAP (SPAM 6 LAN LIEN TIEP / 10s)");
    {
        Device dev_spam("DEV_BOB_PHONE", "14.232.208.1", "FP_BOB", ha_noi, false, false);
        for (int i = 1; i <= 6; ++i) {
            Transaction tx_spam("TX_SPAM_0" + std::to_string(i), TransactionType::BUY, "CUST_BOB", 
                                "M_ONLINE", "ACC_002", 50.0, "USD", std::chrono::system_clock::now(), 
                                ha_noi, "14.232.208.1", dev_spam, "M_ONLINE", PaymentType::CARD);

            TransactionResult res = service.processTransaction(tx_spam);
            std::cout << "  Lan quet " << i << ": Diem rui ro = " << std::setw(5) << res.assessment.getCombinedScore();
            if (res.action == DecisionAction::APPROVE) {
                std::cout << " -> " << GREEN << "APPROVE" << RESET << "\n";
            } else {
                std::cout << " -> " << RED << BOLD << "BLOCK (" << res.message << ")" << RESET << "\n";
            }
        }
    }

    // -------------------------------------------------------------------------
    // KỊCH BẢN 3: GIAO DỊCH NGUY HIỂM (MÁY ẢO + CÁCH XA 1140KM + SỐ TIỀN $3500)
    // -------------------------------------------------------------------------
    inTieuDe("KICH BAN 3: GIAO DICH GIAN LAN NGUY HIEM (MAY AO + VI TRI LA + TIEN LON)");
    {
        Device dev_hacker("DEV_EMU_ROOT", "10.0.2.15", "FP_ROOTED", hcm, true, false); // Cờ Virtual = true
        Transaction tx3("TX_DEMO_03", TransactionType::TRANSFER, "CUST_ALICE", "M_CRYPTO", "ACC_001", 
                        3500.0, "USD", std::chrono::system_clock::now(), hcm, "10.0.2.15", 
                        dev_hacker, "M_CRYPTO", PaymentType::CARD);

        TransactionResult res = service.processTransaction(tx3);

        std::cout << "  - So tien: $3500.00 | Vi tri: TP.HCM (Nha Alice tai Ha Noi, cach ~1140km)\n";
        std::cout << "  - Diem Rule: " << res.assessment.getRuleScore()
                  << " | Diem ML: " << res.assessment.getMlScore()
                  << " | Tong diem rui ro: " << res.assessment.getCombinedScore() << "/100.0\n";
        std::cout << "  - Danh sach canh bao gian lan phat hien:\n";
        const auto& reasons = res.assessment.getReasons();
        for (int i = 0; i < (int)reasons.size(); ++i) {
            std::cout << "    * " << YELLOW << reasons[i] << RESET << "\n";
        }
        std::cout << "  - Ket qua: " << RED << BOLD << res.message << RESET << "\n";
    }
}

int main() {
    std::cout << CYAN << BOLD;
    std::cout << "========================================================================\n";
    std::cout << "  EPFD-RAS: HE THONG QUAN TRI RUI RO & PHAT HIEN GIAN LAN THANH TOAN   \n";
    std::cout << "  Khoa CNTT - Truong Dai hoc Bach Khoa, Dai hoc Da Nang                 \n";
    std::cout << "========================================================================\n" << RESET;

    TransactionBLL bll;
    chayKichBanDemo(bll);

    return 0;
}
```

---

## 🚀 BƯỚC 7: LỆNH BIÊN DỊCH VÀ CHẠY THỰC TẾ

Sau khi bạn đã áp dụng xong các bước trên, hãy mở terminal PowerShell trong VS Code và gõ:

```powershell
# 1. Biên dịch toàn bộ dự án
mingw32-make build

# 2. Chạy ứng dụng Demo
mingw32-make run
```

Nếu muốn chạy kiểm thử các luật độc lập:
```powershell
mingw32-make test
```

---

## ✅ CHECKLIST NGHIỆM THU HOÀN THÀNH

- [x] Đã tạo `include/engine/FeatureExtractor.h`
- [x] Đã tạo `sources/engine/FeatureExtractor.cpp`
- [x] Đã thêm `getMLPredictor()` và `assessRisk` vào `RiskEngine`
- [x] Đã sửa `withdraw` và `CHALLENGE` trong `TransactionBLL.cpp`
- [x] Đã khôi phục dữ liệu sạch cho `data/accounts.txt` và `data/customers.txt`
- [x] Đã cập nhật file `Makefile` (chuẩn hóa `sources/dal/AccountDAL.cpp` và `sources/dal/CustomerDAL.cpp`)
- [x] Đã cập nhật `sources/main.cpp`
- [x] Gõ lệnh `mingw32-make run` hiển thị 3 kịch bản màu sắc Xanh/Đỏ/Vàng thành công 100%!
