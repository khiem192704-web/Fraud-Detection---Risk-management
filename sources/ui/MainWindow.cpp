#include "ui/MainWindow.h"
#include "core/models/PaymentMethod.h"
#include "engine/rules/BlacklistRule.h"
#include <fstream>
#include <sstream>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFont>
#include <QDateTime>
#include <QInputDialog>
#include <QMenu>

MainWindow::MainWindow(AuthService* authService, QWidget *parent)
    : QMainWindow(parent),
      authService_(authService),
      mVinMart_("M_VINMART", "VinMart Supermarket", "5411", 5.0),
      mShopee_("M_SHOPEE", "Shopee Mall Official", "5311", 5.0),
      mCasino_("M_CASINO", "Royal Casino Macau", "7995", 85.0),
      mCrypto_("M_CRYPTO", "Binance P2P Crypto", "6051", 75.0),
      mDarknet_("M_DARKNET", "Darknet Scam Shop", "5967", 95.0, true) {

    setWindowTitle(QString::fromUtf8("EPFD-RAS: Electronic Payment Fraud Detection & Risk Management System"));
    resize(1440, 900);
    setMinimumSize(1280, 800);

    // 1. Nạp dữ liệu cốt lõi
    initSampleData();

    // 2. Khởi tạo giao diện đồ họa
    setupUi();
    applyThemeStyles();
}

void MainWindow::initSampleData() {
    // 1. Nạp danh sách Khách hàng từ Tầng Storage (data/customers.txt)
    bool custLoaded = customerRepo_.loadFromDisk("data/customers.txt");
    if (!custLoaded || customerRepo_.count() == 0) {
        Location locAlice("Vietnam", "Da Nang", 16.0544, 108.2022);
        Customer cAlice("CUST_ALICE", "Alice Nguyen", "alice@email.com", "0905123456", locAlice, CustomerRiskTier::TRUSTED, true);
        cAlice.addTrustedDevice("DEV_ALICE");
        cAlice.addLinkedAccount("ACC_ALICE");

        Location locBob("Vietnam", "Ha Noi", 21.0285, 105.8542);
        Customer cBob("CUST_BOB", "Bob Tran", "bob@email.com", "0912345678", locBob, CustomerRiskTier::NEW, false);
        cBob.addTrustedDevice("DEV_BOB");
        cBob.addLinkedAccount("ACC_BOB");

        Location locCharlie("Vietnam", "Ho Chi Minh", 10.8231, 106.6297);
        Customer cCharlie("CUST_CHARLIE", "Charlie Le", "charlie@email.com", "0988776655", locCharlie, CustomerRiskTier::SUSPICIOUS, false);
        cCharlie.addTrustedDevice("DEV_CHARLIE");
        cCharlie.addLinkedAccount("ACC_CHARLIE");

        customerRepo_.add(cAlice);
        customerRepo_.add(cBob);
        customerRepo_.add(cCharlie);
        customerRepo_.syncToDisk("data/customers.txt");
    }

    // 2. Nạp danh sách Tài khoản từ Tầng Storage (data/accounts.txt)
    bool accLoaded = accountRepo_.loadFromDisk("data/accounts.txt");
    if (!accLoaded || accountRepo_.count() == 0) {
        Account aAlice("ACC_ALICE", "CUST_ALICE", 2000.0, "USD");
        Account aBob("ACC_BOB", "CUST_BOB", 5000.0, "USD");
        Account aCharlie("ACC_CHARLIE", "CUST_CHARLIE", 8000.0, "USD");

        accountRepo_.add(aAlice);
        accountRepo_.add(aBob);
        accountRepo_.add(aCharlie);
        accountRepo_.syncToDisk("data/accounts.txt");
    }

    alice_ = customerRepo_.findById("CUST_ALICE");
    bob_ = customerRepo_.findById("CUST_BOB");
    charlie_ = customerRepo_.findById("CUST_CHARLIE");

    accAlice_ = accountRepo_.findById("ACC_ALICE");
    accBob_ = accountRepo_.findById("ACC_BOB");
    accCharlie_ = accountRepo_.findById("ACC_CHARLIE");

    // 3. Nạp lịch sử giao dịch từ TransactionRepository (data/transactions.csv)
    transactionRepo_.loadFromDisk("data/transactions.csv");
}

void MainWindow::setupUi() {
    auto *centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    setCentralWidget(centralWidget);

    auto *rootLayout = new QVBoxLayout(centralWidget);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // 1. TOP HEADER (Toàn bộ chiều ngang phía trên)
    rootLayout->addWidget(createTopHeader());

    // 2. PHẦN THÂN: LEFT SIDEBAR + MAIN STACK
    auto *bodyWidget = new QWidget();
    auto *bodyLayout = new QHBoxLayout(bodyWidget);
    bodyLayout->setContentsMargins(0, 0, 0, 0);
    bodyLayout->setSpacing(0);

    // Sidebar bên trái
    bodyLayout->addWidget(createLeftSidebar());

    // Stack hiển thị nội dung chính
    mainStack_ = new QStackedWidget();
    mainStack_->addWidget(createDashboardPage());             // Trang 0: Tổng quan (Exact match ảnh người dùng)
    mainStack_->addWidget(createTransactionManagementPage());  // Trang 1: Giao dịch & Quản lý Tài khoản / Khách hàng
    mainStack_->addWidget(createRiskEnginePage());             // Trang 2: Động cơ rủi ro & Review Queue
    mainStack_->addWidget(createBlacklistPage());              // Trang 3: Luật gian lận & Blacklist HashMap
    mainStack_->addWidget(createMlDatasetPage());              // Trang 4: ML & Dữ liệu

    bodyLayout->addWidget(mainStack_, 1);
    rootLayout->addWidget(bodyWidget, 1);
}

// =========================================================================
// 1. TOP HEADER COMPONENT
// =========================================================================
QWidget* MainWindow::createTopHeader() {
    auto *header = new QWidget();
    header->setObjectName("topHeader");
    header->setFixedHeight(58);

    auto *lay = new QHBoxLayout(header);
    lay->setContentsMargins(18, 0, 18, 0);
    lay->setSpacing(16);

    // Logo & Brand Name
    auto *logoBox = new QWidget();
    auto *logoLay = new QHBoxLayout(logoBox);
    logoLay->setContentsMargins(0, 0, 0, 0);
    logoLay->setSpacing(10);

    auto *lblShieldIcon = new QLabel(QString::fromUtf8("🛡️"));
    lblShieldIcon->setStyleSheet("font-size: 26px; color: #38BDF8;");

    auto *titleBox = new QWidget();
    auto *titleLay = new QVBoxLayout(titleBox);
    titleLay->setContentsMargins(0, 0, 0, 0);
    titleLay->setSpacing(0);

    auto *lblMainTitle = new QLabel("EPFD-RAS");
    lblMainTitle->setStyleSheet("color: #FFFFFF; font-size: 16px; font-weight: 800; letter-spacing: 0.8px;");

    auto *lblSubTitle = new QLabel("Electronic Payment Fraud Detection & Risk Management System");
    lblSubTitle->setStyleSheet("color: #64748B; font-size: 10px; font-weight: 500;");

    titleLay->addWidget(lblMainTitle);
    titleLay->addWidget(lblSubTitle);

    logoLay->addWidget(lblShieldIcon);
    logoLay->addWidget(titleBox);
    lay->addWidget(logoBox);

    lay->addStretch();

    // Thanh tìm kiếm nhanh ở giữa (Search Bar + Ctrl + K badge)
    auto *searchBox = new QWidget();
    searchBox->setObjectName("searchContainer");
    searchBox->setFixedWidth(420);
    searchBox->setFixedHeight(36);
    auto *searchLay = new QHBoxLayout(searchBox);
    searchLay->setContentsMargins(10, 0, 10, 0);
    searchLay->setSpacing(8);

    auto *lblSearchIcon = new QLabel(QString::fromUtf8("🔍"));
    lblSearchIcon->setStyleSheet("color: #64748B; font-size: 13px;");

    txtSearch_ = new QLineEdit();
    txtSearch_->setObjectName("headerSearchInput");
    txtSearch_->setPlaceholderText(QString::fromUtf8("Tìm kiếm giao dịch theo ID, thẻ, tài khoản, IP .."));
    txtSearch_->setStyleSheet("background: transparent; border: none; color: #F8FAFC; font-size: 12px;");
    connect(txtSearch_, &QLineEdit::textChanged, this, &MainWindow::handleSearchFilter);

    auto *lblKbd = new QLabel("Ctrl + K");
    lblKbd->setObjectName("kbdBadge");
    lblKbd->setStyleSheet("background-color: #1E293B; color: #94A3B8; font-size: 10px; padding: 2px 6px; border-radius: 4px; border: 1px solid #334155;");

    searchLay->addWidget(lblSearchIcon);
    searchLay->addWidget(txtSearch_, 1);
    searchLay->addWidget(lblKbd);
    lay->addWidget(searchBox);

    lay->addStretch();

    // Chuông thông báo (Notification Bell with Badge 3)
    auto *bellBtn = new QPushButton(QString::fromUtf8("🔔 3"));
    bellBtn->setObjectName("bellButton");
    bellBtn->setCursor(Qt::PointingHandCursor);
    bellBtn->setStyleSheet("background-color: #131F3F; color: #EF4444; font-weight: bold; font-size: 12px; border: 1px solid #1E2D5A; border-radius: 16px; padding: 4px 10px;");
    lay->addWidget(bellBtn);

    // Trạng thái hệ thống: "🟢 Engine sẵn sàng"
    auto *engineStatusPill = new QWidget();
    engineStatusPill->setObjectName("engineStatusPill");
    engineStatusPill->setFixedHeight(32);
    auto *stLay = new QHBoxLayout(engineStatusPill);
    stLay->setContentsMargins(10, 0, 10, 0);
    stLay->setSpacing(6);

    auto *lblDot = new QLabel("●");
    lblDot->setStyleSheet("color: #10B981; font-size: 14px;");
    auto *lblEngineReady = new QLabel(QString::fromUtf8("Engine sẵn sàng"));
    lblEngineReady->setStyleSheet("color: #E2E8F0; font-size: 11px; font-weight: 600;");

    stLay->addWidget(lblDot);
    stLay->addWidget(lblEngineReady);
    lay->addWidget(engineStatusPill);

    // Hồ sơ Quản trị viên: "👤 Admin ▼"
    std::string adminDisplay = "Admin";
    if (authService_ && authService_->isAuthenticated()) {
        adminDisplay = authService_->getSessionUsername();
    }

    auto *adminBtn = new QPushButton(QString::fromUtf8("👤 %1  ▼").arg(QString::fromStdString(adminDisplay)));
    adminBtn->setObjectName("adminProfileBtn");
    adminBtn->setCursor(Qt::PointingHandCursor);
    adminBtn->setFixedHeight(32);
    adminBtn->setStyleSheet("background-color: #131F3F; color: #F8FAFC; border: 1px solid #1E2D5A; border-radius: 16px; padding: 4px 14px; font-weight: bold; font-size: 12px;");

    // Menu dropdown khi click vào Admin
    auto *adminMenu = new QMenu(this);
    adminMenu->setStyleSheet("background-color: #131F3F; color: #F8FAFC; border: 1px solid #1E2D5A; padding: 4px;");
    auto *actInfo = adminMenu->addAction(QString::fromUtf8("👤 Vai trò: Quản trị viên rủi ro"));
    actInfo->setEnabled(false);
    adminMenu->addSeparator();
    auto *actLogout = adminMenu->addAction(QString::fromUtf8("🚪 Đăng xuất"));
    connect(actLogout, &QAction::triggered, this, &MainWindow::handleLogout);
    adminBtn->setMenu(adminMenu);

    lay->addWidget(adminBtn);

    return header;
}

