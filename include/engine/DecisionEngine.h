#pragma once
#include "core/common/Types.h"
#include "core/domain/Transaction.h"
#include "core/domain/Customer.h"
#include "core/domain/RiskAssessment.h"
// (Điểm >= 80.0 sẽ bị từ chối BLOCK)
struct DecisionPolicy{
    double approve_max_score{39.99};//approve
    double challenge_max_score{59.99};//request otp
    double review_max_score{79.99};//investigate
};
class DecisionEngine{
private:
    // TODO 2: Khai báo biến thành viên lưu chính sách ra quyết định
    DecisionPolicy policy;
public:
    // TODO 3: Constructor nhận vào policy tùy chỉnh (mặc định DecisionPolicy())
    explicit DecisionEngine(const DecisionPolicy& custom_policy=DecisionPolicy());
    void setPolicy(const DecisionPolicy& new_policy);
    const DecisionPolicy& getPolicy() const;
    // - Tác vụ phụ: Gọi tx.approve(), tx.block(), tx.challenge(), tx.sendToReview() tương ứng
    DecisionAction decide(Transaction&tx,const Customer& customer,RiskAssessment& assessment) const;
    // - Nếu khách hàng bị BANNED hoặc có alert CRITICAL -> lập tức trả về true
    bool checkEmergencyBlock(const Customer& customer,const RiskAssessment& assessment) const;
};
