# KẾ HOẠCH PHÁT TRIỂN & PHÂN CÔNG ĐỒ ÁN PBL 2 (HẠN 2 TUẦN)
> **Đề tài**: Hệ Thống Phát Hiện Gian Lận & Quản Trị Rủi Ro Thanh Toán Điện Tử (EPFD-RAS)  
> **Môn học**: PBL 2 — Đồ Án Cơ Sở Lập Trình (Khoa CNTT - Trường ĐH Bách Khoa, ĐH Đà Nẵng)  
> **Thời gian thực hiện**: 14 Ngày (2 Tuần)  
> **Nhóm thực hiện**: 2 Thành viên (Hoàng & Khiêm)

---

## 🎯 1. MỤC TIÊU VÀ NGUYÊN TẮC CỐT LÕI

1. **Hiểu bản chất 100% mã nguồn**: Tự tay chọn lọc, tinh gọn hóa từ thư mục gốc sang thư mục `(1)`, loại bỏ triệt để các module doanh nghiệp rườm rà không thuộc phạm vi đồ án.
2. **Bám sát tiêu chí chấm điểm PBL 2**:
   - **Lập trình Hướng đối tượng (OOP)**: Đóng gói thực thể, Kế thừa, Đa hình (Polymorphism) thông qua Strategy Pattern (`IFraudRule`).
   - **Cấu trúc dữ liệu & Giải thuật (DSA)**: Tự cài đặt và ứng dụng thực tế `Vector`, `Deque` (Sliding Window), `PriorityQueue` (Top-K Risk Heap), `HashMap`.
   - **Mô hình Lai (Hybrid Engine)**: Kết hợp luật nghiệp vụ tất định với mô hình học máy (Machine Learning) nhẹ nhàng từ Python.
3. **Sản phẩm chạy được + Báo cáo hoàn chỉnh**: Có kịch bản demo trực quan và hoàn thiện đầy đủ 5 chương trong file mẫu `PBL2.doc`.

---

## 👥 2. PHÂN CÔNG TRÁCH NHIỆM RÕ RÀNG

| Hạng mục | Thành viên 1: Phụ trách C++ Core & DSA | Thành viên 2: Phụ trách Python ML & Dữ liệu |
| :--- | :--- | :--- |
| **Vai trò chính** | Xây dựng khung xương sống hệ thống, thuật toán và logic nghiệp vụ. | Xử lý dữ liệu, huấn luyện mô hình học máy và xuất chỉ số đánh giá. |
| **Nhiệm vụ Tuần 1** | - Sửa lỗi biên dịch, hoàn thiện Domain Models (`Account`, `Transaction`,...).<br>- Hoàn thiện DSA tự viết (`Vector`, `Deque`, `PriorityQueue`).<br>- Xây dựng `IFraudRule` và 4 luật gian lận (Strategy Pattern).<br>- Xây dựng `RiskEngine` (tính điểm $0-100$) và `DecisionEngine`. | - Cài đặt môi trường Python (`pandas`, `scikit-learn`, `matplotlib`).<br>- Viết script `generate_data.py` tạo 5,000 giao dịch giả lập (.csv).<br>- Trích xuất đặc trưng (Feature Engineering: amount, velocity, geo-distance).<br>- Huấn luyện mô hình Logistic Regression / Random Forest. |
| **Nhiệm vụ Tuần 2** | - Xây dựng `TransactionService` khép kín.<br>- Nhúng class `NativeMLPredictor` đọc trọng số từ Python.<br>- Dựng giao diện Console trực quan (có màu sắc, menu kịch bản demo).<br>- Soạn nội dung Chương 3 (DSA) và Chương 4 (OOP) trong Báo cáo. | - Vẽ biểu đồ đánh giá: ROC Curve, Confusion Matrix, Feature Importance.<br>- Xuất file trọng số `model_weights.json` gửi cho C++.<br>- Soạn nội dung Chương 1 (Tổng quan) và Chương 4 (Phần ML) trong Báo cáo.<br>- Thiết kế Slide PowerPoint (15-18 trang) cho buổi bảo vệ. |

---

## 📂 3. CẤU TRÚC THƯ MỤC CHUẨN CỦA DỰ ÁN `(1)`