// =========================================================================
// 2. LEFT SIDEBAR COMPONENT
// =========================================================================
QWidget* MainWindow::createLeftSidebar() {
    auto *sidebar = new QWidget();
    sidebar->setObjectName("leftSidebar");
    sidebar->setFixedWidth(190);

    auto *lay = new QVBoxLayout(sidebar);
    lay->setContentsMargins(10, 16, 10, 16);
    lay->setSpacing(6);

    auto addNavItem = [this, lay](int pageIdx, const QString& icon, const QString& label) {
        auto *btn = new QPushButton(QString("%1  %2").arg(icon, label));
        btn->setObjectName(pageIdx == 0 ? "navItemActive" : "navItem");
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedHeight(38);
        btn->setStyleSheet(pageIdx == 0 ?
            "QPushButton { background-color: #0284C7; color: #FFFFFF; font-weight: bold; border-radius: 8px; text-align: left; padding-left: 14px; border: none; font-size: 12px; }" :
            "QPushButton { background: transparent; color: #94A3B8; font-weight: 600; border-radius: 8px; text-align: left; padding-left: 14px; border: none; font-size: 12px; }"
            "QPushButton:hover { background-color: #131F3F; color: #F8FAFC; }"
        );

        connect(btn, &QPushButton::clicked, [this, pageIdx]() {
            handleNavButtonClicked(pageIdx);
        });

        navButtons_.push_back(btn);
        lay->addWidget(btn);
    };

    addNavItem(0, QString::fromUtf8("🏠"), QString::fromUtf8("Tổng quan"));
    addNavItem(1, QString::fromUtf8("⇄"), QString::fromUtf8("Giao dịch"));
    addNavItem(2, QString::fromUtf8("🛡️"), QString::fromUtf8("Động cơ rủi ro"));
    addNavItem(3, QString::fromUtf8("📋"), QString::fromUtf8("Luật gian lận"));
    addNavItem(4, QString::fromUtf8("🤖"), QString::fromUtf8("ML & Dữ liệu"));
    addNavItem(0, QString::fromUtf8("▶"), QString::fromUtf8("Demo kịch bản"));

    lay->addStretch();

    // Logo thương hiệu ở chân Sidebar (Glowing Shield)
    auto *brandCard = new QWidget();
    brandCard->setObjectName("brandCard");
    auto *bLay = new QVBoxLayout(brandCard);
    bLay->setContentsMargins(10, 12, 10, 12);
    bLay->setSpacing(4);
    bLay->setAlignment(Qt::AlignCenter);

    auto *lblBigShield = new QLabel(QString::fromUtf8("🛡️"));
    lblBigShield->setAlignment(Qt::AlignCenter);
    lblBigShield->setStyleSheet("font-size: 34px; color: #0284C7;");

    auto *lblBrandName = new QLabel("EPFD-RAS");
    lblBrandName->setAlignment(Qt::AlignCenter);
    lblBrandName->setStyleSheet("color: #FFFFFF; font-weight: bold; font-size: 12px; letter-spacing: 0.5px;");

    auto *lblSlogan1 = new QLabel("Secure Payments");
    lblSlogan1->setAlignment(Qt::AlignCenter);
    lblSlogan1->setStyleSheet("color: #64748B; font-size: 10px;");

    auto *lblSlogan2 = new QLabel("Safer Tomorrow");
    lblSlogan2->setAlignment(Qt::AlignCenter);
    lblSlogan2->setStyleSheet("color: #64748B; font-size: 10px;");

    bLay->addWidget(lblBigShield);
    bLay->addWidget(lblBrandName);
    bLay->addWidget(lblSlogan1);
    bLay->addWidget(lblSlogan2);

    lay->addWidget(brandCard);

    return sidebar;
}

void MainWindow::handleNavButtonClicked(int pageIndex) {
    currentPageIndex_ = pageIndex;
    mainStack_->setCurrentIndex(pageIndex);

    for (size_t i = 0; i < navButtons_.size(); ++i) {
        if (static_cast<int>(i) == pageIndex) {
            navButtons_[i]->setStyleSheet(
                "QPushButton { background-color: #0284C7; color: #FFFFFF; font-weight: bold; border-radius: 8px; text-align: left; padding-left: 14px; border: none; font-size: 12px; }"
            );
        } else {
            navButtons_[i]->setStyleSheet(
                "QPushButton { background: transparent; color: #94A3B8; font-weight: 600; border-radius: 8px; text-align: left; padding-left: 14px; border: none; font-size: 12px; }"
                "QPushButton:hover { background-color: #131F3F; color: #F8FAFC; }"
            );
        }
    }
}

