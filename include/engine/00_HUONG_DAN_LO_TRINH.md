# 🗺️ Lộ Trình Tự Code Phân Hệ Engine (PBL 2)

> **Mục tiêu**: Tự tay triển khai toàn bộ động cơ thẩm định rủi ro và ra quyết định thanh toán điện tử. Hiểu rõ từng dòng code, áp dụng chuẩn xác mẫu thiết kế **Strategy Pattern** (OOP), cấu trúc dữ liệu **Deque, HashMap, Vector** (DSA) và mô hình lai **Hybrid Engine** (Rules + ML).

---

## 📌 Đồ thị phụ thuộc & Thứ tự thực hiện

```text
[Bước 1: Hợp đồng cơ sở]
      IFraudRule.h
           │
           ▼
[Bước 2: Cài đặt 4 Luật cụ thể]
 ┌─────────┼──────────────────────────┬────────────────────────┐
 │ 2.1     │ 2.2                      │ 2.3                    │ 2.4
 ▼         ▼                          ▼                        ▼
DeviceRisk HighAmountRule             BlacklistRule            VelocityRule
 (Cờ máy)  (Ngưỡng tiền)              (DSA: HashMap O(1))      (DSA: Deque Sliding Window)
           │
           ▼
[Bước 3: Dự đoán học máy] ─────────► NativeMLPredictor.h (Toán Sigmoid độc lập)
           │
           ▼
[Bước 4: Nhạc trưởng chấm điểm] ───► RiskEngine.h (Gom Rules + ML -> Điểm 0-100)
           │
           ▼
[Bước 5: Ra quyết định hành động] ─► DecisionEngine.h (APPROVE / REVIEW / BLOCK)
```

---

## 📂 Danh mục các file hướng dẫn chi tiết trong thư mục này

| Bước | File Hướng Dẫn | Đối Tượng Code | Khái Niệm Cốt Lõi |
| :---: | :--- | :--- | :--- |
| **00** | [00_HUONG_DAN_LO_TRINH.md](file:///d:/Fraud-Detection---Risk-management/include/engine/00_HUONG_DAN_LO_TRINH.md) | Toàn hệ thống | Tổng quan luồng dữ liệu & kiến trúc |
| **01** | [01_STEP1_IFRAUD_RULE.md](file:///d:/Fraud-Detection---Risk-management/include/engine/01_STEP1_IFRAUD_RULE.md) | `rules/IFraudRule.h` | Strategy Pattern, Thuần ảo, Đa hình |
| **02** | [02_STEP2_CONCRETE_RULES.md](file:///d:/Fraud-Detection---Risk-management/include/engine/02_STEP2_CONCRETE_RULES.md) | `rules/*.h` & `.cpp` | DSA Deque (Sliding Window), DSA HashMap |
| **03** | [03_STEP3_NATIVE_ML_PREDICTOR.md](file:///d:/Fraud-Detection---Risk-management/include/engine/03_STEP3_NATIVE_ML_PREDICTOR.md) | `NativeMLPredictor.h` | Mô hình Lai (Hybrid), Hàm Sigmoid |
| **04** | [04_STEP4_RISK_ENGINE.md](file:///d:/Fraud-Detection---Risk-management/include/engine/04_STEP4_RISK_ENGINE.md) | `RiskEngine.h` & `.cpp` | Tổng hợp rủi ro, Explainable Score |
| **05** | [05_STEP5_DECISION_ENGINE.md](file:///d:/Fraud-Detection---Risk-management/include/engine/05_STEP5_DECISION_ENGINE.md) | `DecisionEngine.h` & `.cpp`| Chính sách quyết định, Emergency Override |

---

## 🛠️ Nguyên tắc phân chia Header (`.h`) và Implementation (`.cpp`)

1. **Thư mục Header (`include/engine/`)**: Chỉ chứa định nghĩa lớp, khai báo hàm, struct chính sách (`RiskPolicy`, `DecisionPolicy`).
2. **Thư mục Implementation (`sources/engine/`)**: Chứa code hiện thực logic thuật toán chi tiết:
   - `sources/engine/rules/DeviceRiskRule.cpp`
   - `sources/engine/rules/HighAmountRule.cpp`
   - `sources/engine/rules/BlacklistRule.cpp`
   - `sources/engine/rules/VelocityRule.cpp`
   - `sources/engine/RiskEngine.cpp`
   - `sources/engine/DecisionEngine.cpp`
   - `sources/engine/NativeMLPredictor.cpp`

---

## 💡 Lời khuyên trước khi bắt đầu

- **Đừng vội code file tổng hợp ngay**: Hãy code và test độc lập từng luật nhỏ trước.
- **Tập trung vào câu hỏi phản biện**: Trong mỗi file hướng dẫn đều có mục *"Câu hỏi thầy cô hay hỏi bảo vệ PBL2"*, hãy đọc kỹ để hiểu tại sao thiết kế như vậy.

