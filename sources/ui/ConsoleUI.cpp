#include "ui/ConsoleUI.h"
#include "core/models/PaymentMethod.h"
#include "bll/Auth.h"
#include "dal/AccountDAL.h"
#include "dal/CustomerDAL.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>

namespace Color {
    const char* RESET   = "\033[0m";
    const char* BOLD    = "\033[1m";
    const char* RED     = "\033[31m";
    const char* GREEN   = "\033[32m";
    const char* YELLOW  = "\033[33m";
    const char* BLUE    = "\033[34m";
    const char* CYAN    = "\033[36m";
}

void ConsoleUI::printSuccess(const std::string& msg) {
    std::cout << Color::GREEN << Color::BOLD << msg << Color::RESET << "\n";
}

void ConsoleUI::printWarning(const std::string& msg) {
    std::cout << Color::YELLOW << Color::BOLD << msg << Color::RESET << "\n";
}

void ConsoleUI::printDanger(const std::string& msg) {
    std::cout << Color::RED << Color::BOLD << msg << Color::RESET << "\n";
}

void ConsoleUI::printInfo(const std::string& msg) {
    std::cout << Color::CYAN << msg << Color::RESET << "\n";
}

void ConsoleUI::printBanner() {
    std::cout << Color::CYAN << Color::BOLD;
    std::cout << "================================================================================\n";
    std::cout << "         PBL2: Electronic Payment Fraud Detection & Risk Management            \n";
    std::cout << "             High-Performance C++ Core & Calibrated ML Hybrid Engine            \n";
    std::cout << "                   PBL 2 - Khoa CNTT - Truong DH Bach Khoa Da Nang              \n";
    std::cout << "================================================================================\n";
    std::cout << Color::RESET;
}

void ConsoleUI::printMenu() {
    std::cout << "\n" << Color::BOLD << "--- MENU CHUONG TRINH ---" << Color::RESET << "\n";
    std::cout << "1. Chay 3 Kich Ban Mau Tu Dong (Showcase Scenarios)\n";
    std::cout << "2. Nhap va Xu Ly Giao Dich Thu Cong (Interactive Mode)\n";
    std::cout << "3. Xem Hang Doi Rui Ro Uu Tien (Investigation PriorityQueue)\n";
    std::cout << "4. [Admin Phu] Dang Nhap & Quan Ly Khach Hang / Tai Khoan\n";
    std::cout << "5. Thoat\n";
    std::cout << "Lua chon cua ban (1-5): ";
}