// =========================================================================
// 3. TRANG DASHBOARD CHÍNH (TỔNG QUAN) - KHỚP 100% ẢNH MẪU
// =========================================================================
QWidget* MainWindow::createDashboardPage() {
    auto *page = new QWidget();
    auto *pageLayout = new QHBoxLayout(page);
    pageLayout->setContentsMargins(14, 12, 14, 12);
    pageLayout->setSpacing(14);

    // Cột trung tâm (Center Main Dashboard)
    auto *centerPanel = new QWidget();
    auto *centerLay = new QVBoxLayout(centerPanel);
    centerLay->setContentsMargins(0, 0, 0, 0);
    centerLay->setSpacing(10);

    // Tiêu đề: "Trung tâm phát hiện gian lận" + "C++ Native Engine · Python ML"
    auto *titleRow = new QHBoxLayout();
    auto *headBox = new QWidget();
    auto *hLay = new QVBoxLayout(headBox);
    hLay->setContentsMargins(0, 0, 0, 0);
    hLay->setSpacing(2);

    auto *lblTitle = new QLabel(QString::fromUtf8("Trung tâm phát hiện gian lận"));
    lblTitle->setStyleSheet("color: #FFFFFF; font-size: 17px; font-weight: bold;");

    auto *lblSub = new QLabel("C++ Native Engine · Python ML");
    lblSub->setStyleSheet("color: #38BDF8; font-size: 12px; font-weight: 600;");

    hLay->addWidget(lblTitle);
    hLay->addWidget(lblSub);
    titleRow->addWidget(headBox);
    titleRow->addStretch();

    lblLastUpdated_ = new QLabel(QString::fromUtf8("Cập nhật lần cuối: %1 🔄").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm")));
    lblLastUpdated_->setStyleSheet("color: #64748B; font-size: 11px;");
    titleRow->addWidget(lblLastUpdated_);

    centerLay->addLayout(titleRow);

    // 1. Hàng 4 thẻ chỉ số KPI Summary
    centerLay->addWidget(createKpiRow());

    // 2. Hàng 2 biểu đồ (Trend Line Chart + Donut Decision Chart)
    centerLay->addWidget(createChartsRow());

    // 3. Bảng "Giao dịch gần đây"
    centerLay->addWidget(createRecentTransactionsSection(), 1);

    // 4. Thanh kích hoạt nhanh 3 kịch bản Demo ở đáy màn hình
    centerLay->addWidget(createBottomScenarioBar());

    pageLayout->addWidget(centerPanel, 1);

    // Cột bên phải: Cảnh báo ưu tiên, Bộ luật đang bật, Native ML Predictor
    pageLayout->addWidget(createRightPanel());

    return page;
}

// =========================================================================
// DASHBOARD: ROW 1 - 4 KPI METRIC CARDS
// =========================================================================
QWidget* MainWindow::createKpiRow() {
    auto *row = new QWidget();
    auto *lay = new QHBoxLayout(row);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(10);

    lay->addWidget(createKpiCard(QString::fromUtf8("📄"), QString::fromUtf8("Giao dịch mẫu"), "5,000",
                                QString::fromUtf8("(dữ liệu demo)"), "#0284C7", "#FFFFFF",
                                {100, 110, 125, 115, 130, 140, 155, 170}, &sparkSample_));

    lay->addWidget(createKpiCard(QString::fromUtf8("✔️"), QString::fromUtf8("Được chấp nhận"), "4,612",
                                QString::fromUtf8("(dữ liệu demo)"), "#10B981", "#FFFFFF",
                                {90, 100, 120, 110, 125, 130, 145, 160}, &sparkApproved_));

    lay->addWidget(createKpiCard(QString::fromUtf8("⚠️"), QString::fromUtf8("Cần rà soát"), "276",
                                QString::fromUtf8("(dữ liệu demo)"), "#F59E0B", "#FFFFFF",
                                {15, 20, 18, 25, 22, 28, 24, 32}, &sparkReview_));

    lay->addWidget(createKpiCard(QString::fromUtf8("🛡️"), QString::fromUtf8("Bị chặn"), "112",
                                QString::fromUtf8("(dữ liệu demo)"), "#EF4444", "#FFFFFF",
                                {8, 12, 10, 14, 11, 15, 12, 18}, &sparkBlocked_));

    return row;
}

QWidget* MainWindow::createKpiCard(const QString& iconText, const QString& title, const QString& value,
                                  const QString& subText, const QString& iconBgColor, const QString& valColor,
                                  const std::vector<double>& sparkData, SparklineWidget** outSpark) {
    auto *card = new QWidget();
    card->setObjectName("kpiCard");
    card->setFixedHeight(86);
    auto *lay = new QHBoxLayout(card);
    lay->setContentsMargins(12, 10, 12, 10);
    lay->setSpacing(10);

    // Icon vuông bo tròn góc
    auto *iconLbl = new QLabel(iconText);
    iconLbl->setFixedSize(38, 38);
    iconLbl->setAlignment(Qt::AlignCenter);
    iconLbl->setStyleSheet(QString("background-color: %1; color: white; border-radius: 8px; font-size: 17px;").arg(iconBgColor));

    auto *infoBox = new QWidget();
    auto *iLay = new QVBoxLayout(infoBox);
    iLay->setContentsMargins(0, 0, 0, 0);
    iLay->setSpacing(1);

    auto *lblTitle = new QLabel(title);
    lblTitle->setStyleSheet("color: #94A3B8; font-size: 11px; font-weight: 600;");

    auto *lblVal = new QLabel(value);
    lblVal->setStyleSheet(QString("color: %1; font-size: 19px; font-weight: bold;").arg(valColor));

    auto *lblSub = new QLabel(subText);
    lblSub->setStyleSheet("color: #64748B; font-size: 10px;");

    iLay->addWidget(lblTitle);
    iLay->addWidget(lblVal);
    iLay->addWidget(lblSub);

    lay->addWidget(iconLbl);
    lay->addWidget(infoBox);
    lay->addStretch();

    // Sparkline đồ thị tia ở góc phải thẻ
    auto *spark = new SparklineWidget(sparkData, QColor(iconBgColor));
    if (outSpark) *outSpark = spark;
    lay->addWidget(spark);

    // Lưu con trỏ label để cập nhật số liệu thời gian thực
    if (title.contains(QString::fromUtf8("mẫu"))) lblKpiSample_ = lblVal;
    else if (title.contains(QString::fromUtf8("nhận"))) lblKpiApproved_ = lblVal;
    else if (title.contains(QString::fromUtf8("soát"))) lblKpiReview_ = lblVal;
    else if (title.contains(QString::fromUtf8("chặn"))) lblKpiBlocked_ = lblVal;

    return card;
}

// =========================================================================
// DASHBOARD: ROW 2 - 2 CHARTS (TREND + DONUT)
// =========================================================================
QWidget* MainWindow::createChartsRow() {
    auto *row = new QWidget();
    auto *lay = new QHBoxLayout(row);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(10);

    // Biểu đồ 1: Giao dịch theo thời gian
    auto *cardTrend = new QWidget();
    cardTrend->setObjectName("dashboardCard");
    auto *tLay = new QVBoxLayout(cardTrend);
    tLay->setContentsMargins(12, 10, 12, 10);
    tLay->setSpacing(6);

    auto *headTrend = new QHBoxLayout();
    auto *lblTrendTitle = new QLabel(QString::fromUtf8("📈 Giao dịch theo thời gian"));
    lblTrendTitle->setStyleSheet("color: #FFFFFF; font-size: 12px; font-weight: bold;");

    auto *lblLegValid = new QLabel(QString::fromUtf8("● Giao dịch hợp lệ"));
    lblLegValid->setStyleSheet("color: #0284C7; font-size: 10px; font-weight: 600;");

    auto *lblLegFraud = new QLabel(QString::fromUtf8("● Giao dịch đáng ngờ"));
    lblLegFraud->setStyleSheet("color: #EF4444; font-size: 10px; font-weight: 600;");

    auto *cmbDays = new QComboBox();
    cmbDays->setObjectName("filterDropdown");
    cmbDays->addItem(QString::fromUtf8("7 ngày qua"));
    cmbDays->addItem(QString::fromUtf8("30 ngày qua"));
    cmbDays->setFixedWidth(95);
    cmbDays->setFixedHeight(24);

    headTrend->addWidget(lblTrendTitle);
    headTrend->addSpacing(8);
    headTrend->addWidget(lblLegValid);
    headTrend->addSpacing(4);
    headTrend->addWidget(lblLegFraud);
    headTrend->addStretch();
    headTrend->addWidget(cmbDays);

    tLay->addLayout(headTrend);

    trendChart_ = new TrendChartWidget();
    tLay->addWidget(trendChart_);

    lay->addWidget(cardTrend, 6);

    // Biểu đồ 2: Phân bố quyết định (Donut Chart)
    auto *cardDonut = new QWidget();
    cardDonut->setObjectName("dashboardCard");
    auto *dLay = new QVBoxLayout(cardDonut);
    dLay->setContentsMargins(12, 10, 12, 10);
    dLay->setSpacing(6);

    auto *lblDonutTitle = new QLabel(QString::fromUtf8("⚙️ Phân bố quyết định"));
    lblDonutTitle->setStyleSheet("color: #FFFFFF; font-size: 12px; font-weight: bold;");
    dLay->addWidget(lblDonutTitle);

    auto *donutContentLay = new QHBoxLayout();
    donutChart_ = new DonutChartWidget();
    donutChart_->setFixedSize(140, 140);
    donutContentLay->addWidget(donutChart_);
    donutContentLay->addSpacing(8);

    // Legend bên phải Donut
    auto *legBox = new QWidget();
    auto *lLay = new QVBoxLayout(legBox);
    lLay->setContentsMargins(0, 0, 0, 0);
    lLay->setSpacing(6);
    lLay->setAlignment(Qt::AlignVCenter);

    auto createLegendItem = [](const QString& colorHex, const QString& title, const QString& pct, const QString& count) -> QLabel* {
        auto *lbl = new QLabel();
        lbl->setText(QString("<span style='color:%1; font-size: 12px;'>●</span> <span style='color:#FFFFFF; font-weight:bold; font-size: 11px;'>%2</span> &nbsp; <span style='color:#94A3B8; font-size: 10px;'>%3</span><br>&nbsp;&nbsp;&nbsp;<b style='color:#E2E8F0; font-size: 11px;'>%4</b>")
                     .arg(colorHex, title, pct, count));
        return lbl;
    };

    lblLegendApproved_ = createLegendItem("#10B981", "APPROVE", "92.2%", "4,612");
    lblLegendReview_ = createLegendItem("#F59E0B", "REVIEW", "5.5%", "276");
    lblLegendBlocked_ = createLegendItem("#EF4444", "BLOCK", "2.2%", "112");

    lLay->addWidget(lblLegendApproved_);
    lLay->addWidget(lblLegendReview_);
    lLay->addWidget(lblLegendBlocked_);

    donutContentLay->addWidget(legBox);
    donutContentLay->addStretch();

    dLay->addLayout(donutContentLay);

    lay->addWidget(cardDonut, 4);

    return row;
}

// =========================================================================
// DASHBOARD: ROW 3 - RECENT TRANSACTIONS TABLE
// =========================================================================
QWidget* MainWindow::createRecentTransactionsSection() {
    auto *box = new QWidget();
    box->setObjectName("dashboardCard");
    auto *lay = new QVBoxLayout(box);
    lay->setContentsMargins(12, 10, 12, 8);
    lay->setSpacing(6);

    auto *lblTitle = new QLabel(QString::fromUtf8("💳 Giao dịch gần đây"));
    lblTitle->setStyleSheet("color: #FFFFFF; font-size: 12px; font-weight: bold;");
    lay->addWidget(lblTitle);

    tableRecent_ = new QTableWidget();
    tableRecent_->setObjectName("recentTransactionsTable");
    tableRecent_->setColumnCount(9);
    tableRecent_->setHorizontalHeaderLabels({
        "#", QString::fromUtf8("ID giao dịch"), QString::fromUtf8("Thời gian"),
        QString::fromUtf8("Số tiền"), QString::fromUtf8("Velocity"), QString::fromUtf8("Device risk"),
        QString::fromUtf8("ML probability"), QString::fromUtf8("Risk score"), QString::fromUtf8("Quyết định")
    });

    tableRecent_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableRecent_->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableRecent_->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableRecent_->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableRecent_->setSelectionMode(QAbstractItemView::SingleSelection);
    tableRecent_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableRecent_->setAlternatingRowColors(true);
    tableRecent_->verticalHeader()->setVisible(false);

    connect(tableRecent_, &QTableWidget::cellClicked, this, &MainWindow::handleRecentTableClicked);

    // Nạp sẵn 10 bản ghi chuẩn chỉnh đúng như trong ảnh người dùng tải lên
    addTransactionToRecentTable("TXN-20260624-1001", "24/06/2026 14:20", 2500.0, "3/60s", "Low", 0.12, 18, "APPROVE");
    addTransactionToRecentTable("TXN-20260624-1002", "24/06/2026 14:17", 320.0, "1/60s", "Low", 0.08, 12, "APPROVE");
    addTransactionToRecentTable("TXN-20260624-1003", "24/06/2026 14:15", 8200.0, "2/60s", "Medium", 0.64, 72, "REVIEW");
    addTransactionToRecentTable("TXN-20260624-1004", "24/06/2026 14:12", 150.0, "1/60s", "Low", 0.05, 8, "APPROVE");
    addTransactionToRecentTable("TXN-20260624-1005", "24/06/2026 14:08", 1200.0, "5/60s", "High", 0.78, 86, "BLOCK");
    addTransactionToRecentTable("TXN-20260624-1006", "24/06/2026 14:03", 680.0, "1/60s", "Medium", 0.42, 45, "REVIEW");
    addTransactionToRecentTable("TXN-20260624-1007", "24/06/2026 13:58", 420.0, "2/60s", "Low", 0.15, 20, "APPROVE");
    addTransactionToRecentTable("TXN-20260624-1008", "24/06/2026 13:52", 5600.0, "4/60s", "High", 0.71, 79, "BLOCK");
    addTransactionToRecentTable("TXN-20260624-1009", "24/06/2026 13:47", 310.0, "1/60s", "Low", 0.09, 14, "APPROVE");
    addTransactionToRecentTable("TXN-20260624-1010", "24/06/2026 13:40", 900.0, "3/60s", "Medium", 0.33, 38, "REVIEW");

    lay->addWidget(tableRecent_);
    return box;
}

void MainWindow::addTransactionToRecentTable(const QString& id, const QString& time, double amount,
                                            const QString& velocity, const QString& deviceRisk,
                                            double mlProb, int riskScore, const QString& decision) {
    int r = tableRecent_->rowCount();
    tableRecent_->insertRow(r);

    auto *numItem = new QTableWidgetItem(QString::number(r + 1));
    numItem->setTextAlignment(Qt::AlignCenter);
    numItem->setForeground(QColor("#64748B"));
    tableRecent_->setItem(r, 0, numItem);

    auto *idItem = new QTableWidgetItem(id);
    idItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    idItem->setForeground(QColor("#38BDF8"));
    tableRecent_->setItem(r, 1, idItem);

    auto *timeItem = new QTableWidgetItem(time);
    timeItem->setTextAlignment(Qt::AlignCenter);
    timeItem->setForeground(QColor("#CBD5E1"));
    tableRecent_->setItem(r, 2, timeItem);

    auto *amtItem = new QTableWidgetItem(QString("$%1").arg(amount, 0, 'f', 0));
    amtItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    amtItem->setFont(QFont("Segoe UI", 9, QFont::Bold));
    amtItem->setForeground(QColor("#FFFFFF"));
    tableRecent_->setItem(r, 3, amtItem);

    auto *velItem = new QTableWidgetItem(velocity);
    velItem->setTextAlignment(Qt::AlignCenter);
    velItem->setForeground(QColor("#94A3B8"));
    tableRecent_->setItem(r, 4, velItem);

    auto *devItem = new QTableWidgetItem(deviceRisk);
    devItem->setTextAlignment(Qt::AlignCenter);
    devItem->setFont(QFont("Segoe UI", 9, QFont::Bold));
    if (deviceRisk == "Low") devItem->setForeground(QColor("#10B981"));
    else if (deviceRisk == "Medium") devItem->setForeground(QColor("#F59E0B"));
    else devItem->setForeground(QColor("#EF4444"));
    tableRecent_->setItem(r, 5, devItem);

    auto *mlItem = new QTableWidgetItem(QString::number(mlProb, 'f', 2));
    mlItem->setTextAlignment(Qt::AlignCenter);
    mlItem->setForeground(QColor("#CBD5E1"));
    tableRecent_->setItem(r, 6, mlItem);

    auto *scoreItem = new QTableWidgetItem(QString::number(riskScore));
    scoreItem->setTextAlignment(Qt::AlignCenter);
    scoreItem->setFont(QFont("Segoe UI", 9, QFont::Bold));
    if (riskScore < 30) scoreItem->setForeground(QColor("#10B981"));
    else if (riskScore < 70) scoreItem->setForeground(QColor("#F59E0B"));
    else scoreItem->setForeground(QColor("#EF4444"));
    tableRecent_->setItem(r, 7, scoreItem);

    auto *decItem = new QTableWidgetItem(decision);
    decItem->setTextAlignment(Qt::AlignCenter);
    decItem->setFont(QFont("Segoe UI", 9, QFont::Bold));
    if (decision == "APPROVE") {
        decItem->setBackground(QColor("#064E3B"));
        decItem->setForeground(QColor("#34D399"));
    } else if (decision == "REVIEW") {
        decItem->setBackground(QColor("#78350F"));
        decItem->setForeground(QColor("#FCD34D"));
    } else {
        decItem->setBackground(QColor("#7F1D1D"));
        decItem->setForeground(QColor("#F87171"));
    }
    tableRecent_->setItem(r, 8, decItem);
}

void MainWindow::handleSearchFilter(const QString& text) {
    QString filter = text.trimmed().toLower();
    for (int r = 0; r < tableRecent_->rowCount(); ++r) {
        bool match = false;
        if (filter.isEmpty()) {
            match = true;
        } else {
            for (int c = 0; c < tableRecent_->columnCount(); ++c) {
                auto *item = tableRecent_->item(r, c);
                if (item && item->text().toLower().contains(filter)) {
                    match = true;
                    break;
                }
            }
        }
        tableRecent_->setRowHidden(r, !match);
    }
}

void MainWindow::handleRecentTableClicked(int row, int /*col*/) {
    if (row < 0 || row >= tableRecent_->rowCount()) return;
    QString txId = tableRecent_->item(row, 1)->text();
    QString amt = tableRecent_->item(row, 3)->text();
    QString dec = tableRecent_->item(row, 8)->text();
    QString score = tableRecent_->item(row, 7)->text();

    QMessageBox::information(this, QString::fromUtf8("Chi Tiết Giao Dịch"),
                             QString::fromUtf8("Mã giao dịch: %1\nSố tiền: %2\nĐiểm rủi ro: %3 / 100\nQuyết định xử lý: %4")
                             .arg(txId, amt, score, dec));
}

// =========================================================================
// DASHBOARD: ROW 4 - BOTTOM DEMO SCENARIO ACTION CARDS
// =========================================================================
QWidget* MainWindow::createBottomScenarioBar() {
    auto *bar = new QWidget();
    bar->setObjectName("dashboardCard");
    auto *lay = new QVBoxLayout(bar);
    lay->setContentsMargins(12, 10, 12, 10);
    lay->setSpacing(6);

    auto *lblTitle = new QLabel(QString::fromUtf8("▶ Demo kịch bản"));
    lblTitle->setStyleSheet("color: #FFFFFF; font-size: 12px; font-weight: bold;");
    lay->addWidget(lblTitle);

    auto *cardsLay = new QHBoxLayout();
    cardsLay->setSpacing(10);

    // Kịch bản 1: Giao dịch bình thường
    auto *card1 = new QWidget();
    card1->setObjectName("scenarioActionCard");
    auto *c1Lay = new QHBoxLayout(card1);
    c1Lay->setContentsMargins(10, 8, 10, 8);
    c1Lay->setSpacing(10);

    auto *icon1 = new QLabel(QString::fromUtf8("✔️"));
    icon1->setFixedSize(30, 30);
    icon1->setAlignment(Qt::AlignCenter);
    icon1->setStyleSheet("background-color: #064E3B; color: #34D399; border-radius: 15px; font-size: 14px;");

    auto *info1 = new QWidget();
    auto *i1Lay = new QVBoxLayout(info1);
    i1Lay->setContentsMargins(0, 0, 0, 0);
    i1Lay->setSpacing(2);
    auto *t1 = new QLabel(QString::fromUtf8("Giao dịch bình thường"));
    t1->setStyleSheet("color: #FFFFFF; font-size: 11px; font-weight: bold;");
    auto *d1 = new QLabel(QString::fromUtf8("Thanh toán tại cửa hàng, thiết bị quen thuộc, số tiền hợp lý"));
    d1->setStyleSheet("color: #64748B; font-size: 9px;");
    i1Lay->addWidget(t1);
    i1Lay->addWidget(d1);

    auto *btnApprove = new QPushButton(QString::fromUtf8("→ APPROVE"));
    btnApprove->setObjectName("btnScenarioApprove");
    btnApprove->setCursor(Qt::PointingHandCursor);
    btnApprove->setFixedHeight(30);
    btnApprove->setStyleSheet("background-color: #064E3B; color: #34D399; font-weight: bold; border-radius: 6px; padding: 4px 12px; border: 1px solid #059669; font-size: 11px;");
    connect(btnApprove, &QPushButton::clicked, this, &MainWindow::handleTriggerNormalScenario);

    c1Lay->addWidget(icon1);
    c1Lay->addWidget(info1, 1);
    c1Lay->addWidget(btnApprove);
    cardsLay->addWidget(card1);

    // Kịch bản 2: Quẹt thẻ liên tục
    auto *card2 = new QWidget();
    card2->setObjectName("scenarioActionCard");
    auto *c2Lay = new QHBoxLayout(card2);
    c2Lay->setContentsMargins(10, 8, 10, 8);
    c2Lay->setSpacing(10);

    auto *icon2 = new QLabel(QString::fromUtf8("🛡️"));
    icon2->setFixedSize(30, 30);
    icon2->setAlignment(Qt::AlignCenter);
    icon2->setStyleSheet("background-color: #7F1D1D; color: #F87171; border-radius: 15px; font-size: 14px;");

    auto *info2 = new QWidget();
    auto *i2Lay = new QVBoxLayout(info2);
    i2Lay->setContentsMargins(0, 0, 0, 0);
    i2Lay->setSpacing(2);
    auto *t2 = new QLabel(QString::fromUtf8("Quẹt thẻ liên tục"));
    t2->setStyleSheet("color: #FFFFFF; font-size: 11px; font-weight: bold;");
    auto *d2 = new QLabel(QString::fromUtf8("Nhiều giao dịch trong 60 giây cùng thẻ"));
    d2->setStyleSheet("color: #64748B; font-size: 9px;");
    i2Lay->addWidget(t2);
    i2Lay->addWidget(d2);

    auto *btnBlock = new QPushButton(QString::fromUtf8("→ BLOCK"));
    btnBlock->setObjectName("btnScenarioBlock");
    btnBlock->setCursor(Qt::PointingHandCursor);
    btnBlock->setFixedHeight(30);
    btnBlock->setStyleSheet("background-color: #7F1D1D; color: #F87171; font-weight: bold; border-radius: 6px; padding: 4px 12px; border: 1px solid #DC2626; font-size: 11px;");
    connect(btnBlock, &QPushButton::clicked, this, &MainWindow::handleTriggerVelocityScenario);

    c2Lay->addWidget(icon2);
    c2Lay->addWidget(info2, 1);
    c2Lay->addWidget(btnBlock);
    cardsLay->addWidget(card2);

    // Kịch bản 3: Số tiền lớn + IP lạ
    auto *card3 = new QWidget();
    card3->setObjectName("scenarioActionCard");
    auto *c3Lay = new QHBoxLayout(card3);
    c3Lay->setContentsMargins(10, 8, 10, 8);
    c3Lay->setSpacing(10);

    auto *icon3 = new QLabel(QString::fromUtf8("⚠️"));
    icon3->setFixedSize(30, 30);
    icon3->setAlignment(Qt::AlignCenter);
    icon3->setStyleSheet("background-color: #78350F; color: #FCD34D; border-radius: 15px; font-size: 14px;");

    auto *info3 = new QWidget();
    auto *i3Lay = new QVBoxLayout(info3);
    i3Lay->setContentsMargins(0, 0, 0, 0);
    i3Lay->setSpacing(2);
    auto *t3 = new QLabel(QString::fromUtf8("Số tiền lớn + IP lạ"));
    t3->setStyleSheet("color: #FFFFFF; font-size: 11px; font-weight: bold;");
    auto *d3 = new QLabel(QString::fromUtf8("Giao dịch giá trị cao từ vị trí không quen thuộc"));
    d3->setStyleSheet("color: #64748B; font-size: 9px;");
    i3Lay->addWidget(t3);
    i3Lay->addWidget(d3);

    auto *btnReview = new QPushButton(QString::fromUtf8("→ REVIEW"));
    btnReview->setObjectName("btnScenarioReview");
    btnReview->setCursor(Qt::PointingHandCursor);
    btnReview->setFixedHeight(30);
    btnReview->setStyleSheet("background-color: #78350F; color: #FCD34D; font-weight: bold; border-radius: 6px; padding: 4px 12px; border: 1px solid #D97706; font-size: 11px;");
    connect(btnReview, &QPushButton::clicked, this, &MainWindow::handleTriggerHighAmountScenario);

    c3Lay->addWidget(icon3);
    c3Lay->addWidget(info3, 1);
    c3Lay->addWidget(btnReview);
    cardsLay->addWidget(card3);

    lay->addLayout(cardsLay);
    return bar;
}

// =========================================================================
// DASHBOARD: RIGHT PANEL (ALERTS, RULES, ML PREDICTOR)
// =========================================================================
QWidget* MainWindow::createRightPanel() {
    auto *panel = new QWidget();
    panel->setObjectName("rightPanel");
    panel->setFixedWidth(280);

    auto *lay = new QVBoxLayout(panel);
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(10);

    // 1. CẢNH BÁO ƯU TIÊN (Priority Alerts)
    auto *cardAlerts = new QWidget();
    cardAlerts->setObjectName("dashboardCard");
    auto *aLay = new QVBoxLayout(cardAlerts);
    aLay->setContentsMargins(12, 10, 12, 10);
    aLay->setSpacing(8);

    auto *headAlert = new QHBoxLayout();
    auto *lblAlertTitle = new QLabel(QString::fromUtf8("🚨 Cảnh báo ưu tiên"));
    lblAlertTitle->setStyleSheet("color: #FFFFFF; font-size: 12px; font-weight: bold;");
    auto *btnViewAll = new QPushButton(QString::fromUtf8("Xem tất cả"));
    btnViewAll->setCursor(Qt::PointingHandCursor);
    btnViewAll->setStyleSheet("background: transparent; border: none; color: #38BDF8; font-size: 10px; font-weight: 600;");
    connect(btnViewAll, &QPushButton::clicked, [this]() { handleNavButtonClicked(2); });

    headAlert->addWidget(lblAlertTitle);
    headAlert->addStretch();
    headAlert->addWidget(btnViewAll);
    aLay->addLayout(headAlert);

    layAlertsContainer_ = new QVBoxLayout();
    layAlertsContainer_->setSpacing(6);

    layAlertsContainer_->addWidget(createPriorityAlertItem(
        "#FA-20260624-0048", "VelocityRule", "14:20", 92,
        QString::fromUtf8("Phát hiện giao dịch lặp lại trong 60 giây"),
        QString::fromUtf8("Số tiền: $2,500 | Thẻ: ****1234"), true));

    layAlertsContainer_->addWidget(createPriorityAlertItem(
        "#FA-20260624-0047", "HighAmountRule", "13:45", 78,
        QString::fromUtf8("Giao dịch có số tiền bất thường"),
        QString::fromUtf8("Số tiền: $8,200 | Vị trí: VN"), false));

    layAlertsContainer_->addWidget(createPriorityAlertItem(
        "#FA-20260624-0046", "BlacklistRule", "12:30", 96,
        QString::fromUtf8("Thẻ/IP có trong danh sách đen"),
        QString::fromUtf8("Thẻ: ****9876 | IP: 185.199.23.17"), true));

    aLay->addLayout(layAlertsContainer_);
    lay->addWidget(cardAlerts);

    // 2. BỘ LUẬT ĐANG BẬT (Active Rules)
    auto *cardRules = new QWidget();
    cardRules->setObjectName("dashboardCard");
    auto *rLay = new QVBoxLayout(cardRules);
    rLay->setContentsMargins(12, 10, 12, 10);
    rLay->setSpacing(6);

    auto *headRule = new QHBoxLayout();
    auto *lblRuleTitle = new QLabel(QString::fromUtf8("🛡️ Bộ luật đang bật"));
    lblRuleTitle->setStyleSheet("color: #FFFFFF; font-size: 12px; font-weight: bold;");
    auto *lblRuleCount = new QLabel(QString::fromUtf8("4 luật"));
    lblRuleCount->setStyleSheet("color: #64748B; font-size: 10px; font-weight: 600;");
    headRule->addWidget(lblRuleTitle);
    headRule->addStretch();
    headRule->addWidget(lblRuleCount);
    rLay->addLayout(headRule);

    rLay->addWidget(createRuleToggleItem("VelocityRule", QString::fromUtf8("Phát hiện giao dịch lặp nhanh"), true));
    rLay->addWidget(createRuleToggleItem("HighAmountRule", QString::fromUtf8("Phát hiện số tiền bất thường"), true));
    rLay->addWidget(createRuleToggleItem("BlacklistRule", QString::fromUtf8("Kiểm tra thẻ/IP trong blacklist"), true));
    rLay->addWidget(createRuleToggleItem("DeviceRiskRule", QString::fromUtf8("Phát hiện thiết bị rủi ro"), true));

    lay->addWidget(cardRules);

    // 3. NATIVE ML PREDICTOR
    auto *cardMl = new QWidget();
    cardMl->setObjectName("dashboardCard");
    auto *mlLay = new QVBoxLayout(cardMl);
    mlLay->setContentsMargins(12, 10, 12, 10);
    mlLay->setSpacing(8);

    auto *lblMlTitle = new QLabel(QString::fromUtf8("⚛️ Native ML Predictor"));
    lblMlTitle->setStyleSheet("color: #FFFFFF; font-size: 12px; font-weight: bold;");
    mlLay->addWidget(lblMlTitle);

    auto *badgeLoaded = new QLabel(QString::fromUtf8("✔️  model_weights.json loaded"));
    badgeLoaded->setStyleSheet("background-color: #064E3B; color: #34D399; font-size: 10px; font-weight: bold; padding: 4px 8px; border-radius: 6px;");
    badgeLoaded->setAlignment(Qt::AlignCenter);
    mlLay->addWidget(badgeLoaded);

    auto *lblFeatTitle = new QLabel(QString::fromUtf8("Đặc trưng sử dụng"));
    lblFeatTitle->setStyleSheet("color: #94A3B8; font-size: 10px; font-weight: 600;");
    mlLay->addWidget(lblFeatTitle);

    // Feature tags pills
    auto *featBox = new QWidget();
    auto *fLay = new QHBoxLayout(featBox);
    fLay->setContentsMargins(0, 0, 0, 0);
    fLay->setSpacing(4);

    auto createTag = [](const QString& name) -> QLabel* {
        auto *lbl = new QLabel(name);
        lbl->setStyleSheet("background-color: #131F3F; color: #38BDF8; font-size: 9px; padding: 2px 6px; border-radius: 4px; border: 1px solid #1E2D5A;");
        return lbl;
    };
    fLay->addWidget(createTag("amount"));
    fLay->addWidget(createTag("velocity"));
    fLay->addWidget(createTag("distance"));
    fLay->addWidget(createTag("device_risk"));
    fLay->addStretch();
    mlLay->addWidget(featBox);

    auto *lblModelDesc = new QLabel(QString::fromUtf8("🐍 Logistic Regression (Python)\n   weights: model_weights.json"));
    lblModelDesc->setStyleSheet("color: #64748B; font-size: 9px; font-family: Consolas, monospace;");
    mlLay->addWidget(lblModelDesc);

    lay->addWidget(cardMl);
    lay->addStretch();

    return panel;
}

QWidget* MainWindow::createPriorityAlertItem(const QString& alertId, const QString& ruleBadge,
                                             const QString& timeStr, int score, const QString& title,
                                             const QString& details, bool isCritical) {
    auto *item = new QWidget();
    item->setObjectName("alertCardItem");
    auto *lay = new QHBoxLayout(item);
    lay->setContentsMargins(8, 6, 8, 6);
    lay->setSpacing(8);

    auto *iconLbl = new QLabel(isCritical ? QString::fromUtf8("🛡️") : QString::fromUtf8("⚠️"));
    iconLbl->setFixedSize(26, 26);
    iconLbl->setAlignment(Qt::AlignCenter);
    iconLbl->setStyleSheet(isCritical ?
        "background-color: #7F1D1D; color: #F87171; border-radius: 13px; font-size: 12px;" :
        "background-color: #78350F; color: #FCD34D; border-radius: 13px; font-size: 12px;");

    auto *infoBox = new QWidget();
    auto *iLay = new QVBoxLayout(infoBox);
    iLay->setContentsMargins(0, 0, 0, 0);
    iLay->setSpacing(1);

    auto *topRow = new QHBoxLayout();
    auto *lblId = new QLabel(alertId);
    lblId->setStyleSheet("color: #FFFFFF; font-size: 9px; font-weight: bold;");

    auto *lblRule = new QLabel(ruleBadge);
    lblRule->setStyleSheet(isCritical ?
        "background-color: #7F1D1D; color: #F87171; font-size: 8px; padding: 1px 4px; border-radius: 3px;" :
        "background-color: #78350F; color: #FCD34D; font-size: 8px; padding: 1px 4px; border-radius: 3px;");

    auto *lblTime = new QLabel(timeStr);
    lblTime->setStyleSheet("color: #64748B; font-size: 8px;");

    auto *lblScore = new QLabel(QString("Score: %1").arg(score));
    lblScore->setStyleSheet(isCritical ? "color: #EF4444; font-size: 8px; font-weight: bold;" : "color: #F59E0B; font-size: 8px; font-weight: bold;");

    topRow->addWidget(lblId);
    topRow->addWidget(lblRule);
    topRow->addStretch();
    topRow->addWidget(lblTime);
    topRow->addWidget(lblScore);
    iLay->addLayout(topRow);

    auto *lblTitle = new QLabel(title);
    lblTitle->setStyleSheet("color: #E2E8F0; font-size: 10px; font-weight: 600;");
    iLay->addWidget(lblTitle);

    auto *lblDet = new QLabel(details);
    lblDet->setStyleSheet("color: #64748B; font-size: 8px;");
    iLay->addWidget(lblDet);

    lay->addWidget(iconLbl);
    lay->addWidget(infoBox, 1);

    return item;
}

QWidget* MainWindow::createRuleToggleItem(const QString& ruleName, const QString& ruleDesc, bool initialChecked) {
    auto *row = new QWidget();
    auto *lay = new QHBoxLayout(row);
    lay->setContentsMargins(0, 2, 0, 2);
    lay->setSpacing(6);

    auto *infoBox = new QWidget();
    auto *iLay = new QVBoxLayout(infoBox);
    iLay->setContentsMargins(0, 0, 0, 0);
    iLay->setSpacing(0);

    auto *lblName = new QLabel(ruleName);
    lblName->setStyleSheet("color: #FFFFFF; font-size: 10px; font-weight: bold;");

    auto *lblDesc = new QLabel(ruleDesc);
    lblDesc->setStyleSheet("color: #64748B; font-size: 8px;");

    iLay->addWidget(lblName);
    iLay->addWidget(lblDesc);
    lay->addWidget(infoBox, 1);

    auto *toggleBtn = new QCheckBox();
    toggleBtn->setChecked(initialChecked);
    toggleBtn->setCursor(Qt::PointingHandCursor);
    toggleBtn->setStyleSheet(R"(
        QCheckBox::indicator {
            width: 32px;
            height: 18px;
            border-radius: 9px;
            background-color: #334155;
        }
        QCheckBox::indicator:checked {
            background-color: #0284C7;
        }
    )");

    connect(toggleBtn, &QCheckBox::toggled, [this, ruleName](bool checked) {
        handleRuleToggled(ruleName, checked);
    });

    lay->addWidget(toggleBtn);
    return row;
}

void MainWindow::handleRuleToggled(const QString& ruleName, bool checked) {
    auto& rules = service_.getRiskEngine().getRules();
    for (int i = 0; i < rules.size(); ++i) {
        if (rules[i] && QString::fromStdString(rules[i]->getRuleName()) == ruleName) {
            rules[i]->setEnabled(checked);
            break;
        }
    }
}

// =========================================================================
// INTERACTIVE SCENARIOS LOGIC
// =========================================================================

void MainWindow::handleTriggerNormalScenario() {
    if (!alice_ || !accAlice_) return;
    Transaction tx(("TXN-" + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss")).toStdString(),
                    TransactionType::BUY, alice_->getCustomerId(), accAlice_->getAccountId(),
                    45.0, "USD", Location("VN", "Da Nang", 16.0544, 108.2022),
                    Device("DEV_ALICE", "113.161.45.10", false, false), "4111112233441234", mVinMart_);

    auto res = service_.processTransaction(tx, *accAlice_, *alice_);
    accountRepo_.update(*accAlice_);
    accountRepo_.syncToDisk("data/accounts.txt");
    transactionRepo_.add(tx);
    transactionRepo_.syncToDisk("data/transactions.csv");

    // Chèn dòng mới lên đầu bảng
    addTransactionToRecentTable(QString::fromStdString(tx.getTransactionId()),
                               QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"),
                               tx.getAmount(), "1/60s", "Low",
                               res.assessment.getMlScore() / 100.0,
                               static_cast<int>(res.assessment.getCombinedScore()), "APPROVE");

    countTotalSample_++;
    countApproved_++;
    updateDashboardKpis();

    QMessageBox::information(this, QString::fromUtf8("Kịch Bản 1: Bình Thường"),
                             QString::fromUtf8("Giao dịch $45.00 của Alice đã được hệ thống PHÊ DUYỆT (APPROVE) an toàn!"));
}

void MainWindow::handleTriggerVelocityScenario() {
    if (!bob_ || !accBob_) return;

    for (int i = 0; i < 3; ++i) {
        Transaction txSpam("TXN-SPAM-" + std::to_string(rand() % 9000 + 1000),
                           TransactionType::BUY, bob_->getCustomerId(), accBob_->getAccountId(),
                           100.0, "USD", Location("VN", "Hanoi", 21.0285, 105.8542),
                           Device("DEV_BOB", "14.232.208.50", false, false), "4000123456789010", mShopee_);
        service_.processTransaction(txSpam, *accBob_, *bob_);
    }

    Transaction txAttack(("TXN-" + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss")).toStdString(),
                         TransactionType::BUY, bob_->getCustomerId(), accBob_->getAccountId(),
                         1200.0, "USD", Location("VN", "Hanoi", 21.0285, 105.8542),
                         Device("DEV_BOB_SPAM", "14.232.208.50", true, false), "4000123456789010", mShopee_);

    auto res = service_.processTransaction(txAttack, *accBob_, *bob_);
    accountRepo_.update(*accBob_);
    accountRepo_.syncToDisk("data/accounts.txt");
    transactionRepo_.add(txAttack);
    transactionRepo_.syncToDisk("data/transactions.csv");

    addTransactionToRecentTable(QString::fromStdString(txAttack.getTransactionId()),
                               QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"),
                               txAttack.getAmount(), "5/60s", "High",
                               res.assessment.getMlScore() / 100.0,
                               static_cast<int>(res.assessment.getCombinedScore()), "BLOCK");

    // Thêm cảnh báo mới vào Right Panel
    layAlertsContainer_->insertWidget(0, createPriorityAlertItem(
        QString::fromStdString(txAttack.getTransactionId()), "VelocityRule",
        QDateTime::currentDateTime().toString("HH:mm"),
        static_cast<int>(res.assessment.getCombinedScore()),
        QString::fromUtf8("Quẹt thẻ tần suất cao vượt ngưỡng 60s"),
        QString::fromUtf8("Số tiền: $1,200 | Thẻ: ****9010"), true));

    countTotalSample_++;
    countBlocked_++;
    updateDashboardKpis();

    QMessageBox::warning(this, QString::fromUtf8("Kịch Bản 2: Tần Suất Cao (BLOCK)"),
                         QString::fromUtf8("Phát hiện hành vi quẹt thẻ dồn dập! Hệ thống đã CHẶN (BLOCK) để bảo vệ số dư tài khoản."));
}

void MainWindow::handleTriggerHighAmountScenario() {
    if (!charlie_ || !accCharlie_) return;

    Transaction txHigh(("TXN-" + QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss")).toStdString(),
                       TransactionType::BUY, charlie_->getCustomerId(), accCharlie_->getAccountId(),
                       8200.0, "USD", Location("FR", "Paris", 48.8566, 2.3522),
                       Device("DEV_REMOTE", "198.51.100.44", false, false), "5105105105105100", mCasino_);

    auto res = service_.processTransaction(txHigh, *accCharlie_, *charlie_);
    accountRepo_.update(*accCharlie_);
    accountRepo_.syncToDisk("data/accounts.txt");
    transactionRepo_.add(txHigh);
    transactionRepo_.syncToDisk("data/transactions.csv");

    addTransactionToRecentTable(QString::fromStdString(txHigh.getTransactionId()),
                               QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"),
                               txHigh.getAmount(), "2/60s", "Medium",
                               res.assessment.getMlScore() / 100.0,
                               static_cast<int>(res.assessment.getCombinedScore()), "REVIEW");

    layAlertsContainer_->insertWidget(0, createPriorityAlertItem(
        QString::fromStdString(txHigh.getTransactionId()), "HighAmountRule",
        QDateTime::currentDateTime().toString("HH:mm"),
        static_cast<int>(res.assessment.getCombinedScore()),
        QString::fromUtf8("Giao dịch giá trị lớn tại vị trí quốc tế lạ"),
        QString::fromUtf8("Số tiền: $8,200 | Paris, FR"), false));

    countTotalSample_++;
    countReview_++;
    updateDashboardKpis();

    QMessageBox::warning(this, QString::fromUtf8("Kịch Bản 3: Nghi Vấn (REVIEW)"),
                         QString::fromUtf8("Giao dịch $8,200 từ Paris có dấu hiệu bất thường. Đã đưa vào HÀNG ĐỢI RỦI RO (REVIEW)!"));
}

void MainWindow::updateDashboardKpis() {
    if (lblKpiSample_) lblKpiSample_->setText(QString::number(countTotalSample_));
    if (lblKpiApproved_) lblKpiApproved_->setText(QString::number(countApproved_));
    if (lblKpiReview_) lblKpiReview_->setText(QString::number(countReview_));
    if (lblKpiBlocked_) lblKpiBlocked_->setText(QString::number(countBlocked_));

    if (donutChart_) {
        donutChart_->setValues(countApproved_, countReview_, countBlocked_);
    }

    double pctApprove = (static_cast<double>(countApproved_) / countTotalSample_) * 100.0;
    double pctReview = (static_cast<double>(countReview_) / countTotalSample_) * 100.0;
    double pctBlock = (static_cast<double>(countBlocked_) / countTotalSample_) * 100.0;

    if (lblLegendApproved_) {
        lblLegendApproved_->setText(QString("<span style='color:#10B981; font-size: 12px;'>●</span> <span style='color:#FFFFFF; font-weight:bold; font-size: 11px;'>APPROVE</span> &nbsp; <span style='color:#94A3B8; font-size: 10px;'>%1%</span><br>&nbsp;&nbsp;&nbsp;<b style='color:#E2E8F0; font-size: 11px;'>%2</b>")
                                   .arg(pctApprove, 0, 'f', 1).arg(countApproved_));
    }
    if (lblLegendReview_) {
        lblLegendReview_->setText(QString("<span style='color:#F59E0B; font-size: 12px;'>●</span> <span style='color:#FFFFFF; font-weight:bold; font-size: 11px;'>REVIEW</span> &nbsp; <span style='color:#94A3B8; font-size: 10px;'>%1%</span><br>&nbsp;&nbsp;&nbsp;<b style='color:#E2E8F0; font-size: 11px;'>%2</b>")
                                 .arg(pctReview, 0, 'f', 1).arg(countReview_));
    }
    if (lblLegendBlocked_) {
        lblLegendBlocked_->setText(QString("<span style='color:#EF4444; font-size: 12px;'>●</span> <span style='color:#FFFFFF; font-weight:bold; font-size: 11px;'>BLOCK</span> &nbsp; <span style='color:#94A3B8; font-size: 10px;'>%1%</span><br>&nbsp;&nbsp;&nbsp;<b style='color:#E2E8F0; font-size: 11px;'>%2</b>")
                                  .arg(pctBlock, 0, 'f', 1).arg(countBlocked_));
    }

    if (lblLastUpdated_) {
        lblLastUpdated_->setText(QString::fromUtf8("Cập nhật lần cuối: %1 🔄").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss")));
    }
}

// =========================================================================
// CÁC TRANG QUẢN TRỊ CHI TIẾT (SUB-MANAGEMENT PAGES)
// =========================================================================

QWidget* MainWindow::createTransactionManagementPage() {
    auto *page = new QWidget();
    auto *lay = new QVBoxLayout(page);
    lay->setContentsMargins(16, 16, 16, 16);
    lay->setSpacing(12);

    auto *head = new QLabel(QString::fromUtf8("⇄ Quản Trị Giao Dịch & Khách Hàng (Customer & Account Management)"));
    head->setStyleSheet("color: #FFFFFF; font-size: 16px; font-weight: bold;");
    lay->addWidget(head);

    // Bảng khách hàng
    auto *grpCust = new QGroupBox(QString::fromUtf8("DANH SÁCH KHÁCH HÀNG & QUAN HỆ AGGREGATION"));
    auto *cL = new QVBoxLayout(grpCust);
    tableCustomers_ = new QTableWidget();
    tableCustomers_->setColumnCount(8);
    tableCustomers_->setHorizontalHeaderLabels({
        QString::fromUtf8("Mã KH"), QString::fromUtf8("Họ và Tên"), QString::fromUtf8("Email"),
        QString::fromUtf8("Số ĐT"), QString::fromUtf8("Địa Chỉ"), QString::fromUtf8("Hạng Rủi Ro"),
        QString::fromUtf8("Xác Thực KYC"), QString::fromUtf8("Số TK Liên Kết")
    });
    tableCustomers_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableCustomers_->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableCustomers_->setSelectionMode(QAbstractItemView::SingleSelection);
    tableCustomers_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableCustomers_->setAlternatingRowColors(true);
    connect(tableCustomers_, &QTableWidget::itemSelectionChanged, this, &MainWindow::handleCustomerTableSelectionChanged);
    cL->addWidget(tableCustomers_);
    lay->addWidget(grpCust, 3);

    // Bảng tài khoản liên kết
    auto *grpAcc = new QGroupBox(QString::fromUtf8("TÀI KHOẢN NGÂN HÀNG LIÊN KẾT CỦA KHÁCH HÀNG (CUSTOMER ♢-> ACCOUNT*)"));
    auto *aL = new QVBoxLayout(grpAcc);
    lblSelectedCustomerDetail_ = new QLabel(QString::fromUtf8("Đang chọn khách hàng: Chưa chọn"));
    lblSelectedCustomerDetail_->setStyleSheet("color: #38BDF8; font-weight: bold; font-size: 12px;");
    aL->addWidget(lblSelectedCustomerDetail_);

    tableCustomerLinkedAccounts_ = new QTableWidget();
    tableCustomerLinkedAccounts_->setColumnCount(4);
    tableCustomerLinkedAccounts_->setHorizontalHeaderLabels({
        QString::fromUtf8("Mã Tài Khoản"), QString::fromUtf8("Số Dư"), QString::fromUtf8("Tiền Tệ"), QString::fromUtf8("Trạng Thái")
    });
    tableCustomerLinkedAccounts_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableCustomerLinkedAccounts_->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableCustomerLinkedAccounts_->setSelectionMode(QAbstractItemView::SingleSelection);
    tableCustomerLinkedAccounts_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableCustomerLinkedAccounts_->setAlternatingRowColors(true);
    connect(tableCustomerLinkedAccounts_, &QTableWidget::itemSelectionChanged, this, &MainWindow::handleAccountSelectionChanged);
    aL->addWidget(tableCustomerLinkedAccounts_);

    // Nút thao tác số dư & tài khoản
    auto *accBtnRow = new QHBoxLayout();
    btnDeposit_ = new QPushButton(QString::fromUtf8("💰 Nạp Tiền"));
    btnDeposit_->setStyleSheet("background-color: #0284C7; color: white; padding: 6px 14px; border-radius: 6px; font-weight: bold;");
    connect(btnDeposit_, &QPushButton::clicked, this, &MainWindow::handleDeposit);

    btnWithdraw_ = new QPushButton(QString::fromUtf8("💸 Rút Tiền"));
    btnWithdraw_->setStyleSheet("background-color: #F59E0B; color: white; padding: 6px 14px; border-radius: 6px; font-weight: bold;");
    connect(btnWithdraw_, &QPushButton::clicked, this, &MainWindow::handleWithdraw);

    btnFreeze_ = new QPushButton(QString::fromUtf8("❄️ Đóng Băng TK"));
    btnFreeze_->setStyleSheet("background-color: #EF4444; color: white; padding: 6px 14px; border-radius: 6px; font-weight: bold;");
    connect(btnFreeze_, &QPushButton::clicked, this, &MainWindow::handleFreezeAccount);

    btnUnfreeze_ = new QPushButton(QString::fromUtf8("🔓 Mở Băng TK"));
    btnUnfreeze_->setStyleSheet("background-color: #10B981; color: white; padding: 6px 14px; border-radius: 6px; font-weight: bold;");
    connect(btnUnfreeze_, &QPushButton::clicked, this, &MainWindow::handleUnfreezeAccount);

    btnUpdateRiskTier_ = new QPushButton(QString::fromUtf8("⭐ Đổi Hạng Rủi Ro"));
    btnUpdateRiskTier_->setStyleSheet("background-color: #6366F1; color: white; padding: 6px 14px; border-radius: 6px; font-weight: bold;");
    connect(btnUpdateRiskTier_, &QPushButton::clicked, this, &MainWindow::handleUpdateCustomerTier);

    accBtnRow->addWidget(btnDeposit_);
    accBtnRow->addWidget(btnWithdraw_);
    accBtnRow->addWidget(btnFreeze_);
    accBtnRow->addWidget(btnUnfreeze_);
    accBtnRow->addWidget(btnUpdateRiskTier_);
    accBtnRow->addStretch();
    aL->addLayout(accBtnRow);

    lay->addWidget(grpAcc, 2);

    // Nút hành động
    auto *btnRow = new QHBoxLayout();
    btnAddNewCustomer_ = new QPushButton(QString::fromUtf8("➕ Thêm Khách Hàng Mới"));
    btnAddNewCustomer_->setStyleSheet("background-color: #0284C7; color: white; padding: 6px 14px; border-radius: 6px; font-weight: bold;");
    connect(btnAddNewCustomer_, &QPushButton::clicked, this, &MainWindow::handleAddNewCustomer);

    btnCreateSubAccount_ = new QPushButton(QString::fromUtf8("🏦 Mở Tài Khoản Mới Cho Khách Hàng"));
    btnCreateSubAccount_->setStyleSheet("background-color: #10B981; color: white; padding: 6px 14px; border-radius: 6px; font-weight: bold;");
    connect(btnCreateSubAccount_, &QPushButton::clicked, this, &MainWindow::handleCreateAccountForCustomer);

    btnToggleKyc_ = new QPushButton(QString::fromUtf8("🔒 Bật / Tắt Xác Thực KYC"));
    btnToggleKyc_->setStyleSheet("background-color: #334155; color: white; padding: 6px 14px; border-radius: 6px;");
    connect(btnToggleKyc_, &QPushButton::clicked, this, &MainWindow::handleToggleCustomerKyc);

    btnRow->addWidget(btnAddNewCustomer_);
    btnRow->addWidget(btnCreateSubAccount_);
    btnRow->addWidget(btnToggleKyc_);
    btnRow->addStretch();
    lay->addLayout(btnRow);

    refreshCustomerTable();
    return page;
}

QWidget* MainWindow::createRiskEnginePage() {
    auto *page = new QWidget();
    auto *lay = new QVBoxLayout(page);
    lay->setContentsMargins(16, 16, 16, 16);
    lay->setSpacing(12);

    auto *head = new QLabel(QString::fromUtf8("🛡️ Động Cơ Rủi Ro & Hàng Đợi Ưu Tiên (PriorityQueue Max-Heap)"));
    head->setStyleSheet("color: #FFFFFF; font-size: 16px; font-weight: bold;");
    lay->addWidget(head);

    tableReviewQueue_ = new QTableWidget();
    tableReviewQueue_->setColumnCount(6);
    tableReviewQueue_->setHorizontalHeaderLabels({
        QString::fromUtf8("Mã GD"), QString::fromUtf8("Mã KH"), QString::fromUtf8("Số Tiền"),
        QString::fromUtf8("Điểm Rủi Ro (Heap Priority)"), QString::fromUtf8("Lý Do Chính"), QString::fromUtf8("Hành Động Đề Xuất")
    });
    tableReviewQueue_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableReviewQueue_->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableReviewQueue_->setSelectionMode(QAbstractItemView::SingleSelection);
    tableReviewQueue_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableReviewQueue_->setAlternatingRowColors(true);
    lay->addWidget(tableReviewQueue_, 1);

    auto *btnRow = new QHBoxLayout();
    btnReviewApprove_ = new QPushButton(QString::fromUtf8("✅ Ghi Đè Phê Duyệt (Approve)"));
    btnReviewApprove_->setStyleSheet("background-color: #10B981; color: white; padding: 6px 14px; border-radius: 6px; font-weight: bold;");
    connect(btnReviewApprove_, &QPushButton::clicked, this, &MainWindow::handleReviewQueueApprove);

    btnReviewReject_ = new QPushButton(QString::fromUtf8("⛔ Khẳng Định Gian Lận & Đóng Băng TK (Block)"));
    btnReviewReject_->setStyleSheet("background-color: #EF4444; color: white; padding: 6px 14px; border-radius: 6px; font-weight: bold;");
    connect(btnReviewReject_, &QPushButton::clicked, this, &MainWindow::handleReviewQueueConfirmFraud);

    btnReviewRefresh_ = new QPushButton(QString::fromUtf8("🔄 Làm Mới Hàng Đợi"));
    btnReviewRefresh_->setStyleSheet("background-color: #334155; color: white; padding: 6px 14px; border-radius: 6px;");
    connect(btnReviewRefresh_, &QPushButton::clicked, this, &MainWindow::refreshReviewQueueTable);

    btnRow->addWidget(btnReviewApprove_);
    btnRow->addWidget(btnReviewReject_);
    btnRow->addWidget(btnReviewRefresh_);
    btnRow->addStretch();
    lay->addLayout(btnRow);

    refreshReviewQueueTable();
    return page;
}

QWidget* MainWindow::createBlacklistPage() {
    auto *page = new QWidget();
    auto *lay = new QVBoxLayout(page);
    lay->setContentsMargins(16, 16, 16, 16);
    lay->setSpacing(12);

    auto *head = new QLabel(QString::fromUtf8("📋 Quản Lý Danh Sách Đen Bằng Bảng Băm (HashMap O(1) Lookup)"));
    head->setStyleSheet("color: #FFFFFF; font-size: 16px; font-weight: bold;");
    lay->addWidget(head);

    // Form thêm vào blacklist
    auto *addBox = new QWidget();
    auto *aLay = new QHBoxLayout(addBox);
    aLay->setContentsMargins(0, 0, 0, 0);
    aLay->setSpacing(10);

    cmbBlacklistType_ = new QComboBox();
    cmbBlacklistType_->addItem(QString::fromUtf8("Địa chỉ IP độc hại"), 0);
    cmbBlacklistType_->addItem(QString::fromUtf8("Số thẻ bị rò rỉ (PAN)"), 1);
    cmbBlacklistType_->setFixedWidth(180);

    txtBlacklistValue_ = new QLineEdit();
    txtBlacklistValue_->setPlaceholderText(QString::fromUtf8("Nhập IP hoặc Số thẻ cần chặn tức thời..."));

    btnAddBlacklist_ = new QPushButton(QString::fromUtf8("➕ Thêm Vào Blacklist (HashMap)"));
    btnAddBlacklist_->setStyleSheet("background-color: #EF4444; color: white; padding: 6px 14px; border-radius: 6px; font-weight: bold;");
    connect(btnAddBlacklist_, &QPushButton::clicked, this, &MainWindow::handleAddBlacklist);

    aLay->addWidget(new QLabel(QString::fromUtf8("Loại:")));
    aLay->addWidget(cmbBlacklistType_);
    aLay->addWidget(new QLabel(QString::fromUtf8("Giá trị:")));
    aLay->addWidget(txtBlacklistValue_, 1);
    aLay->addWidget(btnAddBlacklist_);
    lay->addWidget(addBox);

    auto *tblLay = new QHBoxLayout();
    tableBlacklistIps_ = new QTableWidget();
    tableBlacklistIps_->setColumnCount(2);
    tableBlacklistIps_->setHorizontalHeaderLabels({QString::fromUtf8("Địa Chỉ IP"), QString::fromUtf8("Trạng Thái Bảng Băm")});
    tableBlacklistIps_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableBlacklistIps_->setAlternatingRowColors(true);
    tblLay->addWidget(tableBlacklistIps_);

    tableBlacklistCards_ = new QTableWidget();
    tableBlacklistCards_->setColumnCount(2);
    tableBlacklistCards_->setHorizontalHeaderLabels({QString::fromUtf8("Số Thẻ (PAN 16 số)"), QString::fromUtf8("Trạng Thái Bảng Băm")});
    tableBlacklistCards_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableBlacklistCards_->setAlternatingRowColors(true);
    tblLay->addWidget(tableBlacklistCards_);

    lay->addLayout(tblLay);
    refreshBlacklistTables();
    return page;
}

QWidget* MainWindow::createMlDatasetPage() {
    auto *page = new QWidget();
    auto *lay = new QVBoxLayout(page);
    lay->setContentsMargins(16, 16, 16, 16);
    lay->setSpacing(12);

    auto *head = new QLabel(QString::fromUtf8("🤖 Machine Learning & Cơ Sở Dữ Liệu (Native ML Predictor)"));
    head->setStyleSheet("color: #FFFFFF; font-size: 16px; font-weight: bold;");
    lay->addWidget(head);

    auto *txtInfo = new QTextEdit();
    txtInfo->setReadOnly(true);
    txtInfo->setStyleSheet("background-color: #111C38; color: #F8FAFC; border: 1px solid #1E2D5A; border-radius: 8px; font-size: 13px; font-family: Consolas, monospace; padding: 12px;");
    txtInfo->setText(
        "=== MÔ HÌNH NATIVE ML PREDICTOR TRONG C++ CORE ===\n\n"
        "- Kiến trúc mô hình: Calibrated Logistic Regression\n"
        "- Tập tin trọng số: data/model_weights.json (Tự động đọc vào bộ nhớ khi khởi động)\n"
        "- Vector đặc trưng 4 chiều (Feature Vector):\n"
        "  [1] amount      : Số tiền giao dịch chuẩn hóa (Z-Score)\n"
        "  [2] velocity    : Tần suất quẹt thẻ trong 60 giây (Sliding Window Deque)\n"
        "  [3] distance    : Khoảng cách địa lý Haversine so với vị trí đăng ký (km)\n"
        "  [4] device_risk : Cờ cảnh báo thiết bị (Giả lập Emulator, Root/Jailbreak)\n\n"
        "- Hàm suy luận C++:\n"
        "  z = bias + sum(w_i * x_i)\n"
        "  probability = 1.0 / (1.0 + exp(-z))\n"
        "- Thời gian suy luận trung bình: < 0.5 microseconds (siêu tốc độ không phụ thuộc Python runtime)\n"
    );
    lay->addWidget(txtInfo, 1);

    return page;
}

// =========================================================================
// SLOTS PHỤ TRỢ (CUSTOMER & ACCOUNT MANAGEMENT)
// =========================================================================

void MainWindow::refreshCustomerTable() {
    if (!tableCustomers_) return;
    tableCustomers_->setRowCount(0);
    const auto& custs = customerRepo_.getAll();

    for (int i = 0; i < custs.size(); ++i) {
        int r = tableCustomers_->rowCount();
        tableCustomers_->insertRow(r);

        const auto& c = custs[i];
        tableCustomers_->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(c.getCustomerId())));
        tableCustomers_->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(c.getFullName())));
        tableCustomers_->setItem(r, 2, new QTableWidgetItem(QString::fromStdString(c.getEmail())));
        tableCustomers_->setItem(r, 3, new QTableWidgetItem(QString::fromStdString(c.getPhoneNumber())));
        tableCustomers_->setItem(r, 4, new QTableWidgetItem(QString::fromStdString(c.getHomeLocation().getCity())));

        auto *tierItem = new QTableWidgetItem(QString::fromStdString(std::string(toString(c.getRiskTier()))));
        tierItem->setTextAlignment(Qt::AlignCenter);
        tableCustomers_->setItem(r, 5, tierItem);

        auto *kycItem = new QTableWidgetItem(c.isKycVerified() ? QString::fromUtf8("ĐÃ XÁC MINH") : QString::fromUtf8("CHƯA KYC"));
        kycItem->setTextAlignment(Qt::AlignCenter);
        tableCustomers_->setItem(r, 6, kycItem);

        auto *accItem = new QTableWidgetItem(QString("%1 tài khoản").arg(accountRepo_.findByCustomerId(c.getCustomerId()).size()));
        accItem->setTextAlignment(Qt::AlignCenter);
        tableCustomers_->setItem(r, 7, accItem);
    }
}

void MainWindow::handleCustomerTableSelectionChanged() {
    int r = tableCustomers_->currentRow();
    if (r < 0) return;

    std::string cid = tableCustomers_->item(r, 0)->text().toStdString();
    Customer* cust = customerRepo_.findById(cid);
    if (!cust) return;

    lblSelectedCustomerDetail_->setText(QString::fromUtf8("Khách hàng đang chọn: %1 - %2")
                                        .arg(QString::fromStdString(cust->getCustomerId()), QString::fromStdString(cust->getFullName())));

    tableCustomerLinkedAccounts_->setRowCount(0);
    auto accs = accountRepo_.findByCustomerId(cust->getCustomerId());
    for (int i = 0; i < accs.size(); ++i) {
        Account* a = accs[i];
        if (!a) continue;
        int row = tableCustomerLinkedAccounts_->rowCount();
        tableCustomerLinkedAccounts_->insertRow(row);
        tableCustomerLinkedAccounts_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(a->getAccountId())));
        tableCustomerLinkedAccounts_->setItem(row, 1, new QTableWidgetItem(QString("$%1").arg(a->getBalance(), 0, 'f', 2)));
        tableCustomerLinkedAccounts_->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(a->getCurrency())));
        tableCustomerLinkedAccounts_->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(std::string(toString(a->getStatus())))));
    }
}

