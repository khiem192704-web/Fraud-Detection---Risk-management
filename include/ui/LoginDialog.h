#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "bll/Auth.h"

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(AuthService* authService, QWidget *parent = nullptr);
    ~LoginDialog() override = default;

private slots:
    void handleLogin();
    void handleFillDemo();

private:
    void setupUi();
    void applyStyles();

    AuthService* authService_{nullptr};

    QLineEdit* txtUsername_{nullptr};
    QLineEdit* txtPassword_{nullptr};
    QLabel* lblError_{nullptr};
    QPushButton* btnLogin_{nullptr};
    QPushButton* btnFillDemo_{nullptr};
    QPushButton* btnCancel_{nullptr};
};
