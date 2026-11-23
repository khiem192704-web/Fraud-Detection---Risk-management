#include "ui/ConsoleUI.h"
#include "bll/TransactionBLL.h"
#include <iostream>

int main() {
    ConsoleUI::printBanner();

    TransactionBLL bll;
    int choice = 0;

    while (true) {
        ConsoleUI::printMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::string discard;
            std::cin >> discard;
            continue;
        }

        switch (choice) {
            case 1:
                ConsoleUI::runAutomatedScenarios(bll);
                break;
            case 2:
                ConsoleUI::runInteractiveMode(bll);
                break;
            case 3:
                ConsoleUI::printReviewQueue(bll);
                break;
            case 4:
                ConsoleUI::runAdminManagement(bll);
                break;
            case 5:
                std::cout << "\nCam on ban da su dung he thong EPFD-RAS. Tam biet!\n";
                return 0;
            default:
                std::cout << "Lua chon khong hop le, vui long chon tu 1 den 5.\n";
                break;
        }
    }

    return 0;
}
