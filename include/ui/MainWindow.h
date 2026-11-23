#pragma once

#include <QMainWindow>
#include <QStackedWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <vector>

#include "ui/CustomWidgets.h"
#include "bll/TransactionBLL.h"
#include "bll/Auth.h"
#include "dal/AccountDAL.h"
#include "dal/CustomerDAL.h"
#include "dal/TransactionDAL.h"
#include "core/models/Customer.h"
#include "core/models/Account.h"
#include "core/models/Merchant.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(AuthService* authService = nullptr, QWidget *parent = nullptr);
    ~MainWindow() override = default;

    bool isLoggedOut() const { return logged_out_; }

private slots:
    // Navigation Sidebar
    void handleNavButtonClicked(int pageIndex);
    void handleLogout();

    // Quick Scenario Triggers (Bottom Bar)
    void handleTriggerNormalScenario();
    void handleTriggerVelocityScenario();
    void handleTriggerHighAmountScenario();

    // Search and Table Filter
    void handleSearchFilter(const QString& text);
    void handleRecentTableClicked(int row, int col);

    // Rule Switches
    void handleRuleToggled(const QString& ruleName, bool checked);

    // Sub-management Slots
    void handleCustomerTableSelectionChanged();
    void handleUpdateCustomerTier();
    void handleToggleCustomerKyc();
    void handleAddNewCustomer();
    void handleCreateAccountForCustomer();
    void refreshCustomerTable();

    void handleAccountSelectionChanged();
    void handleDeposit();
    void handleWithdraw();
    void handleFreezeAccount();
    void handleUnfreezeAccount();
    void refreshAccountTable();

    void handleAddBlacklist();
    void refreshBlacklistTables();

    void handleReviewQueueApprove();
    void handleReviewQueueConfirmFraud();
    void refreshReviewQueueTable();

private:
    void setupUi();
    void applyThemeStyles();
    void initSampleData();

    // UI Section Builders
    QWidget* createTopHeader();
    QWidget* createLeftSidebar();
    QWidget* createDashboardPage();
    QWidget* createTransactionManagementPage();
    QWidget* createRiskEnginePage();
    QWidget* createBlacklistPage();
    QWidget* createMlDatasetPage();

    // Dashboard Sub-sections
    QWidget* createKpiRow();
    QWidget* createChartsRow();
    QWidget* createRecentTransactionsSection();
    QWidget* createBottomScenarioBar();
    QWidget* createRightPanel();

    // Helpers to create styled components
    QWidget* createKpiCard(const QString& iconText, const QString& title, const QString& value,
                          const QString& subText, const QString& iconBgColor, const QString& valColor,
                          const std::vector<double>& sparkData, SparklineWidget** outSpark);
    QWidget* createPriorityAlertItem(const QString& alertId, const QString& ruleBadge,
                                     const QString& timeStr, int score, const QString& title,
                                     const QString& details, bool isCritical);
    QWidget* createRuleToggleItem(const QString& ruleName, const QString& ruleDesc, bool initialChecked);

    void updateDashboardKpis();
    void addTransactionToRecentTable(const QString& id, const QString& time, double amount,
                                     const QString& velocity, const QString& deviceRisk,
                                     double mlProb, int riskScore, const QString& decision);

    // Services & Repositories
    AuthService* authService_{nullptr};
    bool logged_out_{false};
    TransactionService service_;
    CustomerRepository customerRepo_;
    AccountRepository accountRepo_;
    TransactionRepository transactionRepo_;

    Customer* alice_{nullptr};
    Account* accAlice_{nullptr};
    Customer* bob_{nullptr};
    Account* accBob_{nullptr};
    Customer* charlie_{nullptr};
    Account* accCharlie_{nullptr};

    Merchant mVinMart_;
    Merchant mShopee_;
    Merchant mCasino_;
    Merchant mCrypto_;
    Merchant mDarknet_;

    // Navigation state
    int currentPageIndex_{0};
    std::vector<QPushButton*> navButtons_;
    QStackedWidget* mainStack_{nullptr};

    // Header Widgets
    QLineEdit* txtSearch_{nullptr};
    QLabel* lblLastUpdated_{nullptr};
    QLabel* lblAdminProfile_{nullptr};

    // Dashboard KPI widgets
    QLabel* lblKpiSample_{nullptr};
    QLabel* lblKpiApproved_{nullptr};
    QLabel* lblKpiReview_{nullptr};
    QLabel* lblKpiBlocked_{nullptr};
    SparklineWidget* sparkSample_{nullptr};
    SparklineWidget* sparkApproved_{nullptr};
    SparklineWidget* sparkReview_{nullptr};
    SparklineWidget* sparkBlocked_{nullptr};

    int countTotalSample_{5000};
    int countApproved_{4612};
    int countReview_{276};
    int countBlocked_{112};

    // Charts
    TrendChartWidget* trendChart_{nullptr};
    DonutChartWidget* donutChart_{nullptr};
    QLabel* lblLegendApproved_{nullptr};
    QLabel* lblLegendReview_{nullptr};
    QLabel* lblLegendBlocked_{nullptr};

    // Recent Transactions Table
    QTableWidget* tableRecent_{nullptr};

    // Right Panel: Priority Alerts container
    QVBoxLayout* layAlertsContainer_{nullptr};

    // Sub-management Page Widgets
    // Customer
    QTableWidget* tableCustomers_{nullptr};
    QLabel* lblSelectedCustomerDetail_{nullptr};
    QTableWidget* tableCustomerLinkedAccounts_{nullptr};
    QComboBox* cmbChangeRiskTier_{nullptr};
    QPushButton* btnUpdateRiskTier_{nullptr};
    QPushButton* btnToggleKyc_{nullptr};
    QPushButton* btnAddNewCustomer_{nullptr};
    QPushButton* btnCreateSubAccount_{nullptr};

    // Account
    QTableWidget* tableAccounts_{nullptr};
    QLabel* lblSelectedAccountInfo_{nullptr};
    QLineEdit* txtAccountActionAmount_{nullptr};
    QPushButton* btnDeposit_{nullptr};
    QPushButton* btnWithdraw_{nullptr};
    QPushButton* btnFreeze_{nullptr};
    QPushButton* btnUnfreeze_{nullptr};
    QTextEdit* txtAccountLog_{nullptr};

    // Blacklist
    QComboBox* cmbBlacklistType_{nullptr};
    QLineEdit* txtBlacklistValue_{nullptr};
    QPushButton* btnAddBlacklist_{nullptr};
    QTableWidget* tableBlacklistIps_{nullptr};
    QTableWidget* tableBlacklistCards_{nullptr};

    // Review Queue
    QTableWidget* tableReviewQueue_{nullptr};
    QLabel* lblQueueCount_{nullptr};
    QPushButton* btnReviewApprove_{nullptr};
    QPushButton* btnReviewReject_{nullptr};
    QPushButton* btnReviewRefresh_{nullptr};
    QTextEdit* txtReviewDetail_{nullptr};

    struct ReviewCaseItem {
        Transaction tx;
        RiskAssessment assess;
        Account* accPtr{nullptr};
    };
    std::vector<ReviewCaseItem> pending_reviews_;
};
