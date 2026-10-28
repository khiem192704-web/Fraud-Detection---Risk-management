# 📜 Bước 1: Thiết Kế Interface Cơ Sở `IFraudRule`

> **File cần code**: [include/engine/rules/IFraudRule.h](file:///d:/Fraud-Detection---Risk-management/include/engine/rules/IFraudRule.h)  
> **Khái niệm OOP cốt lõi**: Interface (Lớp trừu tượng thuần ảo), **Strategy Pattern**, Tính Đa hình (Polymorphism).  
> **Thời gian dự kiến**: 15 - 20 phút.

---

## 🎯 1. Bản chất & Mục đích của `IFraudRule`

Trong một hệ thống thanh toán lớn, các luật gian lận **luôn luôn thay đổi** (hôm nay thêm luật cờ bạc, ngày mai thêm luật quét IP lạ, ngày mốt đổi luật hạn mức).
- **Nếu KHÔNG dùng OOP Interface**: Bạn sẽ phải viết một hàm `checkFraud()` dài 2,000 dòng với hàng chục câu lệnh `if...else if...else`. Khi muốn sửa một luật, bạn có nguy cơ làm hỏng toàn bộ hệ thống.
- **Khi DÙNG Strategy Pattern**: 
  - `IFraudRule` đóng vai trò là một "Hợp đồng (Contract)".
  - Mọi luật cụ thể (`VelocityRule`, `BlacklistRule`,...) đều chỉ là một "Chiến lược (Strategy)" thực thi hợp đồng này.
  - Sau này muốn thêm luật mới? Chỉ cần tạo một class mới kế thừa `IFraudRule` mà **không cần sửa một dòng code nào** của hệ thống cũ (nguyên lý Open/Closed Principle - SOLID).

---

## 📐 2. Phân Tích Cấu Trúc File `IFraudRule.h`

```cpp
#pragma once

#include <string>
#include "core/common/Types.h"
#include "core/domain/transaction/Transaction.h"
#include "core/domain/account/Customer.h"
#include "core/domain/alert/FraudAlert.h"

class IFraudRule {
public:
    // BẮT BUỘC: Virtual Destructor để giải phóng bộ nhớ an toàn qua con trỏ lớp cha
    virtual ~IFraudRule() = default;

    // 1. Định danh duy nhất cho luật (vd: "RULE_001_VELOCITY")
    virtual std::string getId() const = 0;

    // 2. Tên mô tả ngắn gọn (vd: "High Velocity Detected")
    virtual std::string getName() const = 0;

    // 3. Phân loại danh mục luật (enum FraudRuleCategory trong Types.h)
    virtual FraudRuleCategory getCategory() const = 0;

    // 4. Số điểm rủi ro đóng góp nếu luật này bị vi phạm (thường từ 10.0 -> 40.0 điểm)
    virtual double getRiskContribution() const = 0;

    /**
     * @brief Phương thức thẩm định chính của luật (Trọng tâm)
     * @param tx Giao dịch cần kiểm tra (đọc số tiền, thiết bị, vị trí, giờ giao dịch)
     * @param customer Hồ sơ khách hàng (đọc phân tầng KYC, lịch sử rủi ro)
     * @param out_alert Đối tượng FraudAlert được gán dữ liệu NẾU phát hiện vi phạm
     * @return true nếu VI PHẠM (kích hoạt cảnh báo), false nếu GIAO DỊCH AN TOÀN
     */
    virtual bool evaluate(
        const Transaction& tx,
        const Customer& customer,
        FraudAlert& out_alert
    ) = 0;
};
```

---

## 🔍 3. Những Điểm Kỹ Thuật Quan Trọng Bạn Cần Hiểu Khi Tự Code

1. **Tại sao hàm hủy ảo `virtual ~IFraudRule() = default;` lại sống còn?**
   - Trong `RiskEngine`, bạn sẽ quản lý một danh sách con trỏ `Vector<IFraudRule*> rules;`.
   - Khi giải phóng một luật con thông qua con trỏ cha (`delete rules[i]`), nếu destructor không có từ khóa `virtual`, C++ sẽ chỉ gọi destructor của `IFraudRule` mà bỏ qua destructor của lớp con $\to$ **Rò rỉ bộ nhớ (Memory Leak)** (đặc biệt nguy hiểm với `VelocityRule` vì có chứa `Deque` cấp phát động).
2. **Cú pháp `= 0;` (Pure Virtual Function)**:
   - Biến `IFraudRule` thành một **Lớp trừu tượng (Abstract Class)**.
   - Bạn không thể tạo trực tiếp `IFraudRule r;` (sẽ báo lỗi biên dịch). Bắt buộc phải tạo lớp con hiện thực đủ 5 hàm trên mới được khởi tạo đối tượng.
3. **Tại sao `tx` và `customer` truyền tham chiếu hằng (`const&`), còn `out_alert` truyền tham chiếu thường (`&`)?**
   - `const Transaction& tx`: Chỉ đọc thông tin giao dịch, tuyệt đối không được sửa đổi dữ liệu gốc trong lúc kiểm tra luật.
   - `FraudAlert& out_alert`: Là **Tham số đầu ra (Output Parameter)**. Nếu luật phát hiện gian lận, luật sẽ tự tay gán mã vi phạm, mô tả nguyên nhân và điểm số vào `out_alert` để chuyển ra ngoài cho `RiskEngine`.

---

## 🎓 4. Câu Hỏi Vấn Đáp Thầy Cô Hay Hỏi Bảo Vệ PBL2

> **Hỏi**: *"Trong dự án em áp dụng OOP như thế nào ở module thẩm định rủi ro?"*  
> **Đáp**: *"Dạ em sử dụng **Strategy Pattern** thông qua interface trừu tượng `IFraudRule`. Mỗi luật gian lận như kiểm tra tần suất `VelocityRule` hay danh sách đen `BlacklistRule` là một chiến lược độc lập kế thừa từ `IFraudRule`. Nhờ áp dụng tính **Đa hình (Polymorphism)**, lớp điều phối `RiskEngine` chỉ cần lưu trữ `Vector<IFraudRule*>` và duyệt qua hàm `evaluate()` mà không cần quan tâm chi tiết bên trong từng luật hoạt động ra sao. Điều này giúp hệ thống dễ dàng mở rộng thêm luật mới mà không làm thay đổi mã nguồn sẵn có."*

