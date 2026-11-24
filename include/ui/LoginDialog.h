#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "bll/Auth.h"
#include "ui/I18n.h"

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(AuthService* authService, QWidget *parent = nullptr);
    ~LoginDialog() override = default;

private slots:
    void handleLogin();
    void handleToggleLanguage();

private:
    void setupUi();
    void applyStyles();
    void retranslateUi();

    AuthService* authService_{nullptr};

    QLabel* lblSub_{nullptr};
    QLabel* lblIntroTitle_{nullptr};
    QLabel* lblIntroDesc_{nullptr};
    QLabel* lblUser_{nullptr};
    QLineEdit* txtUsername_{nullptr};
    QLabel* lblPass_{nullptr};
    QLineEdit* txtPassword_{nullptr};
    QLabel* lblError_{nullptr};
    QPushButton* btnLogin_{nullptr};
    QPushButton* btnCancel_{nullptr};
    QPushButton* btnLang_{nullptr};
};