```text
Electronic-Payment-Fraud-Detection---Risk-management (1)/
├── CMakeLists.txt                      # Cấu hình biên dịch toàn dự án
├── Makefile                            # Build nhanh bằng MinGW make
├── README.md                           # Giới thiệu & hướng dẫn chạy ứng dụng
│
├── include/                            # TOÀN BỘ FILE HEADER (.h)
│   ├── common/
│   │   ├── Types.h                     # Enum: TransactionStatus, RiskLevel, DecisionAction...
│   │   └── Constants.h                 # Các ngưỡng số tiền, hạn mức, trọng số rủi ro
│   │
│   ├── domain/                         # [OOP DOMAIN - Các đối tượng nghiệp vụ]
│   │   ├── Account.h                   # Tài khoản ngân hàng, số dư
│   │   ├── Customer.h                  # Khách hàng, phân tầng KYC
│   │   ├── Transaction.h               # Thông tin giao dịch thanh toán
│   │   ├── PaymentMethod.h             # Thẻ/Ví, che số PAN (4111****1234), thuật toán Luhn
│   │   ├── Device.h                    # Thiết bị (IP, cờ rooted, cờ emulator)
│   │   ├── Location.h                  # Tọa độ GPS, tính khoảng cách di chuyển
│   │   └── FraudAlert.h                # Cảnh báo vi phạm luật
│   │
│   ├── dsa/                            # [DSA CORE - Cấu trúc dữ liệu tự viết tay]
│   │   ├── Vector.h                    # Mảng động lưu trữ lịch sử giao dịch
│   │   ├── Deque.h                     # Cửa sổ trượt (Sliding Window) cho kiểm tra tần suất
│   │   ├── PriorityQueue.h             # Heap ưu tiên lọc Top giao dịch nguy hiểm nhất
│   │   └── HashMap.h                   # Bảng băm tra cứu danh sách đen O(1)
│   │
│   ├── engine/                         # [OOP PATTERNS - Động cơ thẩm định rủi ro]
│   │   ├── rules/
│   │   │   ├── IFraudRule.h            # Interface cơ sở (Strategy Pattern)
│   │   │   ├── VelocityRule.h          # Luật spam quẹt thẻ liên tục (Dùng Deque)
│   │   │   ├── HighAmountRule.h        # Luật số tiền đột biến so với tài khoản
│   │   │   ├── BlacklistRule.h         # Luật thẻ/IP trong danh sách đen (Dùng HashMap)
│   │   │   └── DeviceRiskRule.h        # Luật môi trường thiết bị độc hại
│   │   │
│   │   ├── RiskEngine.h                # Thu thập vi phạm -> Tính điểm rủi ro tổng hợp (0-100)
│   │   ├── DecisionEngine.h            # Phân loại hành động (APPROVE / REVIEW / BLOCK)
│   │   └── NativeMLPredictor.h         # Tính xác suất ML từ trọng số nhận từ Python
│   │
│   ├── services/                       # [SERVICE LAYER - Tầng điều phối]
│   │   └── TransactionService.h        # Tiếp nhận GD -> Validate -> Chấm điểm -> Trừ tiền
│   │
│   └── ui/                             # [GIAO DIỆN NGƯỜI DÙNG]
│       └── ConsoleUI.h                 # Menu console trực quan có màu sắc ANSI
│
├── sources/                            # TOÀN BỘ FILE HIỆN THỰC (.cpp)
│   ├── common/     -> Types.cpp
│   ├── domain/     -> Account.cpp, Customer.cpp, Transaction.cpp, FraudAlert.cpp...
│   ├── engine/     -> Các file .cpp của Rules, RiskEngine, DecisionEngine, NativeMLPredictor
│   ├── services/   -> TransactionService.cpp
│   ├── ui/         -> ConsoleUI.cpp
│   └── main.cpp                        # Điểm khởi chạy & Kịch bản Demo tự động
│
├── tests/                              # KIỂM THỬ TỰ ĐỘNG
│   ├── test_framework.hpp              # Framework test gọn nhẹ không cần cài thêm thư viện
│   ├── test_dsa.cpp                    # Test các cấu trúc Vector, Deque, PriorityQueue
│   ├── test_rules.cpp                  # Test các luật gian lận
│   └── main_test.cpp                   # File chạy bộ test
│
├── python/                             # PHÂN HỆ MACHINE LEARNING (Thành viên 2)
│   ├── generate_data.py                # Tạo 5,000 giao dịch giả lập (.csv)
│   ├── train_model.py                  # Huấn luyện Logistic Regression / Random Forest
│   ├── evaluate_plots.py               # Xuất biểu đồ ROC, Confusion Matrix dán vào Báo cáo
│   └── export_weights.py               # Xuất file trọng số weights.json sang cho C++
│
└── data/                               # DỮ LIỆU ĐỌC/GHI
    ├── transactions.csv                # Dữ liệu giao dịch mẫu
    ├── blacklist.txt                   # Danh sách đen mẫu
    └── model_weights.json              # File trọng số ML nạp vào C++
```

---

### 🧩 4. CÁCH GẮN CẤU TRÚC DỮ LIỆU (DSA) VÀO BÀI TOÁN

| Cấu trúc dữ liệu tự viết | Vị trí ứng dụng trong hệ thống | Độ phức tạp thuật toán | Giá trị bảo vệ trước hội đồng |
| :--- | :--- | :---: | :--- |
| **`Vector<T>`** | Lưu trữ danh sách giao dịch lịch sử và danh sách các cảnh báo vi phạm. | Truy xuất $O(1)$, thêm cuối amortized $O(1)$. | Quản lý cấp phát bộ nhớ động an toàn, tự động mở rộng dung lượng (grow/shrink). |
| **`Deque<Timestamp>`** | **`VelocityRule`**: Kiểm tra tần suất quẹt thẻ dồn dập (Sliding Time Window). | Thêm cuối $O(1)$, xóa đầu $O(1)$. | Tránh quét lại toàn bộ lịch sử tốn $O(N)$. Luôn duy trì kích thước bằng đúng số giao dịch trong 60 giây. |
| **`PriorityQueue<FraudAlert>`** | Hàng đợi ưu tiên lọc ra các giao dịch nghi vấn có điểm rủi ro cao nhất cho điều tra viên. | Thêm $O(\log K)$, lấy max/min $O(1)$. | Thay vì sắp xếp lại toàn bộ danh sách tốn $O(N \log N)$, Heap duy trì Top-K chỉ tốn $O(N \log K)$. |
| **`HashMap<Key, Value>`** | Tra cứu danh sách đen (Blacklisted Card / Blacklisted IP). | Tra cứu trung bình $O(1)$. | Đạt tốc độ kiểm tra tức thì, nhanh hơn gấp hàng chục lần so với tìm kiếm tuyến tính $O(N)$. |

---

### 🔗 5. CƠ CHẾ KẾT NỐI PYTHON ML VÀ C++ NATIVE ENGINE

Không cần cài đặt thư viện C++ nặng nề (như ONNX runtime hay CPython), hai bạn kết nối cực kỳ thanh lịch theo mô hình:

1. **Phía Python (Thành viên 2)**:
   - Dùng mô hình **Logistic Regression** học ra mảng trọng số $W = [w_1, w_2, \dots, w_n]$ và hệ số tự do $b$ (bias).
   - Xuất ra file `data/model_weights.json`:
     ```json
     {
       "features": ["amount", "velocity", "distance", "device_risk"],
       "weights": [0.002, 1.45, 0.015, 2.10],
       "bias": -3.50
     }
     ```
2. **Phía C++ (Thành viên 1)**:
   - Viết class `NativeMLPredictor`: đọc các số thực từ JSON và tính điểm bằng hàm Sigmoid:
     ```cpp
     double z = bias;
     for (size_t i = 0; i < features.size(); ++i) {
         z += weights[i] * features[i];
     }
     double probability = 1.0 / (1.0 + std::exp(-z));
     double ml_risk_score = probability * 100.0;
     ```
   - **Tốc độ xử lý**: Dưới $0.5\mu\text{s}$ cho mỗi lần suy luận!

---

### 📅 6. QUÁ TRÌNH THỰC HIỆN 14 NGÀY (CHI TIẾT TỪNG NGÀY)