void MainWindow::handleUpdateCustomerTier() {
    int r = tableCustomers_->currentRow();
    if (r < 0) {
        QMessageBox::warning(this, QString::fromUtf8("Cảnh Báo"), QString::fromUtf8("Vui lòng chọn khách hàng trong bảng!"));
        return;
    }
    std::string cid = tableCustomers_->item(r, 0)->text().toStdString();
    Customer* cust = customerRepo_.findById(cid);
    if (!cust) return;

    QStringList tiers = {"NEW", "ESTABLISHED", "TRUSTED", "SUSPICIOUS", "BANNED"};
    bool ok = false;
    QString chosen = QInputDialog::getItem(this, QString::fromUtf8("Đổi Hạng Rủi Ro"),
                                          QString::fromUtf8("Chọn Hạng Rủi Ro (Risk Tier) mới cho khách hàng:"),
                                          tiers, static_cast<int>(cust->getRiskTier()), false, &ok);
    if (!ok) return;

    int idx = tiers.indexOf(chosen);
    if (idx >= 0) {
        cust->setRiskTier(static_cast<CustomerRiskTier>(idx));
        customerRepo_.update(*cust);
        customerRepo_.syncToDisk("data/customers.txt");
        refreshCustomerTable();
        tableCustomers_->selectRow(r);
        QMessageBox::information(this, QString::fromUtf8("Thành Công"),
                                 QString::fromUtf8("Đã cập nhật Risk Tier của %1 thành %2!")
                                 .arg(QString::fromStdString(cust->getFullName()), chosen));
    }
}

