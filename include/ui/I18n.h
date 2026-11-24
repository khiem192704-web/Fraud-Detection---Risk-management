#pragma once
#include <QString>
#include <string>

enum class Language {
    VIETNAMESE,
    ENGLISH
};

class I18n {
public:
    static I18n& instance() {
        static I18n inst;
        return inst;
    }

    Language getLanguage() const { return lang_; }
    void setLanguage(Language lang) { lang_ = lang; }
    void toggle() {
        lang_ = (lang_ == Language::VIETNAMESE) ? Language::ENGLISH : Language::VIETNAMESE;
    }

    bool isVietnamese() const { return lang_ == Language::VIETNAMESE; }
    bool isEnglish() const { return lang_ == Language::ENGLISH; }

    QString t(const QString& vi, const QString& en) const {
        return (lang_ == Language::VIETNAMESE) ? vi : en;
    }

    std::string tStr(const std::string& vi, const std::string& en) const {
        return (lang_ == Language::VIETNAMESE) ? vi : en;
    }

private:
    I18n() : lang_(Language::VIETNAMESE) {}
    Language lang_{Language::VIETNAMESE};
};
