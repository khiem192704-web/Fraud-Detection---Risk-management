# 📋 TÀI LIỆU HƯỚNG DẪN CHI TIẾT CÁC ĐIỂM CẦN CHỈNH SỬA & HOÀN THIỆN
*(Tài liệu tham chiếu chuẩn — Tuyệt đối không can thiệp hay làm thay đổi mã nguồn hiện tại)*

> **Mục đích**: Tài liệu này tổng hợp toàn bộ các phần cần chỉnh sửa, bổ sung và hoàn thiện cho dự án **EPFD-RAS (PBL2)**. Bạn có thể sử dụng tài liệu này để tự tay đối chiếu, copy/paste hoặc chỉnh sửa từng file khi sẵn sàng mà **không sợ làm gãy bất kỳ đoạn code nào đang có**.

---

## 📌 MỤC LỤC CÁC HẠNG MỤC CHỈNH SỬA

| STT | Hạng mục | Mức độ ảnh hưởng | File liên quan |
| :---: | :--- | :---: | :--- |
| **1** | [Module Trích xuất đặc trưng `FeatureExtractor`](#1-module-trích-xuất-đặc-trưng-featureextractor-tạo-mới) | **Tạo file mới** (Không ảnh hưởng code cũ) | `include/engine/FeatureExtractor.h`<br>`sources/engine/FeatureExtractor.cpp` |
| **2** | [Đồng bộ hóa 4 đặc trưng trong `NativeMLPredictor`](#2-đồng-bộ-hóa-4-đặc-trưng-trong-nativemlpredictor-chỉnh-sửa) | Chỉnh sửa nhỏ 1 hàm constructor | `sources/engine/NativeMLPredictor.cpp` |
| **3** | [Nâng cấp `RiskEngine` tự chủ & tương thích ngược](#3-nâng-cấp-riskengine-tự-chủ--tương-thích-ngược-chỉnh-sửa) | Thêm phương thức mới (Giữ nguyên hàm cũ) | `include/engine/RiskEngine.h`<br>`sources/engine/RiskEngine.cpp` |
| **4** | [Sửa lỗi đường dẫn trong `Makefile`](#4-sửa-lỗi-đường-dẫn-trong-makefile-chỉnh-sửa) | Chỉnh sửa cấu hình build | `Makefile` |
| **5** | [Hoàn thiện Script huấn luyện `train_model.py`](#5-hoàn-thiện-script-huấn-luyện-train_modelpy-bỏ-comment) | Bỏ comment các TODO | `python/train_model.py` |
| **6** | [Xây dựng Tầng Dịch vụ `TransactionService`](#6-xây-dựng-tầng-dịch-vụ-transactionservice-tạo-mới) | **Tạo file mới** | `include/services/TransactionService.h`<br>`sources/services/TransactionService.cpp` |
| **7** | [Giao diện điều khiển `ConsoleUI` & Kịch bản `main.cpp`](#7-giao-diện-điều-khiển-consoleui--kịch-bản-maincpp-tạo-mới--cập-nhật) | Tạo file UI & Cập nhật `main.cpp` | `include/ui/ConsoleUI.h`<br>`sources/ui/ConsoleUI.cpp`<br>`sources/main.cpp` |
| **8** | [Cấu trúc dữ liệu `HashMap.h` tự viết (Tùy chọn DSA)](#8-cấu-trúc-dữ-liệu-hashmaph-tự-viết-tùy-chọn-dsa) | Nâng cao điểm DSA | `include/dsa/hashmap.h` |

---

## 1. MODULE TRÍCH XUẤT ĐẶC TRƯNG `FeatureExtractor` (TẠO MỚI)

### 🎯 Mục đích:
Giải quyết triệt để nhận xét của thầy cô: Trích xuất chính xác 4 thuộc tính số học (`amount`, `velocity_count`, `geo_distance_km`, `is_device_risk`) từ đối tượng `Transaction` và `Customer`.

---

### 📄 File 1.1: `include/engine/FeatureExtractor.h` (Tạo mới hoàn toàn)

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
    // Cửa sổ trượt thời gian lưu trữ các mốc thời gian giao dịch gần nhất
    Deque<Timestamp> sliding_window;
    std::chrono::seconds window_duration{60}; // Cửa sổ kiểm tra mặc định: 60 giây

public:
    explicit FeatureExtractor(std::chrono::seconds duration = std::chrono::seconds(60));

    // Trích xuất vector 4 đặc trưng: [amount, velocity_count, geo_distance_km, is_device_risk]
    Vector<double> extract(const Transaction& tx, const Customer& customer);

    // Xóa bộ đệm cửa sổ trượt (dùng khi bắt đầu phiên test mới)
    void reset();

    // Lấy danh sách tên 4 đặc trưng theo đúng thứ tự
    Vector<std::string> getFeatureNames() const;

private:
    double extractAmount(const Transaction& tx) const;
    double extractVelocity(const Transaction& tx);
    double extractGeoDistance(const Transaction& tx, const Customer& customer) const;
    double extractDeviceRisk(const Transaction& tx) const;
};
```

---

### 📄 File 1.2: `sources/engine/FeatureExtractor.cpp` (Tạo mới hoàn toàn)

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

    // 1. Loại bỏ các giao dịch cũ vượt quá cửa sổ trượt (trượt thời gian)
    while (!sliding_window.empty()) {
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - sliding_window.front()).count();
        if (diff > window_duration.count()) {
            sliding_window.pop_front();
        } else {
            break;
        }
    }

    // 2. Thêm giao dịch hiện tại vào cuối Deque
    sliding_window.push_back(now);

    // 3. Số lượng giao dịch trong cửa sổ chính là velocity
    return static_cast<double>(sliding_window.size());
}

double FeatureExtractor::extractGeoDistance(const Transaction& tx, const Customer& customer) const {
    const Location& tx_loc = tx.GetLocation();
    const Location& home_loc = customer.getHomeLocation();

    // Kiểm tra an toàn: nếu tọa độ không hợp lệ thì trả về 0.0 (tránh crash hệ thống)
    if (!tx_loc.isValid() || !home_loc.isValid()) {
        return 0.0;
    }

    try {
        // Tận dụng hàm Haversine đã có sẵn trong Location.cpp
        return tx_loc.distanceTo(home_loc);
    } catch (...) {
        return 0.0;
    }
}

double FeatureExtractor::extractDeviceRisk(const Transaction& tx) const {
    const Device& dev = tx.GetDevice();
    
    // Nếu thiết bị là máy ảo, hoặc bị can thiệp root/hacker, hoặc không an toàn
    if (dev.GetIs_Virtual() || dev.GetIs_Hacker() || !dev.is_Safe()) {
        return 1.0;
    }
    return 0.0;
}

Vector<double> FeatureExtractor::extract(const Transaction& tx, const Customer& customer) {
    Vector<double> features;
    
    // TUYỆT ĐỐI tuân thủ đúng thứ tự 4 đặc trưng đã thống nhất với Python và model_weights.json:
    features.push_back(extractAmount(tx));                // [0] amount
    features.push_back(extractVelocity(tx));              // [1] velocity_count
    features.push_back(extractGeoDistance(tx, customer)); // [2] geo_distance_km
    features.push_back(extractDeviceRisk(tx));            // [3] is_device_risk

    return features;
}
```

---

## 2. ĐỒNG BỘ HÓA 4 ĐẶC TRƯNG TRONG `NativeMLPredictor` (CHỈNH SỬA)

### 🎯 Mục đích:
Constructor mặc định hiện thời trong `sources/engine/NativeMLPredictor.cpp` mới có 3 đặc trưng, cần cập nhật đủ 4 đặc trưng để khớp hoàn toàn với `data/model_weights.json`.

### 🔍 Vị trí cần chỉnh sửa:
Mở file `sources/engine/NativeMLPredictor.cpp`, thay thế constructor mặc định ở dòng 4-20:

#### ❌ Code hiện tại (3 đặc trưng):
```cpp
NativeMLPredictor::NativeMLPredictor() {
    bias = -2.8;
    weights.push_back(0.002);
    weights.push_back(1.45);
    weights.push_back(2.10);
    feature_names.push_back("amount");
    feature_names.push_back("velocity");
    feature_names.push_back("device_risk");
    ready = true;
}
```

#### ✅ Code đề xuất cập nhật (4 đặc trưng chuẩn):
```cpp
NativeMLPredictor::NativeMLPredictor() {
    // Bộ trọng số mặc định đã được hiệu chuẩn cho 4 đặc trưng:
    bias = -3.2;
    weights.push_back(0.0018); // [0] amount
    weights.push_back(1.42);   // [1] velocity_count
    weights.push_back(0.0035); // [2] geo_distance_km
    weights.push_back(2.15);   // [3] is_device_risk

    feature_names.push_back("amount");
    feature_names.push_back("velocity_count");
    feature_names.push_back("geo_distance_km");
    feature_names.push_back("is_device_risk");
    ready = true;
}
```

---

## 3. NÂNG CẤP `RiskEngine` TỰ CHỦ & TƯƠNG THÍCH NGƯỢC (CHỈNH SỬA)

### 🎯 Mục đích:
Cho phép `RiskEngine` tự động trích xuất đặc trưng và tự tính `ml_score` khi nhận `tx` và `customer`, đồng thời **vẫn giữ nguyên hàm cũ** để các unit test cũ không bao giờ bị lỗi biên dịch.

---

### 📄 File 3.1: Chỉnh sửa `include/engine/RiskEngine.h`

Thêm `#include "engine/FeatureExtractor.h"` và `#include "engine/NativeMLPredictor.h"`:

```cpp
#pragma once
#include <string>
#include "DSA/vector.h"
#include "engine/rules/IFraudRule.h"
#include "engine/FeatureExtractor.h"     // <--- THÊM MỚI
#include "engine/NativeMLPredictor.h"    // <--- THÊM MỚI
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
    Vector<IFraudRule*> rules;
    RiskPolicy policy;
    FeatureExtractor feature_extractor; // <--- THÊM MỚI: Tự quản lý bộ trích xuất đặc trưng
    NativeMLPredictor ml_predictor;       // <--- THÊM MỚI: Tự quản lý bộ dự đoán học máy

public:
    explicit RiskEngine(const RiskPolicy& custom_policy = RiskPolicy());
    ~RiskEngine();

    void registerRule(IFraudRule* rule);
    int getRuleCount() const;
    void setPolicy(const RiskPolicy& new_policy);
    const RiskPolicy& getPolicy() const;

    // Getter cho các module con để cấu hình nếu cần
    NativeMLPredictor& getMLPredictor() { return ml_predictor; }
    FeatureExtractor& getFeatureExtractor() { return feature_extractor; }

    // =========================================================================
    // HÀM MỚI: Tự động trích xuất đặc trưng -> tự tính ML Score -> đánh giá rủi ro
    // =========================================================================
    RiskAssessment assessRisk(const Transaction& tx, const Customer& customer);

    // HÀM CŨ: Vẫn giữ nguyên 100% để tương thích ngược các bài test thủ công cũ
    RiskAssessment assessRisk(const Transaction& tx, const Customer& customer, double ml_score);
};
```

---

### 📄 File 3.2: Chỉnh sửa `sources/engine/RiskEngine.cpp`

Thêm định nghĩa cho hàm `assessRisk(const Transaction& tx, const Customer& customer)` ở cuối file:

```cpp
// THÊM ĐOẠN NÀY VÀO CUỐI FILE sources/engine/RiskEngine.cpp:
RiskAssessment RiskEngine::assessRisk(const Transaction& tx, const Customer& customer) {
    // 1. Tự động trích xuất 4 đặc trưng từ Transaction và Customer
    Vector<double> features = feature_extractor.extract(tx, customer);

    // 2. Tự động gọi NativeMLPredictor tính điểm rủi ro ML (0.0 -> 100.0)
    double computed_ml_score = ml_predictor.predictScore(features);

    // 3. Tận dụng lại hàm assessRisk cũ với ml_score vừa tính được
    return assessRisk(tx, customer, computed_ml_score);
}
```

---

## 4. SỬA LỖI ĐƯỜNG DẪN TRONG `Makefile` (CHỈNH SỬA)

### 🎯 Mục đích:
Khắc phục lỗi `No rule to make target 'sources/core/domain/transaction/transaction.cpp'`. Trong dự án thực tế, các file `.cpp` nằm trực tiếp trong thư mục `sources/core/domain/`, không có các thư mục con lồng nhau.

### 🔍 Vị trí cần chỉnh sửa:
Mở file `Makefile`, thay thế khối `SRCS_CORE` ở dòng 4-12:

#### ❌ Code hiện tại:
```makefile
SRCS_CORE = sources/core/common/Types.cpp \
            sources/core/domain/transaction/transaction.cpp \
            sources/core/domain/device/device.cpp \
            sources/core/domain/location/location.cpp \
            sources/core/domain/alert/FraudAlert.cpp \
            sources/core/domain/alert/RiskAssessment.cpp \
            sources/core/domain/account/Customer.cpp
```

#### ✅ Code đề xuất cập nhật:
```makefile
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

# Core domain & infrastructure sources (Đã sửa đường dẫn đúng thực tế)
SRCS_CORE = sources/core/common/Types.cpp \
            sources/core/domain/transaction.cpp \
            sources/core/domain/device.cpp \
            sources/core/domain/location.cpp \
            sources/core/domain/FraudAlert.cpp \
            sources/core/domain/RiskAssessment.cpp \
            sources/core/domain/Customer.cpp \
            sources/core/domain/Account.cpp \
            sources/core/domain/Merchant.cpp \
            sources/core/domain/PaymentMethod.cpp

# Engine & Rules implementation
SRCS_ENGINE = sources/engine/rules/DeviceRiskRule.cpp \
              sources/engine/rules/HighAmountRule.cpp \
              sources/engine/rules/BlacklistRule.cpp \
              sources/engine/rules/VelocityRule.cpp \
              sources/engine/NativeMLPredictor.cpp \
              sources/engine/FeatureExtractor.cpp \
              sources/engine/RiskEngine.cpp \
              sources/engine/DecisionEngine.cpp

# Storage & Services
SRCS_STORAGE = sources/storage/AccountRepo.cpp \
               sources/storage/CustomerRepo.cpp \
               sources/services/Auth.cpp

# Main application
SRCS_APP = sources/main.cpp $(SRCS_ENGINE) $(SRCS_STORAGE) $(SRCS_CORE)

# Step-by-step test
SRCS_TEST = tests/test_rules_step_by_step.cpp $(SRCS_ENGINE) $(SRCS_STORAGE) $(SRCS_CORE)

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

## 5. HOÀN THIỆN SCRIPT HUẤN LUYỆN `train_model.py` (BỎ COMMENT)

### 🎯 Mục đích:
Kích hoạt toàn bộ các đoạn code gợi ý trong file `python/train_model.py` để khi chạy `python python/train_model.py`, mô hình sẽ đọc dữ liệu từ `data/transactions.csv` và sinh ra file `data/model_weights.json` thật.

### 📄 Nội dung hoàn chỉnh của `python/train_model.py`:

```python
"""
============================================================================
[BƯỚC 6.1] SCRIPT HUẤN LUYỆN MÔ HÌNH HỌC MÁY & XUẤT FILE MODEL_WEIGHTS.JSON
============================================================================
"""

import json
import os
import pandas as pd
import numpy as np
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, roc_auc_score

DATA_PATH = "data/transactions.csv"
OUTPUT_PATH = "data/model_weights.json"

def main():
    print("=" * 60)
    print("  QUY TRINH HUAN LUYEN MO HINH HOC MAY & XUAT JSON")
    print("=" * 60)

    # 1. Đọc dữ liệu từ file CSV
    if not os.path.exists(DATA_PATH):
        print(f"Lỗi: Không tìm thấy file dữ liệu tại {DATA_PATH}")
        return
    df = pd.read_csv(DATA_PATH)
    print(f"-> Đã đọc {len(df)} dòng dữ liệu từ {DATA_PATH}")

    # 2. Chuẩn bị danh sách đặc trưng X và nhãn gian lận y
    feature_cols = ['amount', 'velocity_count', 'geo_distance_km', 'is_device_risk']
    
    # Kiểm tra nhãn is_fraud
    if 'is_fraud' not in df.columns or df['is_fraud'].nunique() < 2:
        print("-> Cảnh báo: Tập dữ liệu chưa có nhãn gian lận, khởi tạo nhãn giả lập...")
        df['is_fraud'] = ((df['amount'] > 2000.0) | (df['velocity_count'] > 5) | (df['is_device_risk'] == 1)).astype(int)

    X = df[feature_cols]
    y = df['is_fraud']
    print(f"-> Phân bố nhãn mục tiêu: {dict(y.value_counts())}")

    # 3. Huấn luyện mô hình Logistic Regression
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42, stratify=y)
    model = LogisticRegression(class_weight='balanced', max_iter=1000)
    model.fit(X_train, y_train)

    y_pred_prob = model.predict_proba(X_test)[:, 1]
    print(f"-> ROC-AUC đánh giá trên tập Test: {roc_auc_score(y_test, y_pred_prob):.4f}")

    # 4. Trích xuất trọng số Weights và Bias
    weights = [round(float(w), 4) for w in model.coef_[0]]
    bias = round(float(model.intercept_[0]), 4)
    print(f"-> Trọng số weights [w1, w2, w3, w4]: {weights}")
    print(f"-> Hệ số tự do bias (w0): {bias}")

    # 5. Đóng gói ra file JSON
    export_data = {
        "description": "Calibrated Logistic Regression Weights for EPFD-RAS",
        "features": feature_cols,
        "weights": weights,
        "bias": bias,
        "optimal_threshold": 0.5
    }

    with open(OUTPUT_PATH, 'w', encoding='utf-8') as f:
        json.dump(export_data, f, indent=4)
    print(f"[THÀNH CÔNG] Đã xuất file trọng số ra: {OUTPUT_PATH}")

if __name__ == "__main__":
    main()
```

---

## 6. XÂY DỰNG TẦNG DỊCH VỤ `TransactionService` (TẠO MỚI)

### 🎯 Mục đích:
Đóng gói toàn bộ luồng nghiệp vụ thực tế thành một quy trình khép kín:
$$\text{Transaction} \to \text{Kiểm tra tài khoản/số dư} \to \text{RiskEngine} \to \text{DecisionEngine} \to \text{Cập nhật số dư} \to \text{Ghi nhận kết quả}$$

---

### 📄 File 6.1: `include/services/TransactionService.h` (Tạo mới hoàn toàn)

```cpp
#pragma once

#include <string>
#include "core/domain/Transaction.h"
#include "core/domain/Customer.h"
#include "core/domain/Account.h"
#include "core/domain/RiskAssessment.h"
#include "engine/RiskEngine.h"
#include "engine/DecisionEngine.h"
#include "storage/AccountRepo.h"
#include "storage/CustomerRepo.h"
#include "DSA/vector.h"

struct TransactionResult {
    bool success{false};
    DecisionAction action{DecisionAction::BLOCK};
    std::string message{""};
    RiskAssessment assessment;
};

class TransactionService {
private:
    RiskEngine risk_engine;
    DecisionEngine decision_engine;
    AccountRepo account_repo;
    CustomerRepo customer_repo;
    Vector<Transaction> transaction_history;

public:
    TransactionService();

    // Xử lý một giao dịch hoàn chỉnh từ đầu đến cuối
    TransactionResult processTransaction(Transaction& tx);

    // Đăng ký một luật gian lận mới vào hệ thống
    void addRule(IFraudRule* rule);

    // Truy cập các Repository
    AccountRepo& getAccountRepo() { return account_repo; }
    CustomerRepo& getCustomerRepo() { return customer_repo; }
    const Vector<Transaction>& getHistory() const { return transaction_history; }
};
```

---

### 📄 File 6.2: `sources/services/TransactionService.cpp` (Tạo mới hoàn toàn)

```cpp
#include "services/TransactionService.h"
#include "engine/rules/HighAmountRule.h"
#include "engine/rules/VelocityRule.h"
#include "engine/rules/DeviceRiskRule.h"
#include "engine/rules/BlacklistRule.h"

TransactionService::TransactionService() {
    // Nạp dữ liệu tài khoản và khách hàng từ file ổ đĩa
    account_repo.loadFromFile("data/accounts.txt");
    customer_repo.loadFromFile("data/customers.txt");

    // Đăng ký bộ 4 luật mặc định vào RiskEngine
    risk_engine.registerRule(new HighAmountRule(3000.0, 3.0, 25.0));
    risk_engine.registerRule(new VelocityRule(5, std::chrono::seconds(60), 30.0));
    risk_engine.registerRule(new DeviceRiskRule(30.0));
    risk_engine.registerRule(new BlacklistRule(50.0));

    // Nạp trọng số mô hình ML nếu có file
    risk_engine.getMLPredictor().loadModel("data/model_weights.json");
}

void TransactionService::addRule(IFraudRule* rule) {
    risk_engine.registerRule(rule);
}

TransactionResult TransactionService::processTransaction(Transaction& tx) {
    TransactionResult res;

    // 1. Kiểm tra sự tồn tại của Tài khoản
    Account* acc = account_repo.findById(tx.GetAccountID());
    if (!acc) {
        res.success = false;
        res.action = DecisionAction::BLOCK;
        res.message = "Lỗi: Không tìm thấy tài khoản nguồn ID: " + tx.GetAccountID();
        tx.reject();
        return res;
    }

    // 2. Kiểm tra số dư tài khoản
    if (acc->getBalance() < tx.GetAmount()) {
        res.success = false;
        res.action = DecisionAction::BLOCK;
        res.message = "Lỗi: Số dư không đủ để thực hiện giao dịch!";
        tx.reject();
        return res;
    }

    // 3. Tìm thông tin khách hàng tương ứng (hoặc dùng profile mặc định)
    Customer* cust = customer_repo.findById(tx.GetCustomerID());
    Customer current_cust;
    if (cust) {
        current_cust = *cust;
    } else {
        current_cust = Customer(tx.GetCustomerID(), "Standard Customer", "user@bank.com", "0900000000", tx.GetLocation());
    }

    // 4. Thẩm định rủi ro tự chủ (RiskEngine tự gọi FeatureExtractor & NativeMLPredictor)
    RiskAssessment assessment = risk_engine.assessRisk(tx, current_cust);
    res.assessment = assessment;

    // 5. Ra quyết định nghiệp vụ (DecisionEngine)
    DecisionAction action = decision_engine.decide(tx, current_cust, assessment);
    res.action = action;

    // 6. Thực thi kết quả tài chính
    if (action == DecisionAction::APPROVE) {
        acc->deduct(tx.GetAmount()); // Trừ tiền tài khoản
        account_repo.saveToFile("data/accounts.txt");
        res.success = true;
        res.message = "Giao dịch được CHẤP THUẬN (APPROVE). Đã trừ tiền thành công!";
    } else if (action == DecisionAction::CHALLENGE) {
        res.success = false;
        res.message = "Giao dịch có nghi vấn (CHALLENGE). Yêu cầu xác thực thêm OTP!";
    } else if (action == DecisionAction::REVIEW) {
        res.success = false;
        res.message = "Giao dịch bị TẠM GIỮ (REVIEW). Đã chuyển sang hàng đợi thẩm định!";
    } else {
        res.success = false;
        res.message = "Giao dịch bị TỪ CHỐI (BLOCK) do rủi ro vượt ngưỡng an toàn!";
    }

    // 7. Lưu vào lịch sử giao dịch
    transaction_history.push_back(tx);
    return res;
}
```

---

## 7. GIAO DIỆN ĐIỀU KHIỂN `ConsoleUI` & KỊCH BẢN `main.cpp` (TẠO MỚI / CẬP NHẬT)

### 🎯 Mục đích:
Cung cấp giao diện Console có màu sắc ANSI trực quan, kèm 3 kịch bản chạy thử nghiệm ghi điểm tuyệt đối trước hội đồng bảo vệ.

---

### 📄 File 7.1: `sources/main.cpp` (Cập nhật để chạy kịch bản tự động)

```cpp
#include <iostream>
#include "services/TransactionService.h"

// Màu sắc ANSI hiển thị trực quan
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

void inTieuDe(const std::string& tieu_de) {
    std::cout << "\n" << CYAN << BOLD 
              << "========================================================\n"
              << "  " << tieu_de << "\n"
              << "========================================================\n" << RESET;
}

void chayKichBanDemo(TransactionService& service) {
    Location ha_noi("Vietnam", "Ha Noi", 21.0285, 105.854);
    Location hcm("Vietnam", "Ho Chi Minh", 10.8231, 106.63);

    // -------------------------------------------------------------
    // KỊCH BẢN 1: GIAO DỊCH HỢP LỆ (ALICE MUA HÀNG $45 TẠI NHÀ)
    // -------------------------------------------------------------
    inTieuDe("KICH BAN 1: GIAO DICH HOP LE TAI NHA (ALICE - $45)");
    {
        Device thiet_bi_that("DEV_ALICE_REAL", "192.168.1.10", "FP_OK", ha_noi, false, false);
        Transaction tx1("TX_DEMO_01", TransactionType::BUY, "CUST_ALICE", "M_WINMART", "ACC_001", 
                        45.0, "USD", std::chrono::system_clock::now(), ha_noi, "192.168.1.10", 
                        thiet_bi_that, "M_WINMART", PaymentType::CARD);

        TransactionResult res = service.processTransaction(tx1);

        std::cout << "  - So tien: $45.0 | Diem Rule: " << res.assessment.getRuleScore()
                  << " | Diem ML: " << res.assessment.getMlScore()
                  << " | Tong diem: " << res.assessment.getCombinedScore() << "\n";
        std::cout << "  - Ket qua: " << GREEN << BOLD << res.message << RESET << "\n";
    }

    // -------------------------------------------------------------
    // KỊCH BẢN 2: SPAM QUẸT THẺ DỒN DẬP (KÍCH HOẠT VELOCITY RULE)
    // -------------------------------------------------------------
    inTieuDe("KICH BAN 2: TAN CONG QUET THE DON DAP (SPAM 6 LAN / 10s)");
    {
        Device thiet_bi("DEV_SPAM", "14.232.208.1", "FP_SPAM", ha_noi, false, false);
        for (int i = 1; i <= 6; ++i) {
            Transaction tx_spam("TX_SPAM_0" + std::to_string(i), TransactionType::BUY, "CUST_BOB", 
                                "M_ONLINE", "ACC_002", 50.0, "USD", std::chrono::system_clock::now(), 
                                ha_noi, "14.232.208.1", thiet_bi, "M_ONLINE", PaymentType::CARD);

            TransactionResult res = service.processTransaction(tx_spam);
            std::cout << "  Lan quet " << i << ": Diem = " << res.assessment.getCombinedScore();
            if (res.action == DecisionAction::APPROVE) {
                std::cout << " -> " << GREEN << "APPROVE" << RESET << "\n";
            } else {
                std::cout << " -> " << RED << BOLD << "BLOCK (" << res.message << ")" << RESET << "\n";
            }
        }
    }

    // -------------------------------------------------------------
    // KỊCH BẢN 3: GIAO DỊCH NGUY HIỂM (CÁCH 1140KM + MÁY ẢO + SỐ TIỀN LỚN)
    // -------------------------------------------------------------
    inTieuDe("KICH BAN 3: GIAO DICH NGUY HIEM (MAY AO + TAI KHOAN CACH XA 1140KM)");
    {
        Device may_ao("DEV_EMULATOR_01", "10.0.2.15", "FP_ROOT", hcm, true, false); // Cờ Virtual = true
        Transaction tx3("TX_DEMO_03", TransactionType::TRANSFER, "CUST_ALICE", "M_CRYPTO", "ACC_001", 
                        3500.0, "USD", std::chrono::system_clock::now(), hcm, "10.0.2.15", 
                        may_ao, "M_CRYPTO", PaymentType::CARD);

        TransactionResult res = service.processTransaction(tx3);

        std::cout << "  - So tien: $3500.0 | Diem Rule: " << res.assessment.getRuleScore()
                  << " | Diem ML: " << res.assessment.getMlScore()
                  << " | Tong diem: " << res.assessment.getCombinedScore() << "\n";
        std::cout << "  - Cac ly do vi pham phat hien:\n";
        const auto& reasons = res.assessment.getReasons();
        for (int i = 0; i < reasons.size(); ++i) {
            std::cout << "    * " << YELLOW << reasons[i] << RESET << "\n";
        }
        std::cout << "  - Ket qua: " << RED << BOLD << res.message << RESET << "\n";
    }
}

int main() {
    std::cout << CYAN << BOLD;
    std::cout << "========================================================\n";
    std::cout << "  EPFD-RAS: HE THONG QUAN TRI RUI RO & PHAT HIEN GIAN LAN\n";
    std::cout << "  Khoa CNTT - Truong Dai hoc Bach Khoa, Dai hoc Da Nang  \n";
    std::cout << "========================================================\n" << RESET;

    TransactionService service;
    chayKichBanDemo(service);

    return 0;
}
```

---

## 8. CẤU TRÚC DỮ LIỆU `HashMap.h` TỰ VIẾT (TÙY CHỌN DSA)

### 🎯 Mục đích:
Nếu thầy cô yêu cầu 100% cấu trúc dữ liệu phải tự tay cài đặt (không được dùng `std::unordered_map`), file header template này sẽ thay thế trực tiếp vào `BlacklistRule.cpp`.

### 📄 File: `include/dsa/hashmap.h` (Tạo mới)

```cpp
#pragma once
#include <string>
#include <functional>
#include <stdexcept>

template<typename K, typename V>
class HashMap {
private:
    struct HashNode {
        K key;
        V value;
        HashNode* next{nullptr};
        HashNode(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    HashNode** buckets{nullptr};
    size_t capacity{101}; // Kích thước bảng băm (số nguyên tố)
    size_t count{0};

    size_t hashFunction(const K& key) const {
        return std::hash<K>{}(key) % capacity;
    }

public:
    explicit HashMap(size_t cap = 101) : capacity(cap), count(0) {
        buckets = new HashNode*[capacity]();
    }

    ~HashMap() {
        clear();
        delete[] buckets;
    }

    void insert(const K& key, const V& value) {
        size_t index = hashFunction(key);
        HashNode* head = buckets[index];

        while (head) {
            if (head->key == key) {
                head->value = value;
                return;
            }
            head = head->next;
        }

        HashNode* newNode = new HashNode(key, value);
        newNode->next = buckets[index];
        buckets[index] = newNode;
        count++;
    }

    bool find(const K& key, V& out_value) const {
        size_t index = hashFunction(key);
        HashNode* head = buckets[index];

        while (head) {
            if (head->key == key) {
                out_value = head->value;
                return true;
            }
            head = head->next;
        }
        return false;
    }

    bool contains(const K& key) const {
        V temp;
        return find(key, temp);
    }

    void clear() {
        for (size_t i = 0; i < capacity; ++i) {
            HashNode* curr = buckets[i];
            while (curr) {
                HashNode* next = curr->next;
                delete curr;
                curr = next;
            }
            buckets[i] = nullptr;
        }
        count = 0;
    }

    size_t size() const { return count; }
};
```

---

## 📌 LỘ TRÌNH THỰC HIỆN CHECKLIST (TỰ TAY TICK KHI LÀM)

- [ ] **Bước 1**: Tạo `include/engine/FeatureExtractor.h` và `sources/engine/FeatureExtractor.cpp`.
- [ ] **Bước 2**: Sửa constructor trong `sources/engine/NativeMLPredictor.cpp` lên 4 đặc trưng.
- [ ] **Bước 3**: Thêm hàm `assessRisk(tx, customer)` vào `RiskEngine.h` và `RiskEngine.cpp`.
- [ ] **Bước 4**: Cập nhật file `Makefile` để sửa các đường dẫn file.
- [ ] **Bước 5**: Chạy thử lệnh `mingw32-make` trong terminal PowerShell.
- [ ] **Bước 6**: Bỏ comment file `python/train_model.py` và chạy `python python/train_model.py`.
- [ ] **Bước 7**: Tạo `TransactionService` và cập nhật `main.cpp` để chạy 3 kịch bản demo.