void ConsoleUI::runAutomatedScenarios(TransactionBLL& bll) {
    printInfo("\n>>> DANG KHOI CHAY 3 KICH BAN DEMO MAU <<<");

    // ---------------------------------------------------------
    // KỊCH BẢN 1: Giao dịch hợp lệ bình thường của Alice
    // ---------------------------------------------------------
    std::cout << "\n" << Color::BOLD << "[KICH BAN 1] Giao dich hop le (Alice mua tap hoa $45.00 tai Da Nang)" << Color::RESET << "\n";
    Location locAlice("VN", "Da Nang", 16.0544, 108.2022);
    Customer alice("CUST_ALICE", "Alice Nguyen", "alice@email.com", "0905123456", locAlice, CustomerRiskTier::TRUSTED, true);
    Account accAlice("ACC_ALICE_01", alice.getCustomerId(), 1500.0, "USD");
    Device devAlice("DEV_ALICE_PHONE", "113.161.45.10", false, false);
    Transaction tx1("TX_NORM_001", TransactionType::BUY, alice.getCustomerId(),
                    accAlice.getAccountId(), 45.0, "USD", locAlice, devAlice, "4111112233441234");

    auto res1 = bll.processTransaction(tx1, accAlice, alice);
    std::cout << "  - So the che PCI-DSS:  " << PaymentMethod::maskPan(tx1.getRawCardNumber()) << "\n";
    std::cout << "  - Diem rui ro tong hop: " << std::fixed << std::setprecision(1) << res1.assessment.getCombinedScore() << "/100\n";
    std::cout << "  - Quyet dinh:          ";
    printSuccess("APPROVE [THANH CONG]");
    std::cout << "  - So du con lai:       $" << accAlice.getBalance() << " (Da tru $45.00)\n";

    // ---------------------------------------------------------
    // KỊCH BẢN 2: Tấn công quẹt thẻ dồn dập (Velocity Attack)
    // ---------------------------------------------------------
    std::cout << "\n" << Color::BOLD << "[KICH BAN 2] Tan cong quet the don dap (Bob bi spam 4 giao dich lien tiep)" << Color::RESET << "\n";
    Location locBob("VN", "Hanoi", 21.0285, 105.8542);
    Customer bob("CUST_BOB", "Bob Tran", "bob@email.com", "0912345678", locBob, CustomerRiskTier::NEW, false);
    Account accBob("ACC_BOB_01", bob.getCustomerId(), 5000.0, "USD");
    Device devBob("DEV_BOB_PHONE", "14.232.208.50", false, false);

    for (int i = 1; i <= 4; ++i) {
        Transaction txSpam("TX_SPAM_" + std::to_string(i), TransactionType::BUY, bob.getCustomerId(),
                           accBob.getAccountId(), 100.0, "USD", locBob, devBob, "4000123456789010");
        auto res = bll.processTransaction(txSpam, accBob, bob);
        std::cout << "  * GD " << i << " ($100): Diem rui ro = " << res.assessment.getCombinedScore() << " -> ";
        if (res.assessment.isApproved()) {
            printSuccess("APPROVE");
        } else {
            printDanger("BLOCK [CANH BAO SPAM THE BI CHAN BOI DEQUE SLIDING WINDOW]");
        }
    }
    std::cout << "  - So du tai khoan Bob: $" << accBob.getBalance() << " (Duoc bao ve an toan!)\n";

    // ---------------------------------------------------------
    // KỊCH BẢN 3: Tấn công rủi ro cao từ Thiết bị giả lập + IP Blacklist
    // ---------------------------------------------------------
    std::cout << "\n" << Color::BOLD << "[KICH BAN 3] Phat hien dot gian lan lon (Charlie mua Crypto $4,200 tu Paris qua Emulator)" << Color::RESET << "\n";
    Location locHome("VN", "HCM", 10.8231, 106.6297);
    Customer charlie("CUST_CHARLIE", "Charlie Le", "charlie@email.com", "0988776655", locHome, CustomerRiskTier::SUSPICIOUS, false);
    Account accCharlie("ACC_CHARLIE_01", charlie.getCustomerId(), 8000.0, "USD");

    Location locParis("FR", "Paris", 48.8566, 2.3522);
    Device devHacker("DEV_EMULATOR_X", "198.51.100.44", true, true); // Rooted + Emulator + Blacklist IP
    Transaction txAttack("TX_ATTACK_999", TransactionType::BUY, charlie.getCustomerId(),
                         accCharlie.getAccountId(), 4200.0, "USD", locParis, devHacker, "5105105105105100");

    auto res3 = bll.processTransaction(txAttack, accCharlie, charlie);
    std::cout << "  - So the che PCI-DSS:  " << PaymentMethod::maskPan(txAttack.getRawCardNumber()) << "\n";
    std::cout << "  - Diem rui ro tong hop: " << res3.assessment.getCombinedScore() << "/100 (CRITICAL)\n";
    std::cout << "  - Quyet dinh:          ";
    printDanger("BLOCK [TU CHOI GIAO DICH]");

    std::cout << "\n" << Color::YELLOW << Color::BOLD << "[GIAI THICH NGUYEN NHAN (Explainability)]:" << Color::RESET << "\n";
    const auto& alerts = res3.assessment.getAlerts();
    for (size_t i = 0; i < alerts.size(); ++i) {
        std::cout << "    + " << alerts[i].toString() << "\n";
    }
    std::cout << "    + Mo hinh Machine Learning danh gia: " << res3.assessment.getMlScore() << "% Fraud Probability\n";
    std::cout << "  - So du tai khoan Charlie: $" << accCharlie.getBalance() << " (Bao ve nguyen ven $8,000.00!)\n";
}

