#pragma once

#include "core/common/Types.h"
#include "core/domain/transaction/Transaction.h"
#include "core/domain/account/Customer.h"
#include "core/domain/alert/RiskAssessment.h"

/**
 * ============================================================================
 * [BƯỚC 5] TODO: KHAI BÁO STRUCT DECISIONPOLICY & LỚP DECISIONENGINE (RA QUYẾT ĐỊNH)
 * Tài liệu hướng dẫn: include/engine/05_STEP5_DECISION_ENGINE.md
 * File hiện thực tương ứng: sources/engine/DecisionEngine.cpp
 * ============================================================================
 * 
 * Mục tiêu:
 * - Chuyển điểm rủi ro số học thành hành động nghiệp vụ thực tế:
 *   APPROVE, CHALLENGE_3DS, REVIEW, BLOCK.
 * - Kiểm tra cơ chế Phủ quyết khẩn cấp (Emergency Override) trước khi xét điểm.
 * - Cập nhật trực tiếp trạng thái vòng đời của Transaction.
 */

// TODO 1: Struct chứa các ngưỡng điểm để phân loại hành động
struct DecisionPolicy {
    double approve_max_score{39.99};    // [0.0, 40.0)  -> APPROVE (Duyệt ngay)
    double challenge_max_score{59.99};  // [40.0, 60.0) -> CHALLENGE_3DS (Yêu cầu OTP)
    double review_max_score{79.99};     // [60.0, 80.0) -> REVIEW (Điều tra viên duyệt tay)
                                        // >= 80.0      -> BLOCK (Chặn giao dịch)
};

class DecisionEngine {
private:
    // TODO 2: Chính sách ra quyết định
    DecisionPolicy policy;

public:
    // TODO 3: Constructor với policy mặc định
    explicit DecisionEngine(const DecisionPolicy& custom_policy = DecisionPolicy());

    // TODO 4: Cấu hình và truy xuất Policy
    void setPolicy(const DecisionPolicy& new_policy);
    const DecisionPolicy& getPolicy() const;

    // TODO 5: Hàm ra quyết định cốt lõi:
    // - Nhận: Transaction& tx, Customer, RiskAssessment& assessment
    // - Trả về: DecisionAction
    // - Tác vụ phụ: Gọi tx.approve(), tx.block(), tx.challenge() tương ứng
    DecisionAction decide(
        Transaction& tx,
        const Customer& customer,
        RiskAssessment& assessment
    ) const;

    // TODO 6: Hàm kiểm tra phủ quyết khẩn cấp:
    // Nếu khách hàng bị BANNED hoặc có alert CRITICAL -> lập tức trả về true
    bool checkEmergencyBlock(
        const Customer& customer,
        const RiskAssessment& assessment
    ) const;
};
