# 🗺️ Lộ Trình Hoàn Thiện Engine — EPFD-RAS (Dành Cho Hoàng)

> **Nhánh**: `Hoang_engine`  
> **Phạm vi phụ trách**: Toàn bộ hệ thống lõi **Engine** (Trích xuất đặc trưng $\to$ Quét luật gian lận $\to$ Dự đoán Machine Learning $\to$ Chấm điểm rủi ro đa nhân tố $\to$ Phân luồng quyết định giao dịch).

---

## 🏗️ 1. Kiến Trúc Luồng Dữ Liệu Engine (End-to-End Pipeline)

```text
[Giao Dịch Mới Đến + Thông Tin Khách Hàng / Lịch Sử]
                       │
                       ▼
    ┌──────────────────────────────────────┐
    │  BƯỚC 1: FeatureExtractor            │  (Tạo Vector đặc trưng 12 chiều:
    │  (features/FeatureExtractor)         │   velocity, amount deviation, geo, ...)
    └──────────────────┬───────────────────┘
                       │
         ┌─────────────┴─────────────┐
         ▼                           ▼
┌───────────────────────────┐ ┌───────────────────────────┐
│ BƯỚC 2: FraudRuleEngine   │ │ BƯỚC 3: NativeMLPredictor │
│ (Bộ 7+ luật gian lận      │ │ (Dự đoán xác suất P_fraud │
│  tất định: IFraudRule)    │ │  qua IModelPredictor)     │
└─────────────┬─────────────┘ └─────────────┬─────────────┘
              │                             │
              │  Vector<FraudAlert>         │  Xác suất ML (0.0 - 1.0)
              └──────────────┬──────────────┘
                             ▼
    ┌──────────────────────────────────────┐
    │  BƯỚC 4: RiskEngine & RiskPolicy     │  (Tổng hợp điểm rủi ro 0 - 100
    │  (risk/RiskEngine)                   │   kèm giải thích từng nhân tố)
    └──────────────────┬───────────────────┘
                       │
                       ▼  RiskAssessment
    ┌──────────────────────────────────────┐
    │  BƯỚC 5: DecisionEngine              │  (Phân luồng hành động: APPROVE,
    │  (decision/DecisionEngine)           │   REVIEW, CHALLENGE_3DS, BLOCK)
    └──────────────────┬───────────────────┘
                       │
                       ▼
[Transaction Cập Nhật Trạng Thái + Ghi Nhật Ký / Đẩy Vào Case Management]
```

---

## 📑 2. Danh Mục Các File Hướng Dẫn Từng Bước

Bộ hướng dẫn được chia thành 6 file markdown tương ứng với từng giai đoạn thực hiện:

