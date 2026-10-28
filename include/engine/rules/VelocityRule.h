#pragma once

#include "engine/rules/IFraudRule.h"
#include "DSA/Deque.h"
#include <chrono>

/**
 * ============================================================================
 * [BƯỚC 2.4] TODO: KHAI BÁO LỚP VELOCITYRULE (LUẬT QUẸT THẺ SPAM LIÊN TỤC)
 * Tài liệu hướng dẫn: include/engine/02_STEP2_CONCRETE_RULES.md (Mục 2.4)
 * File hiện thực tương ứng: sources/engine/rules/VelocityRule.cpp
 * ============================================================================
 * 
 * Mục tiêu:
 * - Ứng dụng cấu trúc dữ liệu Deque (Hàng đợi 2 đầu) để triển khai thuật toán
 *   Cửa sổ trượt thời gian (Sliding Time Window).
 * - Kiểm tra tần suất giao dịch trong 60 giây gần nhất với độ phức tạp O(1).
 */
class VelocityRule : public IFraudRule {
private:
    // TODO 1: Cửa sổ trượt lưu các mốc thời gian (Timestamp) của các giao dịch gần đây
    // Dùng template Deque tự viết: Deque<Timestamp> sliding_window;
    Deque<Timestamp> sliding_window;

    // TODO 2: Thuộc tính ngưỡng số giao dịch tối đa cho phép trong cửa sổ thời gian
    int max_transactions;

    // TODO 3: Độ dài khoảng thời gian của cửa sổ trượt (mặc định: 60 giây)
    std::chrono::seconds window_duration;

    // TODO 4: Điểm rủi ro đóng góp (mặc định: 30.0)
    double risk_contribution;

public:
    // TODO 5: Constructor khởi tạo các ngưỡng và cấu hình cửa sổ
    explicit VelocityRule(
        int max_txs = 5,
        std::chrono::seconds duration = std::chrono::seconds(60),
        double contribution = 30.0
    );

    // TODO 6: Ghi đè các phương thức định danh:
    // - getId() -> "RULE_001_VELOCITY"
    // - getName() -> "High Transaction Frequency (Velocity)"
    // - getCategory() -> FraudRuleCategory::RAPID_TRANSACTIONS
    // - getRiskContribution()
    std::string getId() const override;
    std::string getName() const override;
    FraudRuleCategory getCategory() const override;
    double getRiskContribution() const override;

    // TODO 7: Khai báo hàm thẩm định giao dịch (Sliding Window Algorithm)
    bool evaluate(
        const Transaction& tx,
        const Customer& customer,
        FraudAlert& out_alert
    ) override;

    // TODO 8: Hàm reset/làm rỗng cửa sổ trượt (dùng khi bắt đầu phiên test mới)
    void reset();
};
