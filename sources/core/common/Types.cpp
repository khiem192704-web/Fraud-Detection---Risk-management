#include "core/common/Types.h"

// ==================== toString Implementations ====================

std::string_view toString(PaymentType type) {
    switch (type) {
        case PaymentType::CARD:          return "CARD";
        case PaymentType::E_WALLET:      return "E_WALLET";
        case PaymentType::BANK_TRANSFER: return "BANK_TRANSFER";
        case PaymentType::CASH:          return "CASH";
        case PaymentType::CRYPTO:        return "CRYPTO";
        default:                         return "UNKNOWN";
    }
}

std::string_view toString(TransactionType type) {
    switch (type) {
        case TransactionType::BUY:      return "BUY";
        case TransactionType::TRANSFER: return "TRANSFER";
        case TransactionType::WITHDRAW: return "WITHDRAW";
        case TransactionType::TOPUP:    return "TOPUP";
        case TransactionType::REFUND:   return "REFUND";
        case TransactionType::PAY_BILL: return "PAY_BILL";
        default:                        return "UNKNOWN";
    }
}

std::string_view toString(TransactionStatus status) {
    switch (status) {
        case TransactionStatus::PENDING:        return "PENDING";
        case TransactionStatus::APPROVED:       return "APPROVED";
        case TransactionStatus::WAIT_REVIEW:    return "WAIT_REVIEW";
        case TransactionStatus::VERIFY_OTP:     return "VERIFY_OTP";
        case TransactionStatus::BLOCKED:        return "BLOCKED";
        case TransactionStatus::FAILED:         return "FAILED";
        case TransactionStatus::SUCCESS:        return "SUCCESS";
        case TransactionStatus::DISPUTED:       return "DISPUTED";
        case TransactionStatus::REFUNDED_FRAUD: return "REFUNDED_FRAUD";
        default:                                return "UNKNOWN";
    }
}

std::string_view toString(RiskLevel level) {
    switch (level) {
        case RiskLevel::VERY_LOW: return "VERY_LOW";
        case RiskLevel::LOW:      return "LOW";
        case RiskLevel::MEDIUM:   return "MEDIUM";
        case RiskLevel::HIGH:     return "HIGH";
        case RiskLevel::CRITICAL: return "CRITICAL";
        default:                  return "UNKNOWN";
    }
}

std::string_view toString(DecisionAction action) {
    switch (action) {
        case DecisionAction::APPROVE:       return "APPROVE";
        case DecisionAction::REVIEW:        return "REVIEW";
        case DecisionAction::CHALLENGE_3DS: return "CHALLENGE_3DS";
        case DecisionAction::BLOCK:         return "BLOCK";
        default:                            return "UNKNOWN";
    }
}

std::string_view toString(CustomerRiskTier tier) {
    switch (tier) {
        case CustomerRiskTier::NEW:         return "NEW";
        case CustomerRiskTier::ESTABLISHED: return "ESTABLISHED";
        case CustomerRiskTier::TRUSTED:     return "TRUSTED";
        case CustomerRiskTier::SUSPICIOUS:  return "SUSPICIOUS";
        case CustomerRiskTier::BANNED:      return "BANNED";
        default:                            return "UNKNOWN";
    }
}

std::string_view toString(FraudRuleCategory category) {
    switch (category) {
        case FraudRuleCategory::VELOCITY:         return "VELOCITY";
        case FraudRuleCategory::AMOUNT_DEVIATION: return "AMOUNT_DEVIATION";
        case FraudRuleCategory::GEO_LOCATION:     return "GEO_LOCATION";
        case FraudRuleCategory::DEVICE_INTEGRITY: return "DEVICE_INTEGRITY";
        case FraudRuleCategory::CARD_TESTING:     return "CARD_TESTING";
        case FraudRuleCategory::ACCOUNT_TAKEOVER: return "ACCOUNT_TAKEOVER";
        case FraudRuleCategory::LIST_MATCHING:    return "LIST_MATCHING";
        case FraudRuleCategory::BEHAVIORAL:       return "BEHAVIORAL";
        default:                                  return "UNKNOWN";
    }
}

std::string toString(MerchantCategory category) {
    switch (category) {
        case MerchantCategory::RETAIL:             return "RETAIL";
        case MerchantCategory::GROCERY:            return "GROCERY";
        case MerchantCategory::ELECTRONICS:        return "ELECTRONICS";
        case MerchantCategory::RESTAURANT:         return "RESTAURANT";
        case MerchantCategory::TRAVEL:             return "TRAVEL";
        case MerchantCategory::ENTERTAINMENT:      return "ENTERTAINMENT";
        case MerchantCategory::GAMBLING:           return "GAMBLING";
        case MerchantCategory::CRYPTO:             return "CRYPTO";
        case MerchantCategory::FINANCIAL_SERVICES: return "FINANCIAL_SERVICES";
        case MerchantCategory::OTHER:              return "OTHER";
        default:                                   return "UNKNOWN";
    }
}

std::string toString(MerchantRiskTier tier) {
    switch (tier) {
        case MerchantRiskTier::TRUSTED:     return "TRUSTED";
        case MerchantRiskTier::STANDARD:    return "STANDARD";
        case MerchantRiskTier::ELEVATED:    return "ELEVATED";
        case MerchantRiskTier::SUSPICIOUS:  return "SUSPICIOUS";
        case MerchantRiskTier::BLACKLISTED: return "BLACKLISTED";
        default:                            return "UNKNOWN";
    }
}

std::string toString(MerchantStatus status) {
    switch (status) {
        case MerchantStatus::ACTIVE:    return "ACTIVE";
        case MerchantStatus::SUSPENDED: return "SUSPENDED";
        default:                        return "UNKNOWN";
    }
}

// ==================== operator<< Implementations ====================

std::ostream& operator<<(std::ostream& os, PaymentType type) {
    return os << toString(type);
}

std::ostream& operator<<(std::ostream& os, TransactionType type) {
    return os << toString(type);
}

std::ostream& operator<<(std::ostream& os, TransactionStatus status) {
    return os << toString(status);
}

std::ostream& operator<<(std::ostream& os, RiskLevel level) {
    return os << toString(level);
}

std::ostream& operator<<(std::ostream& os, DecisionAction action) {
    return os << toString(action);
}

std::ostream& operator<<(std::ostream& os, CustomerRiskTier tier) {
    return os << toString(tier);
}

std::ostream& operator<<(std::ostream& os, FraudRuleCategory category) {
    return os << toString(category);
}