| Thứ tự | File hướng dẫn | Nội dung trọng tâm |
| :---: | :--- | :--- |
| **Bước 0** | [00_TONG_QUAN_VA_TIEN_DE.md](file:///d:/Fraud-Detection---Risk-management/docs/engine/00_TONG_QUAN_VA_TIEN_DE.md) | File này: Tổng quan kiến trúc & Hoàn thiện các file domain tiền đề (`Transaction.cpp`, `FraudAlert.cpp`, `RiskAssessment.cpp`, sửa `vector.h`). |
| **Bước 1** | [01_FEATURE_EXTRACTOR.md](file:///d:/Fraud-Detection---Risk-management/docs/engine/01_FEATURE_EXTRACTOR.md) | Cài đặt `FeatureVector` & `FeatureExtractor` (tính vận tốc, độ lệch tiền, khoảng cách địa lý). |
| **Bước 2** | [02_RULE_BASED_ENGINE.md](file:///d:/Fraud-Detection---Risk-management/docs/engine/02_RULE_BASED_ENGINE.md) | Cài đặt Strategy Pattern `IFraudRule`, 7 luật gian lận tất định và `FraudRuleEngine`. |
| **Bước 3** | [03_ML_INTEGRATION.md](file:///d:/Fraud-Detection---Risk-management/docs/engine/03_ML_INTEGRATION.md) | Cài đặt `IModelPredictor`, `NativeMLPredictor` chạy trực tiếp trong C++ và cơ chế Circuit Breaker an toàn. |
| **Bước 4** | [04_RISK_MANAGEMENT_ENGINE.md](file:///d:/Fraud-Detection---Risk-management/docs/engine/04_RISK_MANAGEMENT_ENGINE.md) | Cài đặt `RiskPolicy`, `RiskAggregator`, `RiskEngine` chấm điểm $0-100$ có khả năng giải thích (Explainable). |
| **Bước 5** | [05_DECISION_ENGINE_VA_TEST.md](file:///d:/Fraud-Detection---Risk-management/docs/engine/05_DECISION_ENGINE_VA_TEST.md) | Cài đặt `DecisionEngine`, luật phủ quyết khẩn cấp (Override), cập nhật Build & Test End-to-End. |

---

## 🛠️ 3. Chi Tiết Bước 0: Hoàn Thiện Các File Tiền Đề

Trước khi bắt tay vào viết Engine, các file header của Domain Model phải có file hiện thực `.cpp` tương ứng để trình biên dịch có thể liên kết (link) không bị lỗi.

### 1. Sửa lỗi nhỏ trong `include/DSA/vector.h`
Tại dòng 19 trong `include/DSA/vector.h`:
```cpp
// SỬA:
this->data = new T[this->capacity]; 
// THÀNH:
this->data = new T[this->capacity_];
```

### 2. Tạo `sources/core/domain/transaction/Transaction.cpp`
Hiện thực các hàm của `Transaction.h`:
- Constructor mặc định và Constructor có tham số đầy đủ.
- Toàn bộ Getter/Setter.
- Lifecycle methods:
  - `approve()`: Chuyển status thành `TransactionStatus::APPROVED`.
  - `sendToReview()`: Chuyển status thành `TransactionStatus::WAIT_REVIEW`.
  - `challenge()`: Chuyển status thành `TransactionStatus::VERIFY_OTP`.
  - `reject()`: Chuyển status thành `TransactionStatus::BLOCKED`.
  - `settle()`: Chuyển status thành `TransactionStatus::SUCCESS`.
  - `chargeback()`: Chuyển status thành `TransactionStatus::REFUNDED_FRAUD`.
  - `dispute()`: Chuyển status thành `TransactionStatus::DISPUTED`.
- Business checks:
  - `isCrossBorder(merchant_country)`: So sánh `location.getCountry() != merchant_country`.
  - `isHighValue(threshold)`: `amount >= threshold`.
  - `getSummary()`: Chuỗi tóm tắt `TX [id] | Amount | Status`.

### 3. Tạo `sources/core/domain/alert/FraudAlert.cpp`
Hiện thực các hàm của `FraudAlert.h`:
- Constructor gán các trường `alert_id`, `transaction_id`, `rule_id`, `rule_name`, `category`, `score_contribution`, `reason`, `severity`, `triggered_at`.
- Getters.
- `isCritical()`: `severity == RiskLevel::CRITICAL`.
- `getSummary()`: Chuỗi format `[ALERT] <RuleName> (+Score) - Reason`.

### 4. Tạo `sources/core/domain/alert/RiskAssessment.cpp`
Hiện thực các hàm của `RiskAssessment.h`:
- Constructor và Getters.
- `addAlert(FraudAlert)`: Thêm vào `alerts.push_back(...)`.
- `addReason(std::string)`: Thêm vào `reasons.push_back(...)`.
- State check:
  - `isApproved()`: `decision == DecisionAction::APPROVE`.
  - `isUnderReview()`: `decision == DecisionAction::REVIEW`.
  - `isChallengeRequired()`: `decision == DecisionAction::CHALLENGE_3DS`.
  - `isBlocked()`: `decision == DecisionAction::BLOCK`.
- `getSummary()`: In điểm tổng hợp `combined_score`, mức độ rủi ro, và danh sách vi phạm.

---
👉 **Tiếp tục với Bước 1: [01_FEATURE_EXTRACTOR.md](file:///d:/Fraud-Detection---Risk-management/docs/engine/01_FEATURE_EXTRACTOR.md)**
