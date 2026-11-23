#pragma once
#include <string>
#include <vector>
#include "engine/rules/IFraudRule.h"
#include "core/common/Types.h"
#include "core/domain/Transaction.h"
#include "core/domain/Customer.h"
#include "core/domain/RiskAssessment.h"
#include "core/domain/FraudAlert.h"
// TODO 1: Khai báo struct RiskPolicy chứa trọng số và ngưỡng phân tầng rủi ro:
struct RiskPolicy{
    double weight_rules{0.5};
    double weight_ml{0.4};
    double weight_profile{0.1};

    double threshold_very_low{20.0};
    double threshold_low{40.0};
    double threshold_medium{60.0};
    double threshold_high{80.0};

    RiskLevel determineRiskLevel(double score) const{
        if(score<threshold_very_low) return RiskLevel::VERY_LOW;
        if(score<threshold_low) return RiskLevel::LOW;
        if(score<threshold_medium) return RiskLevel::MEDIUM;
        if(score<threshold_high) return RiskLevel::HIGH;
        return RiskLevel::CRITICAL;
    }
};
class RiskEngine {
private:
    // TODO 2: Danh sách các luật gian lận được quản lý dưới dạng con trỏ lớp cha IFraudRule* (Đa hình Polymorphism)
    std::vector<IFraudRule*>rules;
    // TODO 3: Thuộc tính lưu chính sách trọng số rủi ro
    RiskPolicy policy;
public:
    // TODO 4: Khởi tạo Constructor nhận vào policy tùy chỉnh (mặc định RiskPolicy())
    explicit RiskEngine(const RiskPolicy& custom_policy=RiskPolicy());
    // TODO 5: Destructor quan trọng: Duyệt qua mảng rules và gọi `delete rules[i]` để giải phóng bộ nhớ đối tượng con
    ~RiskEngine();
    // TODO 6: Hàm đăng ký thêm một luật mới vào Engine
    void registerRule(IFraudRule*rule);
    // TODO 7: Hàm lấy số lượng luật hiện có trong Engine
    int getRuleCount() const;
    // TODO 8: Cấu hình và truy xuất Policy
    void setPolicy(const RiskPolicy& new_policy);
    const RiskPolicy& getPolicy() const;
    // TODO 9: Hàm thẩm định rủi ro cốt lõi (tính điểm tổng hợp và trả về RiskAssessment)
    RiskAssessment assessRisk(const Transaction&tx,const Customer& customer, double ml_score=0.0);
};
