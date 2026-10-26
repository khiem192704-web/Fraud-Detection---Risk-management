# 🤖 Bước 3: Tích Hợp Dự Đoán Học Máy (Machine Learning Predictor)

> **Mục tiêu**: Xây dựng tầng Adapter cách ly mô hình học máy (`IModelPredictor`), cho phép động cơ C++ suy luận xác suất gian lận $P(\text{fraud}) \in [0.0, 1.0]$ ở cấp độ microsecond mà không bị ràng buộc phụ thuộc phức tạp, đồng thời trang bị cơ chế bảo vệ an toàn (Circuit Breaker).  
> **Thư mục làm việc**: `include/engine/ml/` và `sources/engine/ml/`.

---

## 🔌 1. Thiết Kế Giao Diện `IModelPredictor`

Theo đặc tả [`docs/15_ML_CPP_INTEGRATION.md`](file:///d:/Fraud-Detection---Risk-management/docs/15_ML_CPP_INTEGRATION.md), tầng C++ cần dùng Interface thuần ảo để có thể hoán đổi linh hoạt giữa các bản cài đặt (Mock Predictor $\to$ Native Predictor $\to$ ONNX Runtime).

### File: `include/engine/ml/IModelPredictor.h`
```cpp
#pragma once
#include <string>
#include "engine/features/FeatureVector.h"

class IModelPredictor {
public:
    virtual ~IModelPredictor() = default;

    // Tên và phiên bản mô hình (vd: "EPFD_Calibrated_Logistic_v1.0")
    virtual std::string getModelName() const = 0;
    virtual std::string getModelVersion() const = 0;

    /**
     * @brief Dự đoán xác suất gian lận từ vector đặc trưng.
     * @param features Vector đặc trưng đã chuẩn bị từ FeatureExtractor.
     * @return Xác suất gian lận trong đoạn [0.0, 1.0].
     */
    virtual double predictProbability(const FeatureVector& features) = 0;

    // Kiểm tra trạng thái mô hình có sẵn sàng phục vụ hay không
    virtual bool isReady() const = 0;
};
```

---

## ⚡ 2. Cài Đặt Bộ Dự Đoán Thuần C++ (`NativeMLPredictor`)

Để hệ thống chạy mượt mà ngay lập tức mà không cần cài đặt các thư viện C++ nặng như ONNX / LibTorch, ta hiện thực `NativeMLPredictor` sử dụng mô hình tuyến tính đã hiệu chuẩn xác suất (Calibrated Sigmoid/Logistic Regression) dựa trên các trọng số học máy.

$$z = w_0 + \sum_{i=1}^{n} w_i \cdot x_i$$
$$P(\text{fraud}) = \sigma(z) = \frac{1}{1 + e^{-z}}$$

### File: `include/engine/ml/NativeMLPredictor.h`
```cpp
#pragma once
#include "IModelPredictor.h"
#include <cmath>

class NativeMLPredictor : public IModelPredictor {
private:
    std::string model_name{"Native_Calibrated_Logistic"};
    std::string model_version{"1.0.0"};
    bool ready{true};

    // Các trọng số mô hình đã được tối ưu hóa sẵn (Weights)
    double bias{-2.8};                          // w0 (độ lệch cơ sở)
    double w_amount_ratio{0.45};                // Trọng số cho độ lệch số tiền
    double w_velocity_5m{0.85};                 // Trọng số cho tần suất 5 phút
    double w_new_device{1.20};                  // Trọng số cho thiết bị mới
    double w_virtual_device{1.60};              // Trọng số cho môi trường giả lập
    double w_compromised_device{2.50};          // Trọng số cho thiết bị root/hacker
    double w_foreign_country{0.75};             // Trọng số cho giao dịch quốc tế
    double w_speed_travel{0.003};               // Trọng số cho tốc độ di chuyển (km/h)
    double w_unusual_hour{0.35};                // Trọng số cho giao dịch ban đêm
    double w_merchant_risk{0.025};              // Trọng số cho rủi ro Merchant

public:
    NativeMLPredictor() = default;

    std::string getModelName() const override { return model_name; }
    std::string getModelVersion() const override { return model_version; }
    bool isReady() const override { return ready; }

    void setReady(bool state) { ready = state; }

    double predictProbability(const FeatureVector& fv) override;
};
```

### File: `sources/engine/ml/NativeMLPredictor.cpp`
```cpp
#include "engine/ml/NativeMLPredictor.h"
#include <algorithm>

double NativeMLPredictor::predictProbability(const FeatureVector& fv) {
    if (!ready) {
        // Fallback an toàn nếu mô hình chưa sẵn sàng
        return 0.15; // Mức xác suất mặc định trung tính
    }

    // 1. Tính tổng tuyến tính z
    double z = bias;
    z += w_amount_ratio * std::min(fv.amount_ratio_to_avg, 10.0);
    z += w_velocity_5m * std::min(static_cast<double>(fv.tx_count_last_5min), 8.0);
    z += (fv.is_new_device ? w_new_device : 0.0);
    z += (fv.is_device_virtual ? w_virtual_device : 0.0);
    z += (fv.is_device_compromised ? w_compromised_device : 0.0);
    z += (fv.is_foreign_country ? w_foreign_country : 0.0);
    z += w_speed_travel * std::min(fv.speed_from_last_tx_kmh, 2000.0);
    z += (fv.is_unusual_hour ? w_unusual_hour : 0.0);
    z += w_merchant_risk * fv.merchant_risk_score;

    // 2. Hàm kích hoạt Sigmoid đưa về xác suất [0.0, 1.0]
    double probability = 1.0 / (1.0 + std::exp(-z));

    // Đảm bảo chặn biên an toàn
    return std::max(0.0, std::min(1.0, probability));
}
```

---

## 🛡️ 3. Cơ Chế Chống Chịu Lỗi (Circuit Breaker / Fail-Safe Policy)

Theo quy định kiến trúc bảo mật: **Hệ thống không bao giờ được phép dừng hoạt động hay bỏ qua kiểm soát rủi ro khi ML gặp sự cố.**

### Thiết kế lớp bọc `SafeMLPredictor`
- Nếu `IModelPredictor::predictProbability` ném ra ngoại lệ hoặc timeout:
  - Ghi log cảnh báo `[ML_FAILURE] Circuit breaker triggered`.
  - Kích hoạt chính sách **Fail-Safe**: Trả về xác suất dự phòng an toàn (`0.50` - rủi ro mức vừa) để buộc hệ thống phải đánh giá dựa trên luật gian lận.

---

## 🎯 4. Tiêu Chuẩn Nghiệm Thu Bước 3
- [ ] Giao diện `IModelPredictor.h` tách biệt hoàn toàn với logic tính rủi ro.
- [ ] `NativeMLPredictor` cho kết quả:
  - Giao dịch bình thường (Alice): $P(\text{fraud}) \approx 2\% - 6\%$ ($0.02 - 0.06$).
  - Giao dịch tấn công (thiết bị ảo, tốc độ $2000\text{ km/h}$): $P(\text{fraud}) \ge 90\%$ ($> 0.90$).
- [ ] Thời gian suy luận C++ $< 1\mu\text{s}$ (đáp ứng tiêu chí $165,000+\text{ TPS}$).

---
👉 **Tiếp tục với Bước 4: [04_RISK_MANAGEMENT_ENGINE.md](file:///d:/Fraud-Detection---Risk-management/docs/engine/04_RISK_MANAGEMENT_ENGINE.md)**
