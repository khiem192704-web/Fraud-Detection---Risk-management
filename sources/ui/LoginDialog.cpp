#include "ui/LoginDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QMessageBox>
#include <QApplication>

LoginDialog::LoginDialog(AuthService* authService, QWidget *parent)
    : QDialog(parent), authService_(authService) {
    setWindowTitle(QString::fromUtf8("EPFD-RAS: Đăng Nhập Quản Trị Viên (Admin Portal)"));
    setFixedSize(480, 420);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    setupUi();
    applyStyles();

    // Kết nối Enter phím tắt
    connect(txtPassword_, &QLineEdit::returnPressed, this, &LoginDialog::handleLogin);
    connect(txtUsername_, &QLineEdit::returnPressed, this, &LoginDialog::handleLogin);
}

void LoginDialog::setupUi() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(28, 24, 28, 24);
    mainLayout->setSpacing(16);

    // 1. BANNER HEADER
    auto *headerWidget = new QWidget();
    headerWidget->setObjectName("loginHeader");
    auto *headerLayout = new QVBoxLayout(headerWidget);
    headerLayout->setContentsMargins(0, 0, 0, 8);
    headerLayout->setSpacing(6);

    auto *lblIcon = new QLabel(QString::fromUtf8("🛡️ EPFD-RAS SECURITY GATEWAY"));
    lblIcon->setObjectName("headerIcon");
    lblIcon->setAlignment(Qt::AlignCenter);

    auto *lblSub = new QLabel(QString::fromUtf8("Cổng Đăng Nhập Quản Trị Viên (Admin Portal)"));
    lblSub->setObjectName("headerSub");
    lblSub->setAlignment(Qt::AlignCenter);

    headerLayout->addWidget(lblIcon);
    headerLayout->addWidget(lblSub);
    mainLayout->addWidget(headerWidget);

    // 2. FORM INPUTS
    auto *formLayout = new QVBoxLayout();
    formLayout->setSpacing(10);

    auto *lblUser = new QLabel(QString::fromUtf8("Tài khoản Quản trị (Admin Username):"));
    lblUser->setObjectName("formLabel");
    txtUsername_ = new QLineEdit();
    txtUsername_->setPlaceholderText(QString::fromUtf8("Nhập username (vd: admin)..."));
    txtUsername_->setText("admin"); // Điền sẵn cho tiện lợi

    auto *lblPass = new QLabel(QString::fromUtf8("Mật khẩu (Password):"));
    lblPass->setObjectName("formLabel");
    txtPassword_ = new QLineEdit();
    txtPassword_->setEchoMode(QLineEdit::Password);
    txtPassword_->setPlaceholderText(QString::fromUtf8("Nhập mật khẩu..."));

    lblError_ = new QLabel();
    lblError_->setObjectName("errorLabel");
    lblError_->setAlignment(Qt::AlignCenter);
    lblError_->hide();

    formLayout->addWidget(lblUser);
    formLayout->addWidget(txtUsername_);
    formLayout->addWidget(lblPass);
    formLayout->addWidget(txtPassword_);
    formLayout->addWidget(lblError_);

    mainLayout->addLayout(formLayout);

    // 3. ACTION BUTTONS
    auto *btnLayout = new QVBoxLayout();
    btnLayout->setSpacing(8);

    btnLogin_ = new QPushButton(QString::fromUtf8("ĐĂNG NHẬP HỆ THỐNG"));
    btnLogin_->setObjectName("btnLogin");
    btnLogin_->setCursor(Qt::PointingHandCursor);
    connect(btnLogin_, &QPushButton::clicked, this, &LoginDialog::handleLogin);

    auto *subBtnLayout = new QHBoxLayout();
    subBtnLayout->setSpacing(10);

    btnFillDemo_ = new QPushButton(QString::fromUtf8("⚡ Điền Sẵn Admin"));
    btnFillDemo_->setObjectName("btnDemo");
    btnFillDemo_->setCursor(Qt::PointingHandCursor);
    connect(btnFillDemo_, &QPushButton::clicked, this, &LoginDialog::handleFillDemo);

    btnCancel_ = new QPushButton(QString::fromUtf8("Thoát"));
    btnCancel_->setObjectName("btnCancel");
    btnCancel_->setCursor(Qt::PointingHandCursor);
    connect(btnCancel_, &QPushButton::clicked, this, &QDialog::reject);

    subBtnLayout->addWidget(btnFillDemo_);
    subBtnLayout->addWidget(btnCancel_);

    btnLayout->addWidget(btnLogin_);
    btnLayout->addLayout(subBtnLayout);

    mainLayout->addLayout(btnLayout);

    // Chú thích phụ
    auto *lblNote = new QLabel(QString::fromUtf8("Hệ thống chỉ yêu cầu 1 quyền Admin duy nhất để quản trị rủi ro & dữ liệu phụ."));
    lblNote->setStyleSheet("color: #94A3B8; font-size: 11px; font-style: italic;");
    lblNote->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(lblNote);
}

void LoginDialog::applyStyles() {
    setStyleSheet(R"(
        QDialog {
            background-color: #0F172A;
        }
        #headerIcon {
            color: #38BDF8;
            font-size: 18px;
            font-weight: bold;
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
        #btnDemo {
            background-color: #334155;
            color: #38BDF8;
            font-size: 12px;
            font-weight: bold;
            padding: 8px;
            border-radius: 6px;
            border: 1px solid #475569;
        }
        #btnDemo:hover {
            background-color: #475569;
            color: #7DD3FC;
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

void LoginDialog::handleLogin() {
    std::string user = txtUsername_->text().trimmed().toStdString();
    std::string pass = txtPassword_->text().trimmed().toStdString();

    if (user.empty() || pass.empty()) {
        lblError_->setText(QString::fromUtf8("Vui lòng nhập đầy đủ tên đăng nhập và mật khẩu!"));
        lblError_->show();
        return;
    }

    if (authService_ && authService_->login(user, pass)) {
        lblError_->hide();
        accept(); // Đăng nhập thành công -> chuyển sang MainWindow
    } else {
        lblError_->setText(QString::fromUtf8("Sai tài khoản hoặc mật khẩu! Vui lòng thử lại."));
        lblError_->show();
    }
}

void LoginDialog::handleFillDemo() {
    txtUsername_->setText("admin");
    txtPassword_->setText("admin123");
    lblError_->hide();
}
