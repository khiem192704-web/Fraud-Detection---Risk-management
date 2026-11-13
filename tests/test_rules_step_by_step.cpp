#include <iostream>
#include <string>
#include "engine/rules/IFraudRule.h"
#include "engine/rules/DeviceRiskRule.h"
#include "core/domain/transaction/Transaction.h"
#include "core/domain/account/Customer.h"
#include "core/domain/alert/FraudAlert.h"

// Màu sắc ANSI cho console
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_BOLD    "\033[1m"

void printHeader(const std::string& title) {
    std::cout << "\n" << COLOR_CYAN << COLOR_BOLD 
              << "========================================================\n"
              << "  " << title << "\n"
              << "========================================================\n" 
              << COLOR_RESET;
}

void printPass(const std::string& test_name) {
    std::cout << "  [" << COLOR_GREEN << "PASS" << COLOR_RESET << "] " << test_name << "\n";
}

void printFail(const std::string& test_name, const std::string& reason) {
    std::cout << "  [" << COLOR_RED << "FAIL" << COLOR_RESET << "] " << test_name 
              << " - Ly do: " << reason << "\n";
}

int main() {
    printHeader("KIEM THU BƯỚC 1 & 2.1: IFRAUD_RULE & DEVICE_RISK_RULE");

    int total_tests = 0;
    int passed_tests = 0;

    Customer dummy_customer("CUST_001", "Nguyen Van A", "a@gmail.com", "0901234567", Location());

    // -------------------------------------------------------------
    // TEST 1: Kiểm tra tính Đa hình (Polymorphism) qua IFraudRule*
    // -------------------------------------------------------------
    total_tests++;
    std::cout << COLOR_YELLOW << "\n[Test 1] Kiem tra con tro lop cha IFraudRule* (Strategy Pattern):" << COLOR_RESET << "\n";
    {
        IFraudRule* rule = new DeviceRiskRule(30.0);

        bool ok = true;
        if (rule->getId() != "RULE_004_DEVICE_RISK") {
            printFail("getId()", "Ky vong RULE_004_DEVICE_RISK nhung nhan duoc: " + rule->getId());
            ok = false;
        }
        if (rule->getCategory() != FraudRuleCategory::SUSPICIOUS_DEVICE) {
            printFail("getCategory()", "Category khong phai SUSPICIOUS_DEVICE");
            ok = false;
        }
        if (rule->getRiskContribution() != 30.0) {
            printFail("getRiskContribution()", "Ky vong 30.0");
            ok = false;
        }

        delete rule; // Kiem tra virtual destructor

        if (ok) {
            printPass("Lop con DeviceRiskRule ke thua va da hinh thanh cong tu IFraudRule");
            passed_tests++;
        }
    }

    // -------------------------------------------------------------
    // TEST 2: Giao dịch an toàn (Thiết bị thật, không root)
    // -------------------------------------------------------------
    total_tests++;
    std::cout << COLOR_YELLOW << "\n[Test 2] Kiem tra giao dich thuc hien tu Thiet bi An toan:" << COLOR_RESET << "\n";
    {
        Device safe_device("DEV_REAL_01", "192.168.1.10", "FINGERPRINT_OK", Location(), false, false);
        
        Transaction tx;
        tx.SetDevice(safe_device);

        DeviceRiskRule rule;
        FraudAlert alert;
        bool is_fraud = rule.evaluate(tx, dummy_customer, alert);

        if (!is_fraud) {
            printPass("Giao dich an toan khong kich hoat canh bao (evaluate tra ve false)");
            passed_tests++;
        } else {
            printFail("Thiet bi an toan", "evaluate() lai bao la gian lan!");
        }
    }

    // -------------------------------------------------------------
    // TEST 3: Giao dịch từ Máy ảo giả lập (Emulator / Virtual)
    // -------------------------------------------------------------
    total_tests++;
    std::cout << COLOR_YELLOW << "\n[Test 3] Kiem tra giao dich thuc hien tu May ao / Emulator:" << COLOR_RESET << "\n";
    {
        Device virtual_device("DEV_EMU_01", "10.0.2.15", "NOX_PLAYER_FP", Location(), true, false);
        
        Transaction tx;
        tx.SetDevice(virtual_device);

        DeviceRiskRule rule(35.0);
        FraudAlert alert;
        bool is_fraud = rule.evaluate(tx, dummy_customer, alert);

        if (is_fraud && alert.getCategory() == FraudRuleCategory::SUSPICIOUS_DEVICE && alert.getRiskContribution() == 35.0) {
            printPass("Phat hien chinh xac may ao gia lap (+35.0 diem)");
            std::cout << "         -> Ly do vi pham: \"" << alert.getReason() << "\"\n";
            passed_tests++;
        } else {
            printFail("May ao", "Khong bat duoc co virtual hoac thong tin alert sai lech!");
        }
    }

    // -------------------------------------------------------------
    // TEST 4: Giao dịch từ Thiết bị Hacker (Rooted / Compromised)
    // -------------------------------------------------------------
    total_tests++;
    std::cout << COLOR_YELLOW << "\n[Test 4] Kiem tra giao dich thuc hien tu Thiet bi Hacker (Root/Jailbreak):" << COLOR_RESET << "\n";
    {
        Device hacker_device("DEV_ROOT_01", "172.16.0.5", "MAGISK_ROOT_FP", Location(), false, true);
        
        Transaction tx;
        tx.SetDevice(hacker_device);

        DeviceRiskRule rule(30.0);
        FraudAlert alert;
        bool is_fraud = rule.evaluate(tx, dummy_customer, alert);

        if (is_fraud && alert.isCritical()) {
            printPass("Phat hien chinh xac thiet bi root/hacker o muc do nghiem trong CRITICAL");
            std::cout << "         -> Ly do vi pham: \"" << alert.getReason() << "\"\n";
            passed_tests++;
        } else {
            printFail("Thiet bi Hacker", "Khong danh dau muc CRITICAL cho thiet bi root!");
        }
    }

    // -------------------------------------------------------------
    // TỔNG KẾT
    // -------------------------------------------------------------
    std::cout << "\n" << COLOR_CYAN << "--------------------------------------------------------" << COLOR_RESET << "\n";
    if (passed_tests == total_tests) {
        std::cout << COLOR_GREEN << COLOR_BOLD 
                  << "  >>> KET QUA: " << passed_tests << "/" << total_tests 
                  << " TEST PASS HOAN TOAN! SAN SANG CHO BƯỚC 2.2 <<<\n" 
                  << COLOR_RESET;
    } else {
        std::cout << COLOR_RED << COLOR_BOLD 
                  << "  >>> KET QUA: " << passed_tests << "/" << total_tests << " TEST PASS. VUI LONG KIEM TRA LAI! <<<\n" 
                  << COLOR_RESET;
    }
    std::cout << COLOR_CYAN << "========================================================\n" << COLOR_RESET << std::endl;

    return (passed_tests == total_tests) ? 0 : 1;
}