void MainWindow::handleToggleCustomerKyc() {
    int r = tableCustomers_->currentRow();
    if (r < 0) return;
    std::string cid = tableCustomers_->item(r, 0)->text().toStdString();
    Customer* cust = customerRepo_.findById(cid);
    if (!cust) return;
    cust->setKycVerified(!cust->isKycVerified());
    customerRepo_.update(*cust);
    customerRepo_.syncToDisk("data/customers.txt");
    refreshCustomerTable();
    tableCustomers_->selectRow(r);
}

void MainWindow::handleAddNewCustomer() {
    bool ok = false;
    QString name = QInputDialog::getText(this, QString::fromUtf8("Thêm Khách Hàng"),
                                        QString::fromUtf8("Họ và Tên khách hàng mới:"),
                                        QLineEdit::Normal, QString::fromUtf8("Pham Van D"), &ok);
    if (!ok || name.trimmed().isEmpty()) return;

    int newIdNum = customerRepo_.count() + 1;
    std::string newCustId = "CUST_" + std::to_string(100 + newIdNum);
    Location loc("Vietnam", "Da Nang", 16.0544, 108.2022);
    Customer newCust(newCustId, name.trimmed().toStdString(), "user" + std::to_string(newIdNum) + "@bank.vn",
                     "0905000" + std::to_string(newIdNum), loc, CustomerRiskTier::NEW, true);

    customerRepo_.add(newCust);
    customerRepo_.syncToDisk("data/customers.txt");
    refreshCustomerTable();
}

