#pragma once
#include<string>
#include"PaymentType.h"

class PaymentMethod {
private:
    std::string payment_id;
    PaymentType type{PaymentType::CARD};
    std::string masked_card_number;
    std::string card_bin;
    std::string last4;
    std::string card_holder_name;
    int expiry_month{0};
    int expiry_year{0};
    std::string billing_country;

public:
    // Constructor
    PaymentMethod();

    PaymentMethod(const std::string&, PaymentType, const std::string&, const std::string&, int, int, const std::string&);

    // Static Factory Methods
    static PaymentMethod createBankTransfer(const std::string&, const std::string&, const std::string&);

    static PaymentMethod createEWallet(const std::string&, const std::string&, const std::string&);

    // Getter
    const std::string& getPaymentId() const;
    PaymentType getType() const;
    const std::string& getMaskedCardNumber() const;
    const std::string& getCardBin() const;
    const std::string& getLast4() const;
    const std::string& getCardHolderName() const;
    int getExpiryMonth() const;
    int getExpiryYear() const;
    const std::string& getBillingCountry() const;

    // Setter
    void setPaymentId(const std::string&);
    void setType(PaymentType);
    void setCardHolderName(const std::string&);
    void setBillingCountry(const std::string&);

    // Domain Validations & Helpers
    bool isCard() const;
    bool isExpired(int, int) const;
    static bool validateLuhn(const std::string&);
    static std::string maskPan(const std::string&);

    std::string toString() const;
};