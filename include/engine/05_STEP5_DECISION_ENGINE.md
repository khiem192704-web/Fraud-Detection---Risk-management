# 🚦 Bước 5: Động Cơ Ra Quyết Định `DecisionEngine`

> **File cần code**:  
> - Header: [include/engine/DecisionEngine.h](file:///d:/Fraud-Detection---Risk-management/include/engine/DecisionEngine.h)  
> - Source: [sources/engine/DecisionEngine.cpp](file:///d:/Fraud-Detection---Risk-management/sources/engine/DecisionEngine.cpp)  
> **Khái niệm cốt lõi**: Phân loại hành động kinh doanh (Business Decision Maker), Cơ chế Phủ quyết khẩn cấp (Emergency Override), Cập nhật vòng đời đối tượng `Transaction`.  
> **Thời gian dự kiến**: 25 - 35 phút.

---

## 🎯 1. Bản chất: Vai trò của `DecisionEngine`

Nếu `RiskEngine` là **"Bác sĩ chẩn đoán"** (kết luận bệnh nhân có điểm rủi ro $75/100$), thì `DecisionEngine` là **"Bác sĩ kê đơn"** (quyết định cho giao dịch qua, gửi mã OTP, chuyển cho điều tra viên duyệt, hay chặn ngay lập tức).

Nhiệm vụ của `DecisionEngine`:
1. Đọc điểm số `combined_score` từ `RiskAssessment`.
2. Kiểm tra các **Luật phủ quyết khẩn cấp (Emergency Override)**.
3. Ra quyết định `DecisionAction` (`APPROVE`, `CHALLENGE_3DS`, `REVIEW`, `BLOCK`).
4. **Cập nhật trực tiếp trạng thái của `Transaction`** (`tx.approve()`, `tx.block()`,...).

---

## 📐 2. Cấu Trúc `DecisionPolicy` (Chính Sách Ra Quyết Định)

```cpp
struct DecisionPolicy {
    // Ngưỡng điểm để phân loại hành động
    double approve_max_score{39.99};    // [0.0, 40.0)  -> APPROVE (Giao dịch an toàn, duyệt ngay)
    double challenge_max_score{59.99};  // [40.0, 60.0) -> CHALLENGE_3DS (Nghi vấn nhẹ, bắt nhập OTP)
    double review_max_score{79.99};     // [60.0, 80.0) -> REVIEW (Rủi ro cao, chuyển điều tra viên duyệt)
                                        // >= 80.0      -> BLOCK (Nguy hiểm cực độ, từ chối giao dịch)
};
```

---

## ⚡ 3. Cơ Chế Phủ Quyết Khẩn Cấp (`Emergency Override`)

Trong thực tế, có những tình huống nguy hiểm đến mức **không cần quan tâm điểm số tổng hợp bao nhiêu**, hệ thống phải lập tức chặn:
1. **Khách hàng đã nằm trong danh sách cấm**: `customer.getRiskTier() == CustomerRiskTier::BANNED`.
2. **Có cảnh báo mức độ CRITICAL**: Trong danh sách `alerts` có bất kỳ cảnh báo nào trả về `alert.isCritical() == true` (ví dụ: IP nằm trong Blacklist hoặc thiết bị hacker đã root).

### Code kiểm tra trong `DecisionEngine.cpp`:
```cpp
bool DecisionEngine::checkEmergencyBlock(
    const Customer& customer,
    const RiskAssessment& assessment
) const {
    // 1. Khách hàng đã bị cấm trên toàn hệ thống
    if (customer.getRiskTier() == CustomerRiskTier::BANNED) {
        return true;
    }

    // 2. Có cảnh báo nghiêm trọng (CRITICAL) từ bộ luật
    const auto& alerts = assessment.getAlerts();
    for (int i = 0; i < alerts.size(); ++i) {
        if (alerts[i].isCritical()) {
            return true;
        }
    }

    return false;
}
```

---

## 🚦 4. Thuật Toán Ra Quyết Định `decide(...)`

```cpp
DecisionAction DecisionEngine::decide(
    Transaction& tx,
    const Customer& customer,
    RiskAssessment& assessment
) const {
    // BƯỚC 1: Kiểm tra phủ quyết khẩn cấp trước tiên
    if (checkEmergencyBlock(customer, assessment)) {
        tx.block(); // Cập nhật trạng thái Transaction sang BLOCKED
        return DecisionAction::BLOCK;
    }

    // BƯỚC 2: So sánh điểm số tổng hợp với DecisionPolicy
    double score = assessment.getCombinedScore();
    DecisionAction action;

    if (score <= policy.approve_max_score) {
        action = DecisionAction::APPROVE;
        tx.approve(); // Chuyển trạng thái sang APPROVED
    } 
    else if (score <= policy.challenge_max_score) {
        action = DecisionAction::CHALLENGE_3DS;
        tx.challenge(); // Chuyển trạng thái sang VERIFY_OTP
    } 
    else if (score <= policy.review_max_score) {
        action = DecisionAction::REVIEW;
        tx.sendToReview(); // Chuyển trạng thái sang WAIT_REVIEW
    } 
    else {
        action = DecisionAction::BLOCK;
        tx.block(); // Chuyển trạng thái sang BLOCKED
    }

    return action;
}
```

---

## 🎓 Câu Hỏi Vấn Đáp Thầy Cô Hay Hỏi Bảo Vệ PBL2

> **Hỏi**: *"Tại sao hệ thống của em đã có công thức tính điểm rồi mà lại cần thêm hàm `checkEmergencyBlock`?"*  
> **Đáp**: *"Dạ thưa thầy/cô, đây là cơ chế an toàn **Defense in Depth** trong bảo mật thanh toán. Khi một thẻ ngân hàng đã được xác nhận là thẻ bị đánh cắp (Stolen Card) nằm trong danh sách đen, dù hacker chỉ quẹt thử số tiền rất nhỏ $1\$$, công thức tính điểm có thể cho ra điểm thấp (chưa tới 80). Nếu chỉ dựa vào điểm số, giao dịch sẽ lọt qua. Do đó, cơ chế `Emergency Override` sẽ can thiệp ngay lập tức để phủ quyết điểm số và chặn đứng giao dịch mà không cần chờ phân tích thêm."*