void MainWindow::handleCreateAccountForCustomer() {
    int r = tableCustomers_->currentRow();
    if (r < 0) {
        QMessageBox::warning(this, QString::fromUtf8("Cảnh báo"), QString::fromUtf8("Vui lòng chọn khách hàng trong bảng!"));
        return;
    }
    std::string cid = tableCustomers_->item(r, 0)->text().toStdString();
    Customer* cust = customerRepo_.findById(cid);
    if (!cust) return;

    auto currentAccs = accountRepo_.findByCustomerId(cid);
    std::string newAccId = "ACC_" + cid + "_" + std::to_string(currentAccs.size() + 1);
    Account newAcc(newAccId, cid, 2000.0, "USD");
    accountRepo_.add(newAcc);
    accountRepo_.syncToDisk("data/accounts.txt");

    cust->addLinkedAccount(newAccId);
    customerRepo_.update(*cust);
    customerRepo_.syncToDisk("data/customers.txt");

    refreshCustomerTable();
    tableCustomers_->selectRow(r);
    handleCustomerTableSelectionChanged();

    QMessageBox::information(this, QString::fromUtf8("Thành Công"),
                             QString::fromUtf8("Đã mở thành công tài khoản %1 cho khách hàng %2!")
                             .arg(QString::fromStdString(newAccId), QString::fromStdString(cust->getFullName())));
}