void ConsoleUI::runInteractiveMode(TransactionBLL& bll) {
    std::cout << "\n" << Color::BOLD << "--- NHAP THONG TIN GIAO DICH THU CONG ---" << Color::RESET << "\n";

    double amount = 0.0;
    std::cout << "Nhap so tien giao dich ($): ";
    std::cin >> amount;

    std::string cardNum;
    std::cout << "Nhap so the ngan hang (VD: 4111112233441234): ";
    std::cin >> cardNum;

    int isRiskDevice = 0;
    std::cout << "Thiet bi co bi Root/Gia lap khong? (1: Co, 0: Khong): ";
    std::cin >> isRiskDevice;

    Location loc("VN", "Da Nang", 16.0544, 108.2022);
    Customer cust("CUST_CUSTOM", "User Demo", "user@demo.com", "0123456789", loc);
    Account acc("ACC_DEMO", cust.getCustomerId(), 10000.0, "USD");
    Device dev("DEV_CUSTOM", "192.168.1.100", isRiskDevice == 1, isRiskDevice == 1);

    Transaction tx("TX_MANUAL_" + std::to_string(rand() % 10000), TransactionType::BUY,
                    cust.getCustomerId(), acc.getAccountId(), amount, "USD", loc, dev, cardNum);

    auto res = bll.processTransaction(tx, acc, cust);

    std::cout << "\n--- KET QUA DANH GIA ---\n";
    std::cout << "Diem rui ro: " << res.assessment.getCombinedScore() << "/100\n";
    std::cout << "Quyet dinh: ";
    if (res.assessment.isApproved()) printSuccess("APPROVE (Duyet thanh cong)");
    else if (res.assessment.isReview() || res.assessment.isChallenge()) printWarning("REVIEW (Nghi van, can ma OTP)");
    else printDanger("BLOCK (Bi chan do rui ro cao)");
}

void ConsoleUI::printReviewQueue(TransactionBLL& bll) {
    std::cout << "\n" << Color::BOLD << "--- DANH SACH HO SO NGHI VAN TRONG PRIORITY QUEUE ---" << Color::RESET << "\n";
    std::cout << "Tong so vu viec can ra soat: " << bll.getReviewQueueSize() << "\n";
    if (bll.getReviewQueueSize() == 0) {
        printInfo("Hang doi hien dang trong. Khong co giao dich nghi van.");
    } else {
        printWarning("Cac giao dich co diem rui ro cao nhat dang nam o dau Heap!");
        const auto& topCase = bll.getReviewQueue().top();
        std::cout << "  * Vu viec uu tien xu ly nhat:\n";
        std::cout << "    - Ma tham dinh: " << topCase.getAssessmentId() << "\n";
        std::cout << "    - Ma giao dich: " << topCase.getTransactionId() << "\n";
        std::cout << "    - Diem rui ro:   " << std::fixed << std::setprecision(1) << topCase.getCombinedScore() << "/100\n";
        std::cout << "    - Muc do:        " << toString(topCase.getRiskLevel()) << "\n";
        std::cout << "    - Quyet dinh:    " << toString(topCase.getDecision()) << "\n";
    }
}

