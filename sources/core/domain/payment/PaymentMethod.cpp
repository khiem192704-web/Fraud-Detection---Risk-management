#include "core/domain/payment/PaymentMethod.h"

PaymentMethod::PaymentMethod(){}
PaymentMethod::PaymentMethod(const std::string& payment_id, PaymentType type, const std::string& raw_or_masked_pan, 
                        const std::string& card_holder_name, int expiry_month, int expiry_year, const std::string& billing_country)
                                : payment_id(payment_id), type(type), card_bin(""), last4(""), card_holder_name(card_holder_name), 
                                expiry_month(expiry_month), expiry_year(expiry_year), billing_country(billing_country) {
    if (type != PaymentType::CARD || raw_or_masked_pan.empty()) {
        masked_card_number = raw_or_masked_pan;
        return;
    }
    std::string clean_pan;
    for (char c : raw_or_masked_pan) {
        if ((c >= '0' && c <= '9') || c == '*') clean_pan.push_back(c);
    }

    if (clean_pan.length() >= 10) {
        card_bin = clean_pan.substr(0, 6);
        last4 = clean_pan.substr(clean_pan.length() - 4);
        masked_card_number = maskPan(clean_pan);
    } else {
        masked_card_number = raw_or_masked_pan;
        if (clean_pan.length() >= 4) {
            last4 = clean_pan.substr(clean_pan.length() - 4);
        }
    }
}

PaymentMethod PaymentMethod::createBankTransfer(const std::string& payment_id, const std::string& account_number, const std::string& bank_name){
    PaymentMethod pm;
    pm.payment_id = payment_id;
    pm.type = PaymentType::BANK_TRANSFER;
    pm.card_holder_name = bank_name;
    pm.masked_card_number = "ACC-" + (account_number.length() > 4 ? account_number.substr(account_number.length() - 4) : account_number);
    pm.last4 = account_number.length() >= 4 ? account_number.substr(account_number.length() - 4) : account_number;
    return pm;
}

PaymentMethod PaymentMethod::createEWallet(const std::string& payment_id, const std::string& wallet_id, const std::string& provider){
    PaymentMethod pm;
    pm.payment_id = payment_id;
    pm.type = PaymentType::E_WALLET;
    pm.card_holder_name = provider;
    pm.masked_card_number = "WALLET-" + (wallet_id.length() > 4 ? wallet_id.substr(wallet_id.length() - 4) : wallet_id);
    pm.last4 = wallet_id.length() >= 4 ? wallet_id.substr(wallet_id.length() - 4) : wallet_id;
    return pm;
}

void PaymentMethod::setPaymentId(const std::string& payment_id){ this->payment_id = payment_id; }
void PaymentMethod::setType(PaymentType type){ this->type = type; }
void PaymentMethod::setCardHolderName(const std::string& card_holder_name){ this->card_holder_name = card_holder_name; }
void PaymentMethod::setBillingCountry(const std::string& billing_country){ this->billing_country = billing_country; }

const std::string& PaymentMethod::getPaymentId() const { return payment_id; }
PaymentType PaymentMethod::getType() const { return type; }
const std::string& PaymentMethod::getMaskedCardNumber() const { return masked_card_number; }
const std::string& PaymentMethod::getCardBin() const { return card_bin; }
const std::string& PaymentMethod::getLast4() const { return last4; }
const std::string& PaymentMethod::getCardHolderName() const { return card_holder_name; }
int PaymentMethod::getExpiryMonth() const { return expiry_month; }
int PaymentMethod::getExpiryYear() const { return expiry_year; }
const std::string& PaymentMethod::getBillingCountry() const { return billing_country; }

bool PaymentMethod::isCard() const { return type == PaymentType::CARD; }
bool PaymentMethod::isExpired(int current_year, int current_month) const {
    if (!isCard() || expiry_year <= 0) return false;
    if (expiry_year < current_year) return true;
    if (expiry_year == current_year && expiry_month < current_month) return true;
    return false;
}

bool PaymentMethod::validateLuhn(const std::string& raw_pan){
    std::string digits = "";
    for (char c : raw_pan) {
        if (c >= '0' && c <= '9') digits.push_back(c);
    }
    if (digits.length() < 13 || digits.length() > 19) return false;

    int sum = 0;
    bool alternate = false;
    for (int i = static_cast<int>(digits.length()) - 1; i >= 0; --i) {
        int n = digits[i] - '0';
        if (alternate) {
            n *= 2;
            if (n > 9) n -= 9;
        }
        sum += n;
        alternate = !alternate;
    }
    return (sum % 10 == 0);
}

std::string PaymentMethod::maskPan(const std::string& raw_pan){
    std::string digits = "";
    for (char c : raw_pan) {
        if ((c >= '0' && c <= '9') || c == '*') digits.push_back(c);
    }
    if (digits.length() < 10) return digits;

    std::string masked = digits;
    for (std::size_t i = 6; i < digits.length() - 4; ++i) {
        masked[i] = '*';
    }
    return masked;
}

std::string PaymentMethod::toString() const {
    return "PaymentMethod{payment_id='" + payment_id + "', type=" + std::to_string(static_cast<int>(type))
        + ", masked_card_number='" + masked_card_number + "', card_bin='" + card_bin
        + "', last4='" + last4 + "', card_holder_name='" + card_holder_name
        + "', expiry=" + std::to_string(expiry_month) + "/" + std::to_string(expiry_year)
        + ", billing_country='" + billing_country + "'}";
}