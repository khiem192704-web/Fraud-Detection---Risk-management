# 🛡️ Bước 2: Cài Đặt 4 Luật Gian Lận Cốt Lõi

> **Thư mục liên quan**:  
> - Header: [include/engine/rules/](file:///d:/Fraud-Detection---Risk-management/include/engine/rules/)  
> - Source: [sources/engine/rules/](file:///d:/Fraud-Detection---Risk-management/sources/engine/rules/)  
> **Khái niệm cốt lõi**: Kế thừa (Inheritance), Ghi đè phương thức (Override), **DSA Deque** (Cửa sổ trượt), **DSA HashMap** (Tra cứu O(1)).

---

## 📌 Thứ tự code 4 luật (Đi từ dễ đến nâng cao)

```text
2.1. DeviceRiskRule  ──► Dễ nhất (Chỉ đọc cờ thiết bị có sẵn)
2.2. HighAmountRule  ──► Trung bình (So sánh số tiền & hệ số đột biến)
2.3. BlacklistRule   ──► Nâng cao (Ứng dụng Bảng băm HashMap O(1))
2.4. VelocityRule    ──► Nâng cao (Thuật toán Sliding Window với Deque)
```

---

### 🟢 2.1. `DeviceRiskRule` (Luật Môi Trường Thiết Bị Độc Hại)

#### 1. Bản chất
Kiểm tra xem giao dịch có được thực hiện từ môi trường nghi vấn (máy ảo emulator trên PC của hacker, thiết bị Android đã bị root hoặc iPhone đã jailbreak).

#### 2. Dữ liệu đầu vào lấy từ đâu?
Trong `tx`, ta gọi `tx.GetDevice()` để lấy đối tượng `Device`.
Đối tượng `Device` cung cấp 2 phương thức kiểm tra:
- `device.GetIs_Virtual()`: `true` nếu là máy ảo/giả lập.
- `device.GetIs_Hacker()`: `true` nếu thiết bị có dấu hiệu root/can thiệp hệ thống.

#### 3. Thuật toán `evaluate(...)`
```cpp
// Logic kiểm tra:
const Device& dev = tx.GetDevice();
if (dev.GetIs_Virtual() || dev.GetIs_Hacker()) {
    // 1. Tạo FraudAlert với đầy đủ thông tin:
    //    - Alert ID: "ALT_" + tx.GetTransactionID() + "_DEV"
    //    - Transaction ID: tx.GetTransactionID()
    //    - Rule ID: getId()
    //    - Rule Name: getName()
    //    - Reason: "Thiết bị không an toàn (máy ảo hoặc root/hacker)"
    //    - Category: FraudRuleCategory::SUSPICIOUS_DEVICE
    //    - Score: getRiskContribution() (vd: 30.0)
    //    - Severity: RiskLevel::HIGH
    // 2. Gán vào out_alert
    // 3. return true;
}
return false;
```

---

### 🟡 2.2. `HighAmountRule` (Luật Số Tiền Đột Biến)

#### 1. Bản chất
Bắt các giao dịch có số tiền bất thường:
1. **Ngưỡng trần tuyệt đối**: Giao dịch quá lớn (ví dụ $> 5,000\$$ một lần).
2. **Ngưỡng tương đối**: Số tiền gấp $> 5$ lần hạn mức hoặc mức chi tiêu bình quân của tài khoản.

#### 2. Thuật toán `evaluate(...)`
```cpp
double amount = tx.GetAmount();

// Điều kiện 1: Vượt ngưỡng tiền trần cố định
if (amount >= max_amount_threshold) {
    out_alert = FraudAlert(
        "ALT_" + tx.GetTransactionID() + "_AMT",
        tx.GetTransactionID(),
        getId(),
        getName(),
        "So tien giao dich vuot nguong an toan: $" + std::to_string(amount),
        FraudRuleCategory::UNUSUAL_AMOUNT,
        getRiskContribution(),
        RiskLevel::HIGH
    );
    return true;
}
return false;
```

---

### 🟠 2.3. `BlacklistRule` (Luật Danh Sách Đen — Ứng dụng DSA HashMap)

#### 1. Bản chất & Giá trị DSA
- Hệ thống thanh toán có hàng triệu thẻ và IP bị rò rỉ.
- Nếu lưu danh sách đen trong mảng thường `Vector` hoặc `array`, mỗi lần có giao dịch phải duyệt tuyến tính mất **$O(N)$**.
- Khi dùng **`HashMap` (Bảng băm)**, hàm băm tính toán địa chỉ tức thì giúp tốc độ tra cứu đạt **$O(1)$** trung bình!

#### 2. Cấu trúc lớp
```cpp
class BlacklistRule : public IFraudRule {
private:
    double risk_contribution{40.0};
    // Có thể dùng std::unordered_map hoặc HashMap tự viết từ include/DSA/
    // Lưu: Key (IP / AccountID) -> Value (Lý do cấm)

public:
    void addBlacklistedIP(const std::string& ip, const std::string& reason);
    void addBlacklistedAccount(const std::string& account_id, const std::string& reason);

    bool isIPBlacklisted(const std::string& ip) const;
    bool isAccountBlacklisted(const std::string& account_id) const;

    bool evaluate(const Transaction& tx, const Customer& customer, FraudAlert& out_alert) override;
};
```

#### 3. Thuật toán `evaluate(...)`
1. Kiểm tra IP giao dịch `tx.GetIP_Address()` có trong Blacklist không?
2. Kiểm tra mã tài khoản `tx.GetAccountID()` có trong Blacklist không?
3. Nếu khớp bất kỳ điều kiện nào:
   - Điểm vi phạm: $+40.0$ điểm (Mức nghiêm trọng: `RiskLevel::CRITICAL`).
   - `return true;`.

---

### 🔴 2.4. `VelocityRule` (Luật Spam Quẹt Thẻ — Ứng dụng DSA Deque)

#### 1. Bản chất & Bài toán Cửa sổ trượt (Sliding Window)
Hacker thường dùng script tự động để quét mã thẻ (quẹt 10 lần trong 10 giây). Làm sao để máy tính biết khách hàng đã quẹt bao nhiêu lần trong **60 giây gần nhất**?

*Cách làm ngây thơ (Sai lầm)*: Lưu toàn bộ lịch sử 1,000 giao dịch trong database rồi mỗi lần quét lại từ đầu $\to$ Tốn $O(N)$ thời gian và ngốn bộ nhớ.

*Cách làm tối ưu bằng **`Deque<Timestamp>` (Hàng đợi hai đầu)**:
1. `sliding_window` chỉ lưu các mốc thời gian (`Timestamp`) của các giao dịch gần đây.
2. Khi có giao dịch mới lúc $T_{now}$:
   - **Bước loại bỏ (Slide)**: Xem phần tử đầu tiên (`sliding_window.front()`). Nếu nó xảy ra cách đây quá 60 giây, gọi `pop_front()` vứt bỏ. Lặp lại cho đến khi mọi phần tử trong Deque đều nằm trong khoảng $[T_{now} - 60s, T_{now}]$.
   - **Bước ghi nhận**: Gọi `push_back(T_{now})`.
   - **Bước kiểm tra**: Kích thước `sliding_window.size()`. Nếu vượt quá `max_transactions` (ví dụ $> 5$ giao dịch) $\to$ **KÍCH HOẠT CẢNH BÁO GIAN LẬN!**

#### 2. Mã nguồn mẫu cho thuật toán Deque Sliding Window
```cpp
bool VelocityRule::evaluate(const Transaction& tx, const Customer&, FraudAlert& out_alert) {
    Timestamp now = tx.GetTimestamp();

    // 1. Loại bỏ các giao dịch cũ nằm ngoài cửa sổ thời gian (60 giây)
    while (!sliding_window.empty()) {
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - sliding_window.front()).count();
        if (diff > window_duration.count()) {
            sliding_window.pop_front(); // Loại bỏ O(1)
        } else {
            break; // Vì Deque có thứ tự thời gian tăng dần, gặp phần tử hợp lệ thì dừng
        }
    }

    // 2. Thêm mốc thời gian giao dịch hiện tại vào cuối Deque
    sliding_window.push_back(now); // Thêm O(1)

    // 3. Kiểm tra số lượng giao dịch dồn dập
    if (sliding_window.size() > max_transactions) {
        out_alert = FraudAlert(
            "ALT_" + tx.GetTransactionID() + "_VEL",
            tx.GetTransactionID(),
            getId(),
            getName(),
            "Phat hien tan suat quet the bat thuong: " + std::to_string(sliding_window.size()) + " giao dich trong 60 giay",
            FraudRuleCategory::RAPID_TRANSACTIONS,
            getRiskContribution(),
            RiskLevel::HIGH
        );
        return true;
    }

    return false;
}
```

---

## 🎓 Câu Hỏi Vấn Đáp Thầy Cô Hay Hỏi Bảo Vệ PBL2

> **Hỏi**: *"Tại sao trong `VelocityRule` em lại dùng cấu trúc dữ liệu `Deque` mà không dùng `Vector` hay `Queue` thông thường?"*  
> **Đáp**: *"Dạ thưa thầy/cô, thuật toán Cửa sổ trượt (Sliding Window) yêu cầu hai thao tác: thêm mốc thời gian mới vào cuối và loại bỏ các mốc thời gian hết hạn ở đầu. Nếu dùng `Vector`, thao tác xóa ở đầu mảng tốn $O(N)$ do phải dồn toàn bộ phần tử về trước. Còn `Queue` thông thường không hỗ trợ kiểm tra linh hoạt kích thước hoặc duyệt phần tử đầu. `Deque` (Double-ended Queue) tự viết cho phép cả thao tác `push_back()` và `pop_front()` đạt độ phức tạp tức thì **$O(1)$**, giúp hệ thống duy trì cửa sổ thời gian cực kỳ nhẹ và tối ưu bộ nhớ."*

