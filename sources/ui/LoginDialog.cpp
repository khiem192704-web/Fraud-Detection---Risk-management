#include "ui/LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QApplication>
#include "ui/I18n.h"

LoginDialog::LoginDialog(AuthService* authService, QWidget *parent)
    : QDialog(parent), authService_(authService) {
    setFixedSize(500, 530);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setupUi();
    applyStyles();
    retranslateUi();

    // Kết nối Enter phím tắt
    connect(txtPassword_, &QLineEdit::returnPressed, this, &LoginDialog::handleLogin);
    connect(txtUsername_, &QLineEdit::returnPressed, this, &LoginDialog::handleLogin);
}

void LoginDialog::setupUi() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 20, 28, 20);
    mainLayout->setSpacing(14);

    // Hàng nút chuyển đổi ngôn ngữ ở góc trên
    auto *topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(0, 0, 0, 0);
    topBarLayout->addStretch();

    btnLang_ = new QPushButton(this);
    btnLang_->setObjectName("btnLang");
    btnLang_->setCursor(Qt::PointingHandCursor);
    connect(btnLang_, &QPushButton::clicked, this, &LoginDialog::handleToggleLanguage);
    topBarLayout->addWidget(btnLang_);
    mainLayout->addLayout(topBarLayout);

    // 1. BANNER HEADER
    auto *headerWidget = new QWidget();
    headerWidget->setObjectName("loginHeader");
    auto *headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 4);
    headerLayout->setSpacing(4);

    auto *lblIcon = new QLabel("🛡️ PBL2 SECURITY GATEWAY");
    lblIcon->setObjectName("headerIcon");
    lblIcon->setAlignment(Qt::AlignCenter);

    lblSub_ = new QLabel();
    lblSub_->setObjectName("headerSub");
    lblSub_->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(lblIcon);
    headerLayout->addWidget(lblSub_);
    mainLayout->addWidget(headerWidget);

    // THẺ MÔ TẢ MỤC ĐÍCH HỆ THỐNG
    auto *introCard = new QWidget();
    introCard->setObjectName("loginIntroCard");
    auto *introLay = new QVBoxLayout(introCard);
    introLay->setContentsMargins(14, 10, 14, 10);
    introLay->setSpacing(4);

    lblIntroTitle_ = new QLabel();
    lblIntroTitle_->setObjectName("introTitle");
    lblIntroDesc_ = new QLabel();
    lblIntroDesc_->setObjectName("introDesc");
    lblIntroDesc_->setWordWrap(true);

    introLay->addWidget(lblIntroTitle_);
    introLay->addWidget(lblIntroDesc_);
    mainLayout->addWidget(introCard);

    // 2. FORM INPUTS
    auto *formLayout = new QVBoxLayout();
    formLayout->setSpacing(10);

    lblUser_ = new QLabel();
    lblUser_->setObjectName("formLabel");
    txtUsername_ = new QLineEdit();

    lblPass_ = new QLabel();
    lblPass_->setObjectName("formLabel");
    txtPassword_ = new QLineEdit();
    txtPassword_->setEchoMode(QLineEdit::Password);

    lblError_ = new QLabel();
    lblError_->setObjectName("errorLabel");
    lblError_->setAlignment(Qt::AlignCenter);
    lblError_->hide();

    formLayout->addWidget(lblUser_);
    formLayout->addWidget(txtUsername_);
    formLayout->addWidget(lblPass_);
    formLayout->addWidget(txtPassword_);
    formLayout->addWidget(lblError_);

    mainLayout->addLayout(formLayout);

    // 3. ACTION BUTTONS
    auto *btnLayout = new QVBoxLayout();
    btnLayout->setSpacing(8);

    btnLogin_ = new QPushButton();
    btnLogin_->setObjectName("btnLogin");
    btnLogin_->setCursor(Qt::PointingHandCursor);
    connect(btnLogin_, &QPushButton::clicked, this, &LoginDialog::handleLogin);

    auto *subBtnLayout = new QHBoxLayout();
    subBtnLayout->setSpacing(10);

    btnCancel_ = new QPushButton();
    btnCancel_->setObjectName("btnCancel");
    btnCancel_->setCursor(Qt::PointingHandCursor);
    connect(btnCancel_, &QPushButton::clicked, this, &QDialog::reject);

    subBtnLayout->addWidget(btnCancel_);

    btnLayout->addWidget(btnLogin_);
    btnLayout->addLayout(subBtnLayout);

    mainLayout->addLayout(btnLayout);
}

