# 🐍 Phân Hệ Machine Learning Python (`python/`)

Thư mục này chứa các kịch bản (scripts) phân tích dữ liệu, tiền xử lý và huấn luyện mô hình học máy phát hiện gian lận thanh toán (Offline Training Pipeline).

---

## 📂 Danh Mục Tập Tin

| File | Mô Tả | Đầu Vào | Đầu Ra |
| :--- | :--- | :--- | :--- |
| [train_model.py](file:///d:/Fraud-Detection---Risk-management/python/train_model.py) | Script huấn luyện Logistic Regression | [data/transactions.csv](file:///d:/Fraud-Detection---Risk-management/data/transactions.csv) | [data/model_weights.json](file:///d:/Fraud-Detection---Risk-management/data/model_weights.json) |

---

## 🚀 Cách Chạy Huấn Luyện & Xuất JSON

1. Đảm bảo môi trường máy đã cài đặt Python 3 cùng các thư viện:
   ```powershell
   pip install pandas scikit-learn numpy
   ```

2. Chạy script huấn luyện từ thư mục gốc của dự án:
   ```powershell
   python python/train_model.py
   ```

3. Sau khi chạy thành công, file [data/model_weights.json](file:///d:/Fraud-Detection---Risk-management/data/model_weights.json) sẽ được cập nhật bộ trọng số tối ưu mới nhất để lớp C++ `NativeMLPredictor` đọc vào.
