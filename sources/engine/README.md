# 📘 Hướng Dẫn Hiện Thực Triển Khai (`sources/engine/`)

Thư mục này chứa toàn bộ mã nguồn hiện thực logic thuật toán chi tiết (`.cpp`) của phân hệ **Fraud Detection & Risk Management Engine**.

---

## 📌 1. Danh Mục Các File Cần Code & File Header Đối Ứng

| Bước | File Hiện Thực (`.cpp`) | File Khai Báo (`.h`) | Tài Liệu Lý Thuyết | Trọng Tâm Thuật Toán / OOP |
| :---: | :--- | :--- | :--- | :--- |
| **2.1** | [rules/DeviceRiskRule.cpp](file:///d:/Fraud-Detection---Risk-management/sources/engine/rules/DeviceRiskRule.cpp) | [include/engine/rules/DeviceRiskRule.h](file:///d:/Fraud-Detection---Risk-management/include/engine/rules/DeviceRiskRule.h) | [02_STEP2_CONCRETE_RULES.md](file:///d:/Fraud-Detection---Risk-management/include/engine/02_STEP2_CONCRETE_RULES.md) | Kế thừa `IFraudRule`, kiểm tra cờ thiết bị ảo/root |
| **2.2** | [rules/HighAmountRule.cpp](file:///d:/Fraud-Detection---Risk-management/sources/engine/rules/HighAmountRule.cpp) | [include/engine/rules/HighAmountRule.h](file:///d:/Fraud-Detection---Risk-management/include/engine/rules/HighAmountRule.h) | [02_STEP2_CONCRETE_RULES.md](file:///d:/Fraud-Detection---Risk-management/include/engine/02_STEP2_CONCRETE_RULES.md) | Ngưỡng trần tuyệt đối & hệ số đột biến số tiền |
| **2.3** | [rules/BlacklistRule.cpp](file:///d:/Fraud-Detection---Risk-management/sources/engine/rules/BlacklistRule.cpp) | [include/engine/rules/BlacklistRule.h](file:///d:/Fraud-Detection---Risk-management/include/engine/rules/BlacklistRule.h) | [02_STEP2_CONCRETE_RULES.md](file:///d:/Fraud-Detection---Risk-management/include/engine/02_STEP2_CONCRETE_RULES.md) | **DSA Bảng băm (HashMap)** tra cứu IP/Account $O(1)$ |
| **2.4** | [rules/VelocityRule.cpp](file:///d:/Fraud-Detection---Risk-management/sources/engine/rules/VelocityRule.cpp) | [include/engine/rules/VelocityRule.h](file:///d:/Fraud-Detection---Risk-management/include/engine/rules/VelocityRule.h) | [02_STEP2_CONCRETE_RULES.md](file:///d:/Fraud-Detection---Risk-management/include/engine/02_STEP2_CONCRETE_RULES.md) | **DSA Hàng đợi 2 đầu (Deque)** Cửa sổ trượt thời gian |
| **3** | [NativeMLPredictor.cpp](file:///d:/Fraud-Detection---Risk-management/sources/engine/NativeMLPredictor.cpp) | [include/engine/NativeMLPredictor.h](file:///d:/Fraud-Detection---Risk-management/include/engine/NativeMLPredictor.h) | [03_STEP3_NATIVE_ML_PREDICTOR.md](file:///d:/Fraud-Detection---Risk-management/include/engine/03_STEP3_NATIVE_ML_PREDICTOR.md) | Mô hình Logistic Regression, hàm kích hoạt Sigmoid |
| **4** | [RiskEngine.cpp](file:///d:/Fraud-Detection---Risk-management/sources/engine/RiskEngine.cpp) | [include/engine/RiskEngine.h](file:///d:/Fraud-Detection---Risk-management/include/engine/RiskEngine.h) | [04_STEP4_RISK_ENGINE.md](file:///d:/Fraud-Detection---Risk-management/include/engine/04_STEP4_RISK_ENGINE.md) | Nhạc trưởng điều phối (Polymorphism `rules`), tính điểm tổng hợp |
| **5** | [DecisionEngine.cpp](file:///d:/Fraud-Detection---Risk-management/sources/engine/DecisionEngine.cpp) | [include/engine/DecisionEngine.h](file:///d:/Fraud-Detection---Risk-management/include/engine/DecisionEngine.h) | [05_STEP5_DECISION_ENGINE.md](file:///d:/Fraud-Detection---Risk-management/include/engine/05_STEP5_DECISION_ENGINE.md) | Ra quyết định nghiệp vụ, Phủ quyết khẩn cấp (Emergency Override) |

---

## 🎯 2. Cách Học & Luyện Gõ Code Từng Bước

Mỗi file `.cpp` đều đã được dựng sẵn khung sườn các hàm với định dạng:
1. **`// TODO n: ...`**: Giải thích logic nghiệp vụ, thứ tự các bước cần xử lý trong hàm.
2. **`// Gợi ý: ...`**: Đoạn mã C++ mẫu chuẩn xác để bạn tham khảo.
3. **Khoảng trống**: Vùng để bạn tự tay gõ lại code theo gợi ý.

### Quy trình đề xuất:
- **Bước 1**: Đọc phần giải thích trong `TODO` để nắm tư duy thuật toán.
- **Bước 2**: Đọc kỹ phần `Gợi ý` để hiểu cú pháp C++ và cách tương tác với các đối tượng Domain (`Transaction`, `Customer`, `FraudAlert`,...).
- **Bước 3**: Tự tay gõ lại code vào phần thân hàm bên dưới.

---

## 🛠️ 3. Lệnh Biên Dịch & Chạy Kiểm Thử

Sau khi bạn hoàn thành code cho từng file, hãy mở terminal chạy lệnh test để kiểm tra tính đúng đắn:

```powershell
# Biên dịch và chạy bộ test từng bước:
mingw32-make test

# Hoặc biên dịch thủ công bằng g++:
g++ -std=c++17 -Iinclude -o bin/test_rules_step.exe tests/test_rules_step_by_step.cpp sources/engine/rules/DeviceRiskRule.cpp sources/core/common/Types.cpp sources/core/domain/transaction/transaction.cpp sources/core/domain/device/device.cpp sources/core/domain/location/location.cpp sources/core/domain/alert/FraudAlert.cpp sources/core/domain/alert/RiskAssessment.cpp sources/core/domain/account/Customer.cpp

# Chạy file thực thi kiểm thử:
./bin/test_rules_step.exe
```

> [!TIP]
> Khi code xong mỗi luật mới (như `HighAmountRule.cpp`, `BlacklistRule.cpp`, `VelocityRule.cpp`), bạn hãy thêm file `.cpp` đó vào biến `SRCS_ENGINE` trong file [Makefile](file:///d:/Fraud-Detection---Risk-management/Makefile) để bộ kiểm thử tự động nạp vào!