void MainWindow::handleAccountSelectionChanged() {
    int r = tableCustomerLinkedAccounts_->currentRow();
    if (r < 0) {
        if (lblSelectedAccountInfo_) lblSelectedAccountInfo_->setText(QString::fromUtf8("Chưa chọn tài khoản"));
        return;
    }
    std::string accId = tableCustomerLinkedAccounts_->item(r, 0)->text().toStdString();
    Account* acc = accountRepo_.findById(accId);
    if (!acc) return;
    if (lblSelectedAccountInfo_) {
        lblSelectedAccountInfo_->setText(QString::fromUtf8("Đang chọn: %1 | Số dư: $%2 | %3")
            .arg(QString::fromStdString(acc->getAccountId()))
            .arg(acc->getBalance(), 0, 'f', 2)
            .arg(QString::fromStdString(std::string(toString(acc->getStatus())))));
    }
}

void MainWindow::handleDeposit() {
    int r = tableCustomerLinkedAccounts_->currentRow();
    if (r < 0) {
        QMessageBox::warning(this, QString::fromUtf8("Cảnh Báo"), QString::fromUtf8("Vui lòng chọn tài khoản trong bảng tài khoản liên kết!"));
        return;
    }
    std::string accId = tableCustomerLinkedAccounts_->item(r, 0)->text().toStdString();
    Account* acc = accountRepo_.findById(accId);
    if (!acc) return;

    bool ok = false;
    double amount = QInputDialog::getDouble(this, QString::fromUtf8("Nạp Tiền Vào Tài Khoản"),
                                            QString::fromUtf8("Nhập số tiền cần nạp ($ USD):"),
                                            500.0, 1.0, 1000000.0, 2, &ok);
    if (!ok || amount <= 0.0) return;

    acc->deposit(amount);
    accountRepo_.update(*acc);
    accountRepo_.syncToDisk("data/accounts.txt");

    handleCustomerTableSelectionChanged();
    tableCustomerLinkedAccounts_->selectRow(r);
    QMessageBox::information(this, QString::fromUtf8("Thành Công"),
                             QString::fromUtf8("Đã nạp thành công $%1 vào tài khoản %2. Số dư mới: $%3")
                             .arg(amount, 0, 'f', 2)
                             .arg(QString::fromStdString(accId))
                             .arg(acc->getBalance(), 0, 'f', 2));
}

