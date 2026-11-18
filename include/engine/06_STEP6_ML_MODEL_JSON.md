# 🧠 Bước 6: Tích Hợp Mô Hình Học Máy & File `model_weights.json`

> **File liên quan**:  
> - File cấu hình trọng số: [data/model_weights.json](file:///d:/Fraud-Detection---Risk-management/data/model_weights.json)  
> - Script Python huấn luyện: [python/train_model.py](file:///d:/Fraud-Detection---Risk-management/python/train_model.py)  
> - C++ nạp mô hình: [sources/engine/NativeMLPredictor.cpp](file:///d:/Fraud-Detection---Risk-management/sources/engine/NativeMLPredictor.cpp)  
> - C++ khai báo: [include/engine/NativeMLPredictor.h](file:///d:/Fraud-Detection---Risk-management/include/engine/NativeMLPredictor.h)  
> **Khái niệm cốt lõi**: **Offline Training $\to$ Online Inference**, Cầu nối liên ngôn ngữ Python $\to$ C++, Bóc tách cấu trúc JSON thuần C++.

---

## 🎯 1. Bản Chất Kiến Trúc: Tại Sao Cần `model_weights.json`?

Trong các hệ thống thanh toán lớn (như Visa, MoMo, VNPay), thời gian phản hồi cho mỗi giao dịch bắt buộc phải **$< 50\text{ms}$**:
- **Nếu gọi trực tiếp Python (qua REST API / Flask / Socket / IPC)**: Mỗi lần quẹt thẻ phải gửi dữ liệu sang Python $\to$ Độ trễ mạng (Network Overhead) mất $50 - 150\text{ms}$, hệ thống sẽ bị nghẽn cổ chai khi có hàng nghìn giao dịch/giây.
- **Nếu nhúng ONNX Runtime / LibTorch C++**: Thư viện nặng hàng trăm MB, phức tạp khi cấu hình build CMake cho đồ án.
- **Giải pháp tối ưu — Kiến trúc Native Embedded ML**:
  ```text
  ┌─────────────────────────┐          ┌─────────────────────────┐
  │   PYTHON (Offline ML)   │          │    C++ (Online Engine)  │
  │                         │          │                         │
  │  data/transactions.csv  │          │  NativeMLPredictor      │
  │           │             │          │           ▲             │
  │      Huấn luyện         │          │           │ Đọc nạp     │
  │  Logistic Regression    │          │           │             │
  │           │             │          │           │             │
  │     Xuất trọng số       │ ───────► │  data/model_weights.json│
  │   w1, w2, w3, w4, bias  │          │                         │
  └─────────────────────────┘          └─────────────────────────┘
  ```
  1. **Python**: Đảm nhận việc nặng (huấn luyện mô hình trên dữ liệu lớn, tính toán ma trận, tìm trọng số tối ưu).
  2. **File JSON (`model_weights.json`)**: Đóng vai trò là chiếc "cầu nối dữ liệu".
  3. **C++ (`NativeMLPredictor`)**: Chỉ cần đọc các số thực từ JSON và tính toán bằng phép nhân cộng vô hướng ($z = \text{bias} + \sum w_i x_i$) và hàm Sigmoid. Tốc độ suy luận đạt mức **nanosecond ($< 0.5\mu\text{s}$)**!

---

## 📋 2. Cấu Trúc File `data/model_weights.json`

File [data/model_weights.json](file:///d:/Fraud-Detection---Risk-management/data/model_weights.json) có cấu trúc chuẩn như sau:

```json
{
    "description": "Calibrated Logistic Regression Weights for EPFD-RAS",
    "features": [
        "amount",
        "velocity_count",
        "geo_distance_km",
        "is_device_risk"
    ],
    "weights": [
        0.0018,
        1.42,
        0.0035,
        2.15
    ],
    "bias": -3.2,
    "optimal_threshold": 0.5
}
```

### Ý nghĩa các trường:
| Trường | Kiểu dữ liệu | Ý nghĩa |
| :--- | :---: | :--- |
| `features` | Array of String | Tên 4 đặc trưng đầu vào tương ứng với vector dữ liệu |
| `weights` | Array of Float | Vector trọng số $[w_1, w_2, w_3, w_4]$ tương ứng cho từng đặc trưng |
| `bias` | Float | Hệ số chặn tự do ($w_0 = -3.2$) |
| `optimal_threshold`| Float | Ngưỡng xác suất tối ưu để phân loại gian lận (mặc định $0.5$) |

### Ánh xạ sang công thức toán học trong C++:
$$z = (-3.2) + (0.0018 \times \text{amount}) + (1.42 \times \text{velocity}) + (0.0035 \times \text{distance}) + (2.15 \times \text{device\_risk})$$
$$P(\text{fraud}) = \frac{1}{1 + e^{-z}}$$
$$\text{ML Score} = P(\text{fraud}) \times 100.0$$

---

## 🛠️ 3. Lộ Trình Triển Khai Bước 6 (Gồm 2 Phần)

### 🐍 Phần 1: Viết Script Python Xuất File JSON (`python/train_model.py`)
Mục tiêu: Đọc `data/transactions.csv`, huấn luyện Logistic Regression và xuất file `model_weights.json`.

Các bước `TODO`:
- **TODO 1**: Đọc dữ liệu `transactions.csv` bằng thư viện `pandas`.
- **TODO 2**: Chuẩn bị tập đặc trưng $X$ gồm 4 cột (`amount`, `velocity_count`, `geo_distance_km`, `is_device_risk`) và nhãn $y$ (`is_fraud`).
- **TODO 3**: Khởi tạo và huấn luyện mô hình `LogisticRegression(class_weight='balanced')`.
- **TODO 4**: Trích xuất `weights = model.coef_[0]` và `bias = model.intercept_[0]`.
- **TODO 5**: Ghi kết quả vào file [data/model_weights.json](file:///d:/Fraud-Detection---Risk-management/data/model_weights.json).

---

### ⚡ Phần 2: Cập Nhật Hàm `loadModel` Trong C++ (`sources/engine/NativeMLPredictor.cpp`)
Mục tiêu: Đọc file JSON từ đĩa cứng và nạp lại các trọng số vào bộ nhớ mà không cần thư viện bên ngoài phức tạp.

Các bước `TODO`:
- **TODO 1**: Mở file cấu hình bằng `std::ifstream file(filepath);`.
- **TODO 2**: Đọc nội dung file, tìm chuỗi `"weights": [` để trích xuất các trọng số thực nạp vào `weights`.
- **TODO 3**: Tìm chuỗi `"bias":` để đọc giá trị số thực gán vào biến `bias`.
- **TODO 4**: Đặt cờ `ready = true;` và trả về `true`.

---

## 🎓 4. Câu Hỏi Vấn Đáp Thầy Cô Hay Hỏi Về Bước Này

> **Hỏi**: *"Tại sao em không huấn luyện trực tiếp mô hình ngay trong C++ mà lại dùng Python?"*  
> **Đáp**: *"Dạ thưa thầy/cô, trong thực tế công nghiệp (Machine Learning Engineering), ngôn ngữ Python sở hữu hệ sinh thái khoa học dữ liệu mạnh mẽ nhất (Scikit-Learn, Pandas, NumPy) để tiền xử lý, phân tích ma trận tương quan và tối ưu hóa hàm mất mát. Còn C++ lại có thế mạnh vượt trội về tốc độ thực thi, quản lý bộ nhớ và tính tất định. Do đó, việc tách biệt: **Python đảm nhận khâu Huấn luyện ngoại tuyến (Offline Training)** và **C++ đảm nhận khâu Dự đoán trực tuyến (Online Inference)** thông qua file trung gian `model_weights.json` là kiến trúc chuẩn mực (Best Practice), vừa linh hoạt vừa đạt hiệu năng cao nhất."*

> **Hỏi**: *"Nếu file JSON bị lỗi hoặc bị ai đó xóa mất thì hệ thống có bị sập (crash) không?"*  
> **Đáp**: *"Dạ không. Lớp `NativeMLPredictor` được thiết kế theo cơ chế **Fallback / Circuit Breaker**: Nếu hàm `loadModel()` đọc file thất bại, Engine sẽ tự động giữ lại bộ trọng số mặc định đã được hiệu chuẩn sẵn trong mã nguồn hoặc trả về điểm rủi ro trung tính an toàn (0.15) để hệ thống tiếp tục hoạt động bình thường mà không bị crash."*
