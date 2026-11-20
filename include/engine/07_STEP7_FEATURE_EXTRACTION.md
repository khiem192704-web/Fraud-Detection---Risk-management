# 🔍 Bước 7: Module Trích Xuất Đặc Trưng (Feature Extraction) & Hoàn Thiện ML Pipeline

> **File cần tạo & chỉnh sửa**:
> - Header mới: [include/engine/FeatureExtractor.h](file:///d:/study/courses%20in%20university/ki%203/PBL%202/Electronic-Payment-Fraud-Detection---Risk-management%20(1)/include/engine/FeatureExtractor.h)
> - Source mới: [sources/engine/FeatureExtractor.cpp](file:///d:/study/courses%20in%20university/ki%203/PBL%202/Electronic-Payment-Fraud-Detection---Risk-management%20(1)/sources/engine/FeatureExtractor.cpp)
> - Cập nhật Header: [include/engine/RiskEngine.h](file:///d:/study/courses%20in%20university/ki%203/PBL%202/Electronic-Payment-Fraud-Detection---Risk-management%20(1)/include/engine/RiskEngine.h)
> - Cập nhật Source: [sources/engine/RiskEngine.cpp](file:///d:/study/courses%20in%20university/ki%203/PBL%202/Electronic-Payment-Fraud-Detection---Risk-management%20(1)/sources/engine/RiskEngine.cpp)
> - Cập nhật ML Predictor: [sources/engine/NativeMLPredictor.cpp](file:///d:/study/courses%20in%20university/ki%203/PBL%202/Electronic-Payment-Fraud-Detection---Risk-management%20(1)/sources/engine/NativeMLPredictor.cpp)
> - File kiểm thử mới: [tests/test_feature_extraction.cpp](file:///d:/study/courses%20in%20university/ki%203/PBL%202/Electronic-Payment-Fraud-Detection---Risk-management%20(1)/tests/test_feature_extraction.cpp)
>
> **Khái niệm OOP & Thiết kế phần mềm**: Single Responsibility Principle (SRP), Data Pipeline Pattern, Tránh rò rỉ dữ liệu (No Data Leakage), Đồng bộ hóa Đa ngôn ngữ (Python $\leftrightarrow$ C++).  
> **Thời gian dự kiến**: 30 - 45 phút.

---

## 🎯 1. Phân Tích Lỗ Hổng Kiến Trúc Hiện Tại (Từ Nhận Xét Đồ Án)

Trong phiên bản hiện tại của dự án:
1. **Thiếu module chuyển đổi dữ liệu**: Mô hình `NativeMLPredictor` nhận đầu vào là `Vector<double> features` gồm 4 thuộc tính số học:
   - $x_1$: `amount` (Số tiền)
   - $x_2$: `velocity_count` (Tần suất quẹt thẻ trong khoảng thời gian)
   - $x_3$: `geo_distance_km` (Khoảng cách địa lý từ điểm giao dịch tới nhà khách hàng)
   - $x_4$: `is_device_risk` (Cờ cảnh báo thiết bị nghi vấn)
2. **Nghịch lý ở `RiskEngine::assessRisk(...)`**:
   - Hiện tại hàm yêu cầu: `assessRisk(const Transaction& tx, const Customer& customer, double ml_score = 0.0)`.
   - Điểm `ml_score` bị bắt buộc phải **truyền sẵn từ bên ngoài**, nhưng trong toàn bộ code không có bất kỳ dòng nào nhận `Transaction` + `Customer` để trích xuất ra 4 đặc trưng rồi gọi `predictor.predictScore(features)`.
   - Kết quả: `ml_score` luôn rơi vào giá trị mặc định $0.0$, làm mất đi 40% trọng số của mô hình học máy (`policy.weight_ml = 0.40`).

---

## 🗺️ 2. Luồng Xử Lý Chuẩn Sau Khi Hoàn Thiện

```text
       ┌────────────────────────┐       ┌────────────────────────┐
       │   Transaction tx       │       │    Customer customer   │
       └───────────┬────────────┘       └───────────┬────────────┘
                   │                                │
                   └───────────────┬────────────────┘
                                   │
                                   ▼
                   ┌────────────────────────────────┐
                   │    FeatureExtractor            │
                   │  (Trích xuất 4 chiều đặc trưng)│
                   └───────────────┬────────────────┘
                                   │
                                   ▼
                 Vector<double> [amount, velocity, geo_dist, dev_risk]
                                   │
                                   ▼
                   ┌────────────────────────────────┐
                   │    NativeMLPredictor           │
                   │  (Tính Sigmoid trên 4 weights) │
                   └───────────────┬────────────────┘
                                   │
                                   ▼  ml_score (0.0 -> 100.0)
                                   │
   ┌───────────────────────────────┴────────────────────────────────┐
   │                       RiskEngine                               │
   │  - Duyệt các IFraudRule* (HighAmount, Velocity, Device,...)    │
   │  - Kết hợp Rule Score + ML Score + Profile Score               │
   │  - Xuất ra RiskAssessment hoàn chỉnh                           │
   └───────────────────────────────┬────────────────────────────────┘
                                   │
                                   ▼
                   ┌────────────────────────────────┐
                   │       DecisionEngine           │
                   │   APPROVE / CHALLENGE / BLOCK  │
                   └────────────────────────────────┘
```

---

## 🧮 3. Chi Tiết Thuật Toán Trích Xuất 4 Đặc Trưng

### Đặc trưng 1: `amount` (Số tiền giao dịch)
- **Nguồn**: `tx.GetAmount()`.
- **Kiểu**: `double`.
- **Ý nghĩa**: Giá trị giao dịch càng lớn thì nguy cơ tổn thất tài chính càng cao.

### Đặc trưng 2: `velocity_count` (Tần suất quẹt thẻ gần đây)
- **Nguồn**: `Deque<Timestamp>` nội bộ hoặc lịch sử giao dịch.
- **Cách tính**: Đếm số lượng giao dịch của tài khoản/khách hàng trong cửa sổ trượt (mặc định 60 giây).
- **Thuật toán**: 
  1. Lấy mốc thời gian `tx.GetTimestamp()`.
  2. Loại bỏ các giao dịch cũ hơn `tx.GetTimestamp() - window_duration` khỏi đầu `Deque`.
  3. Thêm giao dịch hiện tại vào cuối `Deque`.
  4. Trả về kích thước `sliding_window.size()` (dạng `double`).

### Đặc trưng 3: `geo_distance_km` (Khoảng cách từ điểm giao dịch đến nhà khách hàng)
- **Nguồn**: `tx.GetLocation()` và `customer.getHomeLocation()`.
- **Cách tính**: Gọi hàm Haversine `tx.GetLocation().distanceTo(customer.getHomeLocation())` đã có sẵn trong `Location.cpp`.
- **Xử lý an toàn**: Nếu vị trí không hợp lệ (`!tx.GetLocation().isValid() || !customer.getHomeLocation().isValid()`), trả về giá trị an toàn mặc định `0.0` thay vì để `throw exception`.

### Đặc trưng 4: `is_device_risk` (Cờ rủi ro thiết bị)
- **Nguồn**: `tx.GetDevice()`.
- **Cách tính**: 
  - Nếu `device.GetIs_Virtual() == true` (máy ảo/giả lập) $\to$ Rủi ro.
  - Nếu `device.GetIs_Hacker() == true` (máy root/jailbreak) $\to$ Rủi ro.
  - Nếu `!device.is_Safe()` $\to$ Rủi ro.
  - Trả về `1.0` nếu có bất kỳ dấu hiệu rủi ro nào; ngược lại trả về `0.0`.

---

## 💻 4. Code Mẫu Chi Tiết Module `FeatureExtractor`

### File 1: [include/engine/FeatureExtractor.h](file:///d:/Fraud-Detection---Risk-management/include/engine/FeatureExtractor.h)

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
    // Cửa sổ trượt lưu lịch sử thời gian các giao dịch gần nhất
    Deque<Timestamp> sliding_window;
    std::chrono::seconds window_duration{60}; // Mặc định 60 giây

public:
    explicit FeatureExtractor(std::chrono::seconds duration = std::chrono::seconds(60));

    // Trích xuất vector 4 đặc trưng [amount, velocity_count, geo_distance_km, is_device_risk]
    Vector<double> extract(const Transaction& tx, const Customer& customer);

    // Xóa bộ đệm cửa sổ trượt (phục vụ test phiên mới)
    void reset();

    // Lấy tên các đặc trưng tương ứng theo đúng thứ tự
    Vector<std::string> getFeatureNames() const;

private:
    double extractAmount(const Transaction& tx) const;
    double extractVelocity(const Transaction& tx);
    double extractGeoDistance(const Transaction& tx, const Customer& customer) const;
    double extractDeviceRisk(const Transaction& tx) const;
};
```

---

### File 2: [sources/engine/FeatureExtractor.cpp](file:///d:/Fraud-Detection---Risk-management/sources/engine/FeatureExtractor.cpp)

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

    // 1. Loại bỏ các giao dịch cũ hơn cửa sổ trượt
    while (!sliding_window.empty()) {
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - sliding_window.front()).count();
        if (diff > window_duration.count()) {
            sliding_window.pop_front();
        } else {
            break;
        }
    }

    // 2. Thêm giao dịch hiện tại vào Deque
    sliding_window.push_back(now);

    // 3. Trả về số giao dịch trong cửa sổ trượt hiện tại
    return static_cast<double>(sliding_window.size());
}

double FeatureExtractor::extractGeoDistance(const Transaction& tx, const Customer& customer) const {
    const Location& tx_loc = tx.GetLocation();
    const Location& home_loc = customer.getHomeLocation();

    // Kiểm tra an toàn: nếu một trong hai tọa độ không hợp lệ thì không tính khoảng cách
    if (!tx_loc.isValid() || !home_loc.isValid()) {
        return 0.0;
    }

    try {
        return tx_loc.distanceTo(home_loc);
    } catch (...) {
        return 0.0; // Fallback an toàn nếu có ngoại lệ toán học
    }
}

double FeatureExtractor::extractDeviceRisk(const Transaction& tx) const {
    const Device& dev = tx.GetDevice();
    
    // Nếu thiết bị là máy ảo, hoặc bị hacker can thiệp (root), hoặc không an toàn
    if (dev.GetIs_Virtual() || dev.GetIs_Hacker() || !dev.is_Safe()) {
        return 1.0;
    }
    return 0.0;
}

Vector<double> FeatureExtractor::extract(const Transaction& tx, const Customer& customer) {
    Vector<double> features;
    
    // Đảm bảo TUYỆT ĐỐI đúng thứ tự 4 đặc trưng đồng bộ với model_weights.json và Python!
    features.push_back(extractAmount(tx));          // [0] amount
    features.push_back(extractVelocity(tx));        // [1] velocity_count
    features.push_back(extractGeoDistance(tx, customer)); // [2] geo_distance_km
    features.push_back(extractDeviceRisk(tx));      // [3] is_device_risk

    return features;
}
```

---

## ⚡ 5. Cập Nhật `RiskEngine` Tự Chủ (Đóng Kín Vòng Lặp ML)

Hiện tại `RiskEngine` đòi truyền `ml_score` từ ngoài. Để giải quyết triệt để, ta nâng cấp `RiskEngine` tự sở hữu `FeatureExtractor` và `NativeMLPredictor`:

### Trong [include/engine/RiskEngine.h](file:///d:/Fraud-Detection---Risk-management/include/engine/RiskEngine.h):

```cpp
#include "engine/FeatureExtractor.h"
#include "engine/NativeMLPredictor.h"

class RiskEngine {
private:
    Vector<IFraudRule*> rules;
    RiskPolicy policy;
    FeatureExtractor feature_extractor; // Thêm bộ trích xuất đặc trưng
    NativeMLPredictor ml_predictor;       // Thêm bộ dự đoán học máy

public:
    explicit RiskEngine(const RiskPolicy& custom_policy = RiskPolicy());
    ~RiskEngine();

    void registerRule(IFraudRule* rule);
    int getRuleCount() const;
    void setPolicy(const RiskPolicy& new_policy);
    const RiskPolicy& getPolicy() const;

    // Truy cập trực tiếp để cấu hình hoặc nạp file model_weights.json
    NativeMLPredictor& getMLPredictor() { return ml_predictor; }
    FeatureExtractor& getFeatureExtractor() { return feature_extractor; }

    // HÀM MỚI (Tự chủ hoàn toàn): Tự động trích xuất đặc trưng -> tự tính ML Score -> đánh giá rủi ro
    RiskAssessment assessRisk(const Transaction& tx, const Customer& customer);

    // HÀM CŨ (Giữ lại để tương thích ngược với unit test cũ):
    RiskAssessment assessRisk(const Transaction& tx, const Customer& customer, double ml_score);
};
```

### Trong [sources/engine/RiskEngine.cpp](file:///d:/Fraud-Detection---Risk-management/sources/engine/RiskEngine.cpp):

Thêm hàm tự động tính:
```cpp
RiskAssessment RiskEngine::assessRisk(const Transaction& tx, const Customer& customer) {
    // 1. Trích xuất vector 4 đặc trưng từ Transaction và Customer
    Vector<double> features = feature_extractor.extract(tx, customer);

    // 2. Dự đoán điểm số rủi ro từ mô hình học máy (thang điểm 0.0 -> 100.0)
    double computed_ml_score = ml_predictor.predictScore(features);

    // 3. Tận dụng hàm assessRisk sẵn có với điểm ML vừa tính
    return assessRisk(tx, customer, computed_ml_score);
}
```

---

## 🔄 6. Đồng Bộ Hóa `NativeMLPredictor.cpp` (Chuẩn 4 Đặc Trưng)

Trong [sources/engine/NativeMLPredictor.cpp](file:///d:/Fraud-Detection---Risk-management/sources/engine/NativeMLPredictor.cpp), hàm khởi tạo mặc định hiện tại mới chỉ có 3 đặc trưng. Hãy cập nhật để đồng bộ 4 đặc trưng khớp với [data/model_weights.json](file:///d:/Fraud-Detection---Risk-management/data/model_weights.json):

```cpp
NativeMLPredictor::NativeMLPredictor() {
    // Bộ trọng số mặc định đã hiệu chuẩn cho 4 đặc trưng:
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

## 🧪 7. File Kiểm Thử Tích Hợp `tests/test_feature_extraction.cpp`

Tạo file mới [tests/test_feature_extraction.cpp](file:///d:/Fraud-Detection---Risk-management/tests/test_feature_extraction.cpp) để chứng minh module hoạt động hoàn hảo:

```cpp
#include <iostream>
#include <cassert>
#include "engine/FeatureExtractor.h"
#include "engine/NativeMLPredictor.h"
#include "engine/RiskEngine.h"
#include "engine/rules/DeviceRiskRule.h"
#include "core/domain/Transaction.h"
#include "core/domain/Customer.h"

int main() {
    std::cout << "========================================================\n";
    std::cout << "  KIEM THU MODULE TRICH XUAT DAC TRUNG & ML PIPELINE    \n";
    std::cout << "========================================================\n";

    // 1. Khởi tạo Khách hàng tại Hà Nội (21.0285, 105.854)
    Location home_loc("Vietnam", "Ha Noi", 21.0285, 105.854);
    Customer customer("CUST_001", "Nguyen Van A", "a@gmail.com", "0901234567", home_loc);

    // 2. Giao dịch 1: Bình thường tại Hà Nội, thiết bị an toàn, số tiền 50$
    Device safe_dev("DEV_01", "192.168.1.1", "FP_01", home_loc, false, false);
    Transaction tx1;
    tx1.SetAmount(50.0);
    tx1.SetLocation(home_loc);
    tx1.SetDevice(safe_dev);

    FeatureExtractor extractor;
    Vector<double> feat1 = extractor.extract(tx1, customer);

    std::cout << "\n[Test 1] Giao dich binh thuong tai nha:\n";
    std::cout << "  - Amount: " << feat1[0] << " (Ky vong: 50.0)\n";
    std::cout << "  - Velocity: " << feat1[1] << " (Ky vong: 1.0)\n";
    std::cout << "  - Geo Distance: " << feat1[2] << " km (Ky vong: ~0.0)\n";
    std::cout << "  - Device Risk: " << feat1[3] << " (Ky vong: 0.0)\n";

    assert(feat1[0] == 50.0);
    assert(feat1[1] == 1.0);
    assert(feat1[2] < 1.0);
    assert(feat1[3] == 0.0);
    std::cout << "  -> PASS Test 1!\n";

    // 3. Giao dịch 2: Đột ngột quẹt thẻ tại TP.HCM (cách Hà Nội ~1140 km), số tiền lớn, máy ảo
    Location hcm_loc("Vietnam", "Ho Chi Minh", 10.8231, 106.63);
    Device hacker_dev("DEV_EMU", "10.0.0.1", "FP_ROOT", hcm_loc, true, false); // Máy ảo
    Transaction tx2;
    tx2.SetAmount(2500.0);
    tx2.SetLocation(hcm_loc);
    tx2.SetDevice(hacker_dev);

    Vector<double> feat2 = extractor.extract(tx2, customer);

    std::cout << "\n[Test 2] Giao dich bat thuong (Cach xa 1140km + May ao + Tien lon):\n";
    std::cout << "  - Amount: " << feat2[0] << " (Ky vong: 2500.0)\n";
    std::cout << "  - Velocity: " << feat2[1] << " (Ky vong: 2.0 do quẹt tiếp ngay)\n";
    std::cout << "  - Geo Distance: " << feat2[2] << " km (Ky vong: > 1000 km)\n";
    std::cout << "  - Device Risk: " << feat2[3] << " (Ky vong: 1.0)\n";

    assert(feat2[0] == 2500.0);
    assert(feat2[1] == 2.0);
    assert(feat2[2] > 1000.0);
    assert(feat2[3] == 1.0);
    std::cout << "  -> PASS Test 2!\n";

    // 4. Test tích hợp toàn diện với RiskEngine tự động
    std::cout << "\n[Test 3] Tich hop RiskEngine tu dong goi FeatureExtractor + NativeMLPredictor:\n";
    RiskEngine engine;
    engine.registerRule(new DeviceRiskRule(30.0));

    RiskAssessment asm1 = engine.assessRisk(tx1, customer);
    std::cout << "  - Giao dich 1 ML Score: " << asm1.getMlScore() << " | Tong diem: " << asm1.getCombinedScore() << "\n";

    RiskAssessment asm2 = engine.assessRisk(tx2, customer);
    std::cout << "  - Giao dich 2 ML Score: " << asm2.getMlScore() << " | Tong diem: " << asm2.getCombinedScore() << "\n";

    assert(asm2.getMlScore() > asm1.getMlScore());
    assert(asm2.getCombinedScore() > asm1.getCombinedScore());
    std::cout << "  -> PASS Test 3! ML Score phan hoa ro ret giua an toan va rui ro!\n";

    std::cout << "\n>>> TAT CA KIEM THU THANH CONG 100%! <<<\n";
    return 0;
}
```

---

## 🎓 8. Bộ Câu Hỏi Vấn Đáp Thầy Cô Hay Hỏi Bảo Vệ PBL2

> **Hỏi 1**: *"Tại sao em không viết hàm tính khoảng cách hay tần suất ngay bên trong class `Transaction` mà lại phải tạo riêng class `FeatureExtractor`?"*  
> **Đáp**: *"Dạ thưa thầy/cô, theo nguyên lý **Single Responsibility Principle (SRP)** trong thiết kế hướng đối tượng:
> - Class `Transaction` là một Entity/Domain Model đại diện cho dữ liệu thuần túy của một giao dịch. Nó không nên chứa logic phục vụ riêng cho Machine Learning.
> - Hơn nữa, việc tính khoảng cách đòi hỏi kết hợp giữa `Transaction` và `Customer.home_location`, còn tính tần suất (`velocity_count`) lại cần lưu vết lịch sử theo thời gian (`Sliding Window Deque`).
> - Do đó, tách riêng `FeatureExtractor` giúp code module hóa, độc lập, dễ mở rộng thêm đặc trưng mới mà không sửa đổi cấu trúc dữ liệu lõi."*

> **Hỏi 2**: *"Trong trích xuất đặc trưng cho Machine Learning thời gian thực, làm sao em phòng chống hiện tượng Rò rỉ dữ liệu (Data Leakage)?"*  
> **Đáp**: *"Dạ thưa thầy/cô, trong bài toán phát hiện gian lận, rò rỉ dữ liệu xảy ra khi mô hình sử dụng các thông tin trong tương lai (sau thời điểm giao dịch phát sinh). Để ngăn chặn:
> 1. Trong C++, bộ đệm `sliding_window` của `FeatureExtractor` chỉ cập nhật và tính toán tại đúng thời điểm `tx.GetTimestamp()`, các giao dịch sau thời điểm này chưa xuất hiện nên không bao giờ bị tính lọt vào.
> 2. Phía Python khi huấn luyện trên tập dữ liệu `transactions.csv`, tập train-test được sắp xếp theo chuỗi thời gian (Temporal Split) chứ không shuffle ngẫu nhiên, đảm bảo mô hình không học trước dữ liệu tương lai."*

> **Hỏi 3**: *"Nếu khách hàng chưa cập nhật địa chỉ nhà hoặc thiết bị không có GPS thì hệ thống xử lý như thế nào để không bị crash?"*  
> **Đáp**: *"Dạ thưa thầy/cô, trong hàm `extractGeoDistance()`, em cài đặt cơ chế **Graceful Fallback**: kiểm tra `tx_loc.isValid() && home_loc.isValid()`. Nếu thiếu tọa độ (kinh/vĩ độ bằng 0 hoặc không hợp lệ), hàm sẽ trả về `0.0 km` và bọc trong khối `try-catch`, đảm bảo giao dịch vẫn tiếp tục được thẩm định qua các đặc trưng khác và hệ thống luật mà không bao giờ bị dừng đột ngột."*