void MainWindow::handleWithdraw() {
    int r = tableCustomerLinkedAccounts_->currentRow();
    if (r < 0) {
        QMessageBox::warning(this, QString::fromUtf8("Cảnh Báo"), QString::fromUtf8("Vui lòng chọn tài khoản trong bảng tài khoản liên kết!"));
        return;
    }
    std::string accId = tableCustomerLinkedAccounts_->item(r, 0)->text().toStdString();
    Account* acc = accountRepo_.findById(accId);
    if (!acc) return;

    bool ok = false;
    double amount = QInputDialog::getDouble(this, QString::fromUtf8("Rút Tiền Từ Tài Khoản"),
                                            QString::fromUtf8("Nhập số tiền cần rút ($ USD):"),
                                            100.0, 1.0, acc->getBalance(), 2, &ok);
    if (!ok || amount <= 0.0) return;

    if (!acc->withdraw(amount)) {
        QMessageBox::critical(this, QString::fromUtf8("Lỗi Rút Tiền"),
                              QString::fromUtf8("Không thể rút tiền! Vui lòng kiểm tra trạng thái hoạt động hoặc số dư tài khoản."));
        return;
    }

    accountRepo_.update(*acc);
    accountRepo_.syncToDisk("data/accounts.txt");

    handleCustomerTableSelectionChanged();
    tableCustomerLinkedAccounts_->selectRow(r);
    QMessageBox::information(this, QString::fromUtf8("Thành Công"),
                             QString::fromUtf8("Đã rút thành công $%1 từ tài khoản %2. Số dư mới: $%3")
                             .arg(amount, 0, 'f', 2)
                             .arg(QString::fromStdString(accId))
                             .arg(acc->getBalance(), 0, 'f', 2));
}

void MainWindow::handleFreezeAccount() {
    int r = tableCustomerLinkedAccounts_->currentRow();
    if (r < 0) {
        QMessageBox::warning(this, QString::fromUtf8("Cảnh Báo"), QString::fromUtf8("Vui lòng chọn tài khoản trong bảng tài khoản liên kết!"));
        return;
    }
    std::string accId = tableCustomerLinkedAccounts_->item(r, 0)->text().toStdString();
    if (accountRepo_.freezeAccount(accId)) {
        accountRepo_.syncToDisk("data/accounts.txt");
        handleCustomerTableSelectionChanged();
        tableCustomerLinkedAccounts_->selectRow(r);
        QMessageBox::warning(this, QString::fromUtf8("Đóng Băng"),
                             QString::fromUtf8("Tài khoản %1 đã bị ĐÓNG BĂNG khẩn cấp!").arg(QString::fromStdString(accId)));
    }
}

void MainWindow::handleUnfreezeAccount() {
    int r = tableCustomerLinkedAccounts_->currentRow();
    if (r < 0) {
        QMessageBox::warning(this, QString::fromUtf8("Cảnh Báo"), QString::fromUtf8("Vui lòng chọn tài khoản trong bảng tài khoản liên kết!"));
        return;
    }
    std::string accId = tableCustomerLinkedAccounts_->item(r, 0)->text().toStdString();
    if (accountRepo_.unfreezeAccount(accId)) {
        accountRepo_.syncToDisk("data/accounts.txt");
        handleCustomerTableSelectionChanged();
        tableCustomerLinkedAccounts_->selectRow(r);
        QMessageBox::information(this, QString::fromUtf8("Mở Băng"),
                                 QString::fromUtf8("Tài khoản %1 đã được kích hoạt mở băng trở lại.").arg(QString::fromStdString(accId)));
    }
}

void MainWindow::refreshAccountTable() {
    handleCustomerTableSelectionChanged();
}

void MainWindow::refreshReviewQueueTable() {
    if (!tableReviewQueue_) return;
    tableReviewQueue_->setRowCount(0);
    // Mẫu
    int r = tableReviewQueue_->rowCount();
    tableReviewQueue_->insertRow(r);
    tableReviewQueue_->setItem(r, 0, new QTableWidgetItem("TXN-20260624-1003"));
    tableReviewQueue_->setItem(r, 1, new QTableWidgetItem("CUST_CHARLIE"));
    tableReviewQueue_->setItem(r, 2, new QTableWidgetItem("$8,200.00"));
    tableReviewQueue_->setItem(r, 3, new QTableWidgetItem("72.0 / 100"));
    tableReviewQueue_->setItem(r, 4, new QTableWidgetItem(QString::fromUtf8("Số tiền lớn bất thường + IP lạ")));
    tableReviewQueue_->setItem(r, 5, new QTableWidgetItem(QString::fromUtf8("Yêu cầu mã xác thực OTP")));
}

void MainWindow::handleReviewQueueApprove() {
    QMessageBox::information(this, QString::fromUtf8("Phê Duyệt"), QString::fromUtf8("Điều tra viên đã ghi đè phê duyệt giao dịch thành công."));
}

void MainWindow::handleReviewQueueConfirmFraud() {
    QMessageBox::warning(this, QString::fromUtf8("Khẳng Định Gian Lận"), QString::fromUtf8("Đã khẳng định gian lận! Tài khoản đã bị đóng băng."));
}

void MainWindow::refreshBlacklistTables() {
    if (!tableBlacklistIps_ || !tableBlacklistCards_) return;
    tableBlacklistIps_->setRowCount(0);
    tableBlacklistCards_->setRowCount(0);

    std::ifstream ifs("data/blacklist.txt");
    if (ifs.is_open()) {
        std::string line;
        while (std::getline(ifs, line)) {
            if (line.empty() || line[0] == '#') continue;
            if (line.rfind("IP:", 0) == 0) {
                std::string ip = line.substr(3);
                int r = tableBlacklistIps_->rowCount();
                tableBlacklistIps_->insertRow(r);
                tableBlacklistIps_->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(ip)));
                tableBlacklistIps_->setItem(r, 1, new QTableWidgetItem(QString::fromUtf8("CHẶN O(1)")));
            } else if (line.rfind("CARD:", 0) == 0) {
                std::string card = line.substr(5);
                int r = tableBlacklistCards_->rowCount();
                tableBlacklistCards_->insertRow(r);
                tableBlacklistCards_->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(card)));
                tableBlacklistCards_->setItem(r, 1, new QTableWidgetItem(QString::fromUtf8("CHẶN O(1)")));
            }
        }
    }
}

void MainWindow::handleAddBlacklist() {
    std::string val = txtBlacklistValue_->text().trimmed().toStdString();
    if (val.empty()) return;
    int type = cmbBlacklistType_->currentIndex();
    auto* blRule = service_.getRiskEngine().getBlacklistRule();
    if (type == 0) {
        if (blRule) {
            blRule->addBlacklistedIp(val);
            blRule->appendBlacklistToFile("IP", val, "data/blacklist.txt");
        }
    } else {
        if (blRule) {
            blRule->addBlacklistedCard(val);
            blRule->appendBlacklistToFile("CARD", val, "data/blacklist.txt");
        }
    }
    refreshBlacklistTables();
    txtBlacklistValue_->clear();
    QMessageBox::information(this, QString::fromUtf8("Thành Công"), QString::fromUtf8("Đã thêm vào Bảng băm Blacklist (HashMap) và lưu xuống file data/blacklist.txt!"));
}

void MainWindow::handleLogout() {
    auto res = QMessageBox::question(this, QString::fromUtf8("Đăng Xuất"),
                                    QString::fromUtf8("Bạn có chắc chắn muốn đăng xuất khỏi quyền Admin?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if (res == QMessageBox::Yes) {
        if (authService_) authService_->logout();
        logged_out_ = true;
        close();
    }
}

// =========================================================================
// THEME STYLING (QSS) - KHỚP TỪNG MÃ MÀU VỚI GIAO DIỆN MẪU
// =========================================================================
void MainWindow::applyThemeStyles() {
    setStyleSheet(R"(
        QWidget#centralWidget {
            background-color: #0B132B;
        }
        #topHeader {
            background-color: #0E1838;
            border-bottom: 1px solid #1C2B54;
        }
        #leftSidebar {
            background-color: #0E1838;
            border-right: 1px solid #1C2B54;
        }
        #searchContainer {
            background-color: #131F3F;
            border: 1px solid #1E2D5A;
            border-radius: 8px;
        }
        #engineStatusPill {
            background-color: #131F3F;
            border: 1px solid #1E2D5A;
            border-radius: 16px;
        }
        #brandCard {
            background-color: #111C38;
            border: 1px solid #1E2D5A;
            border-radius: 10px;
        }
        #kpiCard {
            background-color: #111C38;
            border: 1px solid #1C2B54;
            border-radius: 10px;
        }
        #dashboardCard {
            background-color: #111C38;
            border: 1px solid #1C2B54;
            border-radius: 10px;
        }
        #scenarioActionCard {
            background-color: #131F3F;
            border: 1px solid #1E2D5A;
            border-radius: 8px;
        }
        #alertCardItem {
            background-color: #131F3F;
            border: 1px solid #1E2D5A;
            border-radius: 6px;
        }
        QTableWidget {
            background-color: #111C38;
            alternate-background-color: #142145;
            gridline-color: #1C2B54;
            color: #F8FAFC;
            border: none;
            border-radius: 6px;
            font-size: 11px;
        }
        QHeaderView::section {
            background-color: #131F3F;
            color: #94A3B8;
            font-weight: bold;
            font-size: 10px;
            padding: 5px;
            border: none;
            border-bottom: 1px solid #1C2B54;
        }
        QTableWidget::item {
            padding: 4px;
        }
        QTableWidget::item:selected {
            background-color: #0284C7;
            color: #FFFFFF;
        }
        QComboBox#filterDropdown {
            background-color: #131F3F;
            color: #CBD5E1;
            border: 1px solid #1E2D5A;
            border-radius: 6px;
            padding: 2px 6px;
            font-size: 10px;
        }
        QComboBox#filterDropdown QAbstractItemView {
            background-color: #131F3F;
            color: #FFFFFF;
            selection-background-color: #0284C7;
        }
        QGroupBox {
            color: #FFFFFF;
            font-weight: bold;
            font-size: 11px;
            border: 1px solid #1C2B54;
            border-radius: 8px;
            margin-top: 8px;
            padding-top: 10px;
            background-color: #111C38;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 4px;
            color: #38BDF8;
        }
    )");
}