void LoginDialog::applyStyles() {
    setStyleSheet(R"(
        * {
            font-family: "Segoe UI", "Segoe UI Variable Text", "Inter", -apple-system, BlinkMacSystemFont, "Roboto", sans-serif;
        }
        QDialog {
            background-color: #0F172A;
        }
        #btnLang {
            background-color: #1E293B;
            color: #38BDF8;
            font-size: 12px;
            font-weight: 600;
            padding: 5px 12px;
            border-radius: 12px;
            border: 1px solid #334155;
        }
        #btnLang:hover {
            background-color: #334155;
            color: #7DD3FC;
        }
        #loginIntroCard {
            background-color: #131F3F;
            border: 1px solid #1E2D5A;
            border-radius: 8px;
        }
        #introTitle {
            color: #38BDF8;
            font-size: 12px;
            font-weight: bold;
        }
        #introDesc {
            color: #94A3B8;
            font-size: 12px;
            line-height: 1.45;
        }
        #headerIcon {
            color: #38BDF8;
            font-size: 20px;
            font-weight: 800;
            letter-spacing: 0.5px;
        }
        #headerSub {
            color: #94A3B8;
            font-size: 13px;
        }
        #formLabel {
            color: #E2E8F0;
            font-size: 13px;
            font-weight: 600;
        }
        QLineEdit {
            background-color: #1E293B;
            border: 1.5px solid #334155;
            border-radius: 6px;
            padding: 8px 12px;
            color: #F8FAFC;
            font-size: 13px;
        }
        QLineEdit:focus {
            border: 1.5px solid #0284C7;
            background-color: #1E293B;
        }
        #errorLabel {
            color: #EF4444;
            font-size: 12px;
            font-weight: bold;
            padding: 4px;
        }
        #btnLogin {
            background-color: #0284C7;
            color: #FFFFFF;
            font-size: 14px;
            font-weight: bold;
            padding: 10px;
            border-radius: 6px;
            border: none;
        }
        #btnLogin:hover {
            background-color: #0369A1;
        }
        #btnCancel {
            background-color: #334155;
            color: #CBD5E1;
            font-size: 12px;
            padding: 8px;
            border-radius: 6px;
            border: 1px solid #475569;
        }
        #btnCancel:hover {
            background-color: #475569;
            color: #F8FAFC;
        }
    )");
}

void LoginDialog::retranslateUi() {
    auto& i18n = I18n::instance();

    setWindowTitle(i18n.t(
        QString::fromUtf8("PBL2: Đăng Nhập Quản Trị"),
        "PBL2: Administrator Sign In"
    ));

    lblSub_->setText(i18n.t(
        QString::fromUtf8("Cổng Đăng Nhập Quản Trị Viên"),
        "Administrator Portal Gateway"
    ));

    lblIntroTitle_->setText(i18n.t(
        QString::fromUtf8("🎯 Vai trò của Hệ thống:"),
        "🎯 System Role & Mission:"
    ));

    lblIntroDesc_->setText(i18n.t(
        QString::fromUtf8("Giám sát và đánh giá rủi ro giao dịch thanh toán điện tử thời gian thực. Tự động phát hiện quẹt thẻ dồn dập trong 60 giây, tra cứu danh sách đen và dùng Học máy dự báo gian lận."),
        "Real-time electronic payment fraud detection and risk scoring. Detects 60s velocity bursts, looks up blacklists, and utilizes machine learning to prevent unauthorized card transactions."
    ));

    lblUser_->setText(i18n.t(
        QString::fromUtf8("Tài khoản Quản trị:"),
        "Admin Username:"
    ));

    txtUsername_->setPlaceholderText(i18n.t(
        QString::fromUtf8("Nhập username..."),
        "Enter username..."
    ));

    lblPass_->setText(i18n.t(
        QString::fromUtf8("Mật khẩu:"),
        "Password:"
    ));

    txtPassword_->setPlaceholderText(i18n.t(
        QString::fromUtf8("Nhập mật khẩu..."),
        "Enter password..."
    ));

    btnLogin_->setText(i18n.t(
        QString::fromUtf8("ĐĂNG NHẬP HỆ THỐNG"),
        "SIGN IN TO SYSTEM"
    ));

    btnCancel_->setText(i18n.t(
        QString::fromUtf8("Thoát"),
        "Exit"
    ));

    btnLang_->setText(i18n.t(
        QString::fromUtf8("🌐 Ngôn ngữ: Tiếng Việt"),
        "🌐 Language: English"
    ));
}

void LoginDialog::handleToggleLanguage() {
    I18n::instance().toggle();
    retranslateUi();
}

void LoginDialog::handleLogin() {
    std::string user = txtUsername_->text().trimmed().toStdString();
    std::string pass = txtPassword_->text().trimmed().toStdString();

    auto& i18n = I18n::instance();
    if (user.empty() || pass.empty()) {
        lblError_->setText(i18n.t(
            QString::fromUtf8("Vui lòng nhập đầy đủ tên đăng nhập và mật khẩu!"),
            "Please enter both username and password!"
        ));
        lblError_->show();
        return;
    }

    if (authService_ && authService_->login(user, pass)) {
        lblError_->hide();
        accept(); // Đăng nhập thành công -> chuyển sang MainWindow
    } else {
        lblError_->setText(i18n.t(
            QString::fromUtf8("Sai tài khoản hoặc mật khẩu! Vui lòng thử lại."),
            "Invalid username or password! Please try again."
        ));
        lblError_->show();
    }
}
