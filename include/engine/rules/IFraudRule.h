#pragma once

#include <string>
#include "core/common/Types.h"
#include "core/domain/transaction/Transaction.h"
#include "core/domain/account/Customer.h"
#include "core/domain/alert/FraudAlert.h"

/**
 * ============================================================================
 * [BƯỚC 1] TODO: THIẾT KẾ INTERFACE CƠ SỞ IFRAUD_RULE (STRATEGY PATTERN)
 * Tài liệu hướng dẫn: include/engine/01_STEP1_IFRAUD_RULE.md
 * ============================================================================
 * 
 * Mục tiêu:
 * - Áp dụng tính Đa hình (Polymorphism) và Abstract Class trong C++.
 * - Định nghĩa các hàm thuần ảo (= 0;) để mọi luật gian lận bắt buộc phải tuân theo.
 */
class IFraudRule {
public:
    // TODO 1: Khai báo Virtual Destructor mặc định để chống rò rỉ bộ nhớ khi hủy qua con trỏ cha
    // Gợi ý: virtual ~IFraudRule() = default;
    virtual ~IFraudRule() = default;

    // TODO 2: Khai báo hàm thuần ảo trả về mã ID định danh duy nhất của luật (vd: "RULE_004_DEVICE_RISK")
    // Gợi ý: virtual std::string getId() const = 0;
    virtual std::string getId() const = 0;

    // TODO 3: Khai báo hàm thuần ảo trả về tên mô tả ngắn gọn của luật
    // Gợi ý: virtual std::string getName() const = 0;
    virtual std::string getName() const = 0;

    // TODO 4: Khai báo hàm thuần ảo trả về nhóm danh mục luật (enum FraudRuleCategory trong Types.h)
    // Gợi ý: virtual FraudRuleCategory getCategory() const = 0;
    virtual FraudRuleCategory getCategory() const = 0;

    // TODO 5: Khai báo hàm thuần ảo trả về điểm rủi ro đóng góp nếu luật bị vi phạm (0.0 -> 100.0)
    // Gợi ý: virtual double getRiskContribution() const = 0;
    virtual double getRiskContribution() const = 0;

    // TODO 6: Khai báo hàm thuần ảo thẩm định giao dịch (Hàm quan trọng nhất)
    // - Tham số 1: const Transaction& tx (Chỉ đọc thông tin giao dịch)
    // - Tham số 2: const Customer& customer (Chỉ đọc hồ sơ khách hàng)
    // - Tham số 3: FraudAlert& out_alert (Tham số đầu ra để ghi dữ liệu vi phạm)
    // - Giá trị trả về: bool (true nếu vi phạm, false nếu an toàn)
    // Gợi ý: virtual bool evaluate(const Transaction& tx, const Customer& customer, FraudAlert& out_alert) = 0;
    virtual bool evaluate(
        const Transaction& tx,
        const Customer& customer,
        FraudAlert& out_alert
    ) = 0;
};