```text
TUẦN 1: HOÀN THIỆN CỐT LÕI (C++ ENGINE & PYTHON MODEL)
--------------------------------------------------------------------------------
Ngày 1 - 2:
  • C++: Sửa dứt điểm 3 lỗi biên dịch (RiskAssessment.h, FraudAlert.h/.cpp, vector.h).
         Cấu hình CMakeLists.txt build thành công tests/test_dsa.cpp.
  • Python: Cài đặt thư viện, viết generate_data.py tạo tập transactions.csv (5,000 dòng).

Ngày 3 - 5:
  • C++: Viết IFraudRule.h và 4 luật: VelocityRule, HighAmountRule, BlacklistRule, DeviceRiskRule.
         Viết RiskEngine và DecisionEngine.
  • Python: Viết train_model.py, huấn luyện Logistic Regression, đánh giá Precision/Recall.
            Xuất file model_weights.json và vẽ biểu đồ ROC Curve.

Ngày 6 - 7:
  • C++: Viết NativeMLPredictor đọc model_weights.json từ Python.
         Viết TransactionService tích hợp toàn bộ luồng.
  • Ghép nối: Hai bạn ngồi lại chạy thử 1 giao dịch từ đầu đến cuối trên máy tính.

TUẦN 2: GIAO DIỆN, BÁO CÁO [PBL2.DOC] & CHUẨN BỊ BẢO VỆ
--------------------------------------------------------------------------------
Ngày 8 - 9:
  • C++: Viết ConsoleUI menu tiếng Việt hiển thị đẹp mắt (màu Xanh/Vàng/Đỏ).
  • Python: Hoàn thiện biểu đồ ma trận nhầm lẫn (Confusion Matrix) và Feature Importance.

Ngày 10 - 12: VIẾT BÁO CÁO ĐỒ ÁN VÀO FILE PBL2.DOC
  • Chương 1 (Thành viên 2): Giới thiệu đề tài, lý do chọn đề tài, khảo sát bài toán.
  • Chương 2 (Thành viên 1): Phân tích chức năng và quy trình luồng giao dịch.
  • Chương 3 (Thành viên 1): Thiết kế Cấu trúc dữ liệu & phân tích độ phức tạp (Vector, Deque, Heap).
  • Chương 4 (Cả 2 bạn):
      - Thành viên 1: Sơ đồ lớp UML, giải thích tính OOP & Strategy Pattern.
      - Thành viên 2: Trình bày phần Machine Learning & dán các biểu đồ đã vẽ.
      - Dán ảnh chụp màn hình chạy Console Demo.
  • Chương 5 (Cả 2 bạn): Kết luận và hướng phát triển (triển khai Microservices/Kafka).

Ngày 13 - 14: LÀM SLIDE & TẬP LUYỆN BẢO VỆ
  • Thiết kế 15-18 slide PowerPoint súc tích, nhiều hình vẽ, ít chữ.
  • Tập dượt thuyết trình (Rehearsal): 1 người nói phần C++/DSA, 1 người nói phần ML/Phân tích.
  • Chuẩn bị sẵn câu trả lời phản biện trước hội đồng.
```

---

### 🎤 7. KỊCH BẢN DEMO GHI ĐIỂM KHI ĐỨNG TRƯỚC GIẢNG VIÊN

Chuẩn bị sẵn 3 kịch bản chạy thử nghiệm trên màn hình Console:
1. **Kịch bản 1 (Khách hàng bình thường)**:
   - Alice mua hàng tạp hóa $45 tại siêu thị quen thuộc.
   - Kết quả: Điểm rủi ro $2.5/100 \rightarrow$ **APPROVE** (Trừ $45 vào tài khoản).
2. **Kịch bản 2 (Tấn công Brute-force / Spam thẻ dồn dập)**:
   - Kẻ gian quẹt thẻ 5 lần liên tiếp trong 20 giây.
   - Kết quả: `VelocityRule` kích hoạt nhờ `Deque` sliding window $\rightarrow$ Điểm rủi ro $85/100 \rightarrow$ **BLOCK** (Chặn giao dịch, bảo vệ số dư).
3. **Kịch bản 3 (Giao dịch đáng ngờ ở nước ngoài / Số tiền lớn)**:
   - Bob chuyển $4,000 từ một thiết bị có địa chỉ IP lạ.
   - Kết quả: Điểm rủi ro $55/100 \rightarrow$ **REVIEW / OTP** $\rightarrow$ Tự động đẩy vào `PriorityQueue` để chờ rà soát.

---
*Kế hoạch này giúp hai bạn hoàn toàn làm chủ đồ án, tự tin trả lời mọi câu hỏi của hội đồng và đạt điểm số cao nhất.*
