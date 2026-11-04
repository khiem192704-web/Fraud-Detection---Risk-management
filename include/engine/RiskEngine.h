#pragma once

#include <string>
#include "DSA/vector.h"
#include "engine/rules/IFraudRule.h"
#include "core/common/Types.h"
#include "core/domain/Transaction.h"
#include "core/domain/Customer.h"
#include "core/domain/RiskAssessment.h"
#include "core/domain/FraudAlert.h"

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

// TODO 1: Khai báo struct RiskPolicy chứa trọng số và ngưỡng phân tầng rủi ro:
// - Trọng số các thành phần đóng góp (tổng = 1.0): weight_rules (0.50), weight_ml (0.40), weight_profile (0.10)
// - Ngưỡng phân tầng RiskLevel: threshold_very_low (20.0), threshold_low (40.0), threshold_medium (60.0), threshold_high (80.0)
// - Phương thức determineRiskLevel(double score) const ánh xạ điểm sang enum RiskLevel
// Gợi ý:
// struct RiskPolicy {
//     double weight_rules{0.50};
//     double weight_ml{0.40};
//     double weight_profile{0.10};
// 
//     double threshold_very_low{20.0};
//     double threshold_low{40.0};
//     double threshold_medium{60.0};
//     double threshold_high{80.0};
// 
//     RiskLevel determineRiskLevel(double score) const {
//         if (score < threshold_very_low) return RiskLevel::VERY_LOW;
//         if (score < threshold_low)      return RiskLevel::LOW;
//         if (score < threshold_medium)   return RiskLevel::MEDIUM;
//         if (score < threshold_high)     return RiskLevel::HIGH;
//         return RiskLevel::CRITICAL;
//     }
// };
struct RiskPolicy {

};

class RiskEngine {
private:
    // TODO 2: Danh sách các luật gian lận được quản lý dưới dạng con trỏ lớp cha IFraudRule* (Đa hình Polymorphism)
    // Gợi ý: Vector<IFraudRule*> rules;


    // TODO 3: Thuộc tính lưu chính sách trọng số rủi ro
    // Gợi ý: RiskPolicy policy;


public:
    // TODO 4: Khởi tạo Constructor nhận vào policy tùy chỉnh (mặc định RiskPolicy())
    // Gợi ý: explicit RiskEngine(const RiskPolicy& custom_policy = RiskPolicy());


    // TODO 5: Destructor quan trọng: Duyệt qua mảng rules và gọi `delete rules[i]` để giải phóng bộ nhớ đối tượng con
    // Gợi ý: ~RiskEngine();


    // TODO 6: Hàm đăng ký thêm một luật mới vào Engine
    // Gợi ý: void registerRule(IFraudRule* rule);


    // TODO 7: Hàm lấy số lượng luật hiện có trong Engine
    // Gợi ý: int getRuleCount() const;


    // TODO 8: Cấu hình và truy xuất Policy
    // Gợi ý:
    // void setPolicy(const RiskPolicy& new_policy);
    // const RiskPolicy& getPolicy() const;


    // TODO 9: Hàm thẩm định rủi ro cốt lõi (tính điểm tổng hợp và trả về RiskAssessment)
    // Gợi ý:
    // RiskAssessment assessRisk(
    //     const Transaction& tx,
    //     const Customer& customer,
    //     double ml_score = 0.0
    // );

};