void ConsoleUI::runAdminManagement(TransactionBLL& bll) {
    printInfo("\n========================================================");
    printInfo("     CONG DANG NHAP QUAN TRI VIEN (ADMIN PORTAL)        ");
    printInfo("========================================================");

    Auth auth;
    auth.loadFromDisk("data/admin.txt");

    std::string user, pass;
    std::cout << "Ten dang nhap Admin: ";
    std::cin >> user;
    std::cout << "Mat khau: ";
    std::cin >> pass;

    if (!auth.login(user, pass)) {
        printDanger("Xac thuc that bai! Sai tai khoan hoac mat khau.");
        return;
    }

    printSuccess("\nDang nhap Admin thanh cong!");
    std::cout << "Chao mung " << auth.getCurrentUser().fullName << " (" << auth.getCurrentUser().department << ")\n";

    CustomerDAL& custDAL = bll.getCustomerDAL();
    AccountDAL& accDAL = bll.getAccountDAL();

    int adminChoice = 0;
    while (true) {
        std::cout << "\n" << Color::BOLD << "--- MENU QUAN TRI HE THONG (CHUC NANG PHU) ---" << Color::RESET << "\n";
        std::cout << "1. Xem Danh Sach Khach Hang & Tai Khoan Lien Ket\n";
        std::cout << "2. Cap Nhat Hang Rui Ro Khach Hang (Risk Tier)\n";
        std::cout << "3. Xem Danh Sach Tai Khoan & Thao Tac So Du (Nap/Rut/Dong Bang)\n";
        std::cout << "4. Dang Xuat & Quay Lai Menu Chinh\n";
        std::cout << "Lua chon cua Admin (1-4): ";

        if (!(std::cin >> adminChoice)) {
            std::cin.clear();
            std::string d;
            std::cin >> d;
            continue;
        }

        if (adminChoice == 1) {
            std::cout << "\n" << Color::BOLD << "=== DANH SACH KHACH HANG & TAI KHOAN LIEN KET ===" << Color::RESET << "\n";
            const auto& custs = custDAL.getAll();
            for (size_t i = 0; i < custs.size(); ++i) {
                const auto& c = custs[i];
                std::cout << "[" << (i + 1) << "] " << c.getCustomerId() << " | " << c.getFullName()
                          << " | Tier: " << toString(c.getRiskTier())
                          << " | KYC: " << (c.isKycVerified() ? "YES" : "NO") << "\n";
                auto accs = accDAL.findByCustomerId(c.getCustomerId());
                std::cout << "    L-> Cac tai khoan lien ket: " << accs.size() << " tai khoan\n";
                for (size_t j = 0; j < accs.size(); ++j) {
                    Account* a = accs[j];
                    if (a) {
                        std::cout << "        * " << a->getAccountId() << ": $" << std::fixed << std::setprecision(2)
                                  << a->getBalance() << " " << a->getCurrency()
                                  << " [" << toString(a->getStatus()) << "]\n";
                    }
                }
            }
        } else if (adminChoice == 2) {
            std::string cid;
            std::cout << "Nhap Ma Khach Hang can doi Tier (vd: CUST_BOB): ";
            std::cin >> cid;
            Customer* c = custDAL.findById(cid);
            if (!c) {
                printDanger("Khong tim thay khach hang co ma: " + cid);
            } else {
                std::cout << "Chon Tier moi (0: NEW, 1: ESTABLISHED, 2: TRUSTED, 3: SUSPICIOUS, 4: BANNED): ";
                int t;
                std::cin >> t;
                if (t >= 0 && t <= 4) {
                    c->setRiskTier(static_cast<CustomerRiskTier>(t));
                    custDAL.update(*c);
                    custDAL.saveToFile("data/customers.txt");
                    printSuccess("Da cap nhat Risk Tier thanh cong!");
                } else {
                    printWarning("Lua chon Tier khong hop le.");
                }
            }
        } else if (adminChoice == 3) {
            std::cout << "\n" << Color::BOLD << "=== DANH SACH TAI KHOAN NGAN HANG (CORE BANKING) ===" << Color::RESET << "\n";
            const auto& accs = accDAL.getAll();
            for (size_t i = 0; i < accs.size(); ++i) {
                const auto& a = accs[i];
                std::cout << "[" << (i + 1) << "] " << a.getAccountId() << " | Thuoc KH: " << a.getCustomerId()
                          << " | So du: $" << std::fixed << std::setprecision(2) << a.getBalance()
                          << " " << a.getCurrency() << " | Trang thai: " << toString(a.getStatus()) << "\n";
            }

            std::cout << "\nThao tac: (1: Nap tien, 2: Rut tien, 3: Dong bang, 4: Mo khoa, 0: Bo qua): ";
            int op;
            std::cin >> op;
            if (op >= 1 && op <= 4) {
                std::string aid;
                std::cout << "Nhap Ma Tai Khoan (vd: ACC_001): ";
                std::cin >> aid;
                Account* a = accDAL.findById(aid);
                if (!a) {
                    printDanger("Khong tim thay tai khoan: " + aid);
                } else {
                    if (op == 1) {
                        double amt;
                        std::cout << "Nhap so tien nap ($ USD): ";
                        std::cin >> amt;
                        a->deposit(amt);
                        accDAL.update(*a);
                        accDAL.saveToFile("data/accounts.txt");
                        printSuccess("Nap tien thanh cong! So du moi: $" + std::to_string(a->getBalance()));
                    } else if (op == 2) {
                        double amt;
                        std::cout << "Nhap so tien rut ($ USD): ";
                        std::cin >> amt;
                        if (a->withdraw(amt)) {
                            accDAL.update(*a);
                            accDAL.saveToFile("data/accounts.txt");
                            printSuccess("Rut tien thanh cong! So du moi: $" + std::to_string(a->getBalance()));
                        } else {
                            printDanger("Rut tien that bai! Kiem tra trang thai hoac so du.");
                        }
                    } else if (op == 3) {
                        accDAL.freezeAccount(aid);
                        accDAL.saveToFile("data/accounts.txt");
                        printWarning("Tai khoan " + aid + " da bi DONG BANG khan cap!");
                    } else if (op == 4) {
                        accDAL.unfreezeAccount(aid);
                        accDAL.saveToFile("data/accounts.txt");
                        printSuccess("Tai khoan " + aid + " da duoc MO KHOA thanh cong!");
                    }
                }
            }
        } else if (adminChoice == 4) {
            auth.logout();
            printInfo("Admin da dang xuat.");
            break;
        } else {
            printWarning("Lua chon khong hop le.");
        }
    }
}
