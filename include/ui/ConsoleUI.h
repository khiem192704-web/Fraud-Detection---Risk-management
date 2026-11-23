#pragma once
#include <string>
#include "bll/TransactionBLL.h"

class ConsoleUI {
public:
    static void printBanner();
    static void printMenu();
    static void runAutomatedScenarios(TransactionBLL& bll);
    static void runInteractiveMode(TransactionBLL& bll);
    static void printReviewQueue(TransactionBLL& bll);
    static void runAdminManagement(TransactionBLL& bll);

    // Hỗ trợ in màu ANSI
    static void printSuccess(const std::string& msg);
    static void printWarning(const std::string& msg);
    static void printDanger(const std::string& msg);
    static void printInfo(const std::string& msg);
};
