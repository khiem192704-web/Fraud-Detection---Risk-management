#pragma once

#include <string>
#include "DSA/vector.h"
#include "engine/rules/IFraudRule.h"
#include "core/common/Types.h"
#include "core/domain/transaction/Transaction.h"
#include "core/domain/account/Customer.h"
#include "core/domain/alert/RiskAssessment.h"
#include "core/domain/alert/FraudAlert.h"

/**
 * ============================================================================
 * [BƯỚC 4] TODO: KHAI BÁO STRUCT RISKPOLICY & LỚP RISKENGINE (CHẤM ĐIỂM RỦI RO)
 * Tài liệu hướng dẫn: include/engine/04_STEP4_RISK_ENGINE.md
 * File hiện thực tương ứng: sources/engine/RiskEngine.cpp
 * ============================================================================
 * 
 * Mục tiêu:
 * - Đóng vai trò Nhạc trưởng (Coordinator): Quản lý tập hợp Vector<IFraudRule*>.
 * - Chạy duyệt qua từng rule, thu thập FraudAlert, kết hợp điểm ML để tính
 *   điểm rủi ro tổng hợp (0.0 -> 100.0) và xuất RiskAssessment.
 */

// TODO 1: Struct chứa chính sách trọng số và ngưỡng phân tầng rủi ro
struct RiskPolicy {
    // Trọng số các thành phần đóng góp (Tổng = 1.0)
    double weight_rules{0.50};      // 50% từ vi phạm các luật gian lận
    double weight_ml{0.40};         // 40% từ mô hình học máy
    double weight_profile{0.10};    // 10% từ rủi ro nền của hồ sơ khách hàng

    // Ngưỡng phân tầng mức độ rủi ro (RiskLevel)
    double threshold_very_low{20.0};
    double threshold_low{40.0};
    double threshold_medium{60.0};
    double threshold_high{80.0};

    // Hàm ánh xạ từ điểm số [0.0, 100.0] sang enum RiskLevel
    RiskLevel determineRiskLevel(double score) const {
        if (score < threshold_very_low) return RiskLevel::VERY_LOW;
        if (score < threshold_low)      return RiskLevel::LOW;
        if (score < threshold_medium)   return RiskLevel::MEDIUM;
        if (score < threshold_high)     return RiskLevel::HIGH;
        return RiskLevel::CRITICAL;
    }
};

class RiskEngine {
private:
    // TODO 2: Danh sách các luật gian lận được quản lý dưới dạng con trỏ lớp cha
    // Áp dụng Đa hình (Polymorphism): Vector<IFraudRule*> rules;
    Vector<IFraudRule*> rules;

    // TODO 3: Chính sách trọng số rủi ro
    RiskPolicy policy;

public:
    // TODO 4: Khởi tạo Constructor nhận vào policy tùy chỉnh (mặc định RiskPolicy())
    explicit RiskEngine(const RiskPolicy& custom_policy = RiskPolicy());

    // TODO 5: Destructor quan trọng: Duyệt qua mảng rules và gọi `delete rules[i]`
    // để giải phóng bộ nhớ của các đối tượng luật con!
    ~RiskEngine();

    // TODO 6: Hàm đăng ký thêm một luật mới vào Engine
    void registerRule(IFraudRule* rule);

    // TODO 7: Lấy số lượng luật hiện có trong Engine
    int getRuleCount() const;

    // TODO 8: Cấu hình và truy xuất Policy
    void setPolicy(const RiskPolicy& new_policy);
    const RiskPolicy& getPolicy() const;

    // TODO 9: Hàm thẩm định rủi ro cốt lõi:
    // Nhận Transaction, Customer, ml_score -> Tính tổng hợp -> Trả về RiskAssessment
    RiskAssessment assessRisk(
        const Transaction& tx,
        const Customer& customer,
        double ml_score = 0.0
    );
};
