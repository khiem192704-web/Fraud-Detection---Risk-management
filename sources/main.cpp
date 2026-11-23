#include <iostream>
#include <iomanip>
#include "bll/TransactionBLL.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

void inTieuDe(const std::string& tieu_de) {
    std::cout << "\n" << CYAN << BOLD 
              << "========================================================================\n"
              << "  " << tieu_de << "\n"
              << "========================================================================\n" << RESET;
}

void chayKichBanDemo(TransactionBLL& bll) {
    Location ha_noi("Vietnam", "Ha Noi", 21.0285, 105.854);
    Location hcm("Vietnam", "Ho Chi Minh", 10.8231, 106.63);

    // -------------------------------------------------------------------------
    // KỊCH BẢN 1: GIAO DỊCH HỢP LỆ (ALICE MUA HÀNG $45 TẠI NHÀ HÀ NỘI)
    // -------------------------------------------------------------------------
    inTieuDe("KICH BAN 1: GIAO DICH HOP LE TAI NHA (ALICE - $45.00)");
    {
        Device dev_alice("DEV_ALICE_PHONE", "192.168.1.10", "FP_ALICE_OK", ha_noi, false, false);
        Transaction tx1("TX_DEMO_01", TransactionType::BUY, "CUST_ALICE", "M_WINMART", "ACC_001", 
                        45.0, "USD", std::chrono::system_clock::now(), ha_noi, "192.168.1.10", 
                        dev_alice, "M_WINMART", PaymentType::CARD);

        TransactionResult res = bll.processTransaction(tx1);

        std::cout << "  - Khach hang: Alice | Tai khoan nguon: ACC_001 | So tien: $45.00\n";
        std::cout << "  - Diem Rule: " << std::fixed << std::setprecision(1) << res.assessment.getRuleScore()
                  << " | Diem ML: " << res.assessment.getMlScore()
                  << " | Tong diem rui ro: " << res.assessment.getCombinedScore() << "/100.0\n";
        std::cout << "  - Ket qua: " << GREEN << BOLD << res.message << RESET << "\n";
    }

    // -------------------------------------------------------------------------
    // KỊCH BẢN 2: SPAM QUẸT THẺ DỒN DẬP (KÍCH HOẠT VELOCITY RULE)
    // -------------------------------------------------------------------------
    inTieuDe("KICH BAN 2: TAN CONG QUET THE DON DAP (SPAM 6 LAN LIEN TIEP / 10s)");
    {
        Device dev_spam("DEV_BOB_PHONE", "14.232.208.1", "FP_BOB", ha_noi, false, false);
        for (int i = 1; i <= 6; ++i) {
            Transaction tx_spam("TX_SPAM_0" + std::to_string(i), TransactionType::BUY, "CUST_BOB", 
                                "M_ONLINE", "ACC_002", 50.0, "USD", std::chrono::system_clock::now(), 
                                ha_noi, "14.232.208.1", dev_spam, "M_ONLINE", PaymentType::CARD);

            TransactionResult res = bll.processTransaction(tx_spam);
            std::cout << "  Lan quet " << i << ": Diem rui ro = " << std::setw(5) << res.assessment.getCombinedScore();
            if (res.action == DecisionAction::APPROVE) {
                std::cout << " -> " << GREEN << "APPROVE" << RESET << "\n";
            } else {
                std::cout << " -> " << RED << BOLD << "BLOCK (" << res.message << ")" << RESET << "\n";
            }
        }
    }

    // -------------------------------------------------------------------------
    // KỊCH BẢN 3: GIAO DỊCH NGUY HIỂM (MÁY ẢO + CÁCH XA 1140KM + SỐ TIỀN $3500)
    // -------------------------------------------------------------------------
    inTieuDe("KICH BAN 3: GIAO DICH GIAN LAN NGUY HIEM (MAY AO + VI TRI LA + TIEN LON)");
    {
        Device dev_hacker("DEV_EMU_ROOT", "10.0.2.15", "FP_ROOTED", hcm, true, false); // Cờ Virtual = true
        Transaction tx3("TX_DEMO_03", TransactionType::TRANSFER, "CUST_ALICE", "M_CRYPTO", "ACC_001", 
                        3500.0, "USD", std::chrono::system_clock::now(), hcm, "10.0.2.15", 
                        dev_hacker, "M_CRYPTO", PaymentType::CARD);

        TransactionResult res = bll.processTransaction(tx3);

        std::cout << "  - So tien: $3500.00 | Vi tri: TP.HCM (Nha Alice tai Ha Noi, cach ~1140km)\n";
        std::cout << "  - Diem Rule: " << res.assessment.getRuleScore()
                  << " | Diem ML: " << res.assessment.getMlScore()
                  << " | Tong diem rui ro: " << res.assessment.getCombinedScore() << "/100.0\n";
        std::cout << "  - Danh sach canh bao gian lan phat hien:\n";
        const auto& reasons = res.assessment.getReasons();
        for (int i = 0; i < (int)reasons.size(); ++i) {
            std::cout << "    * " << YELLOW << reasons[i] << RESET << "\n";
        }
        std::cout << "  - Ket qua: " << RED << BOLD << res.message << RESET << "\n";
    }
}

int main() {
    std::cout << CYAN << BOLD;
    std::cout << "========================================================================\n";
    std::cout << "  EPFD-RAS: HE THONG QUAN TRI RUI RO & PHAT HIEN GIAN LAN THANH TOAN   \n";
    std::cout << "  Khoa CNTT - Truong Dai hoc Bach Khoa, Dai hoc Da Nang                 \n";
    std::cout << "========================================================================\n" << RESET;

    TransactionBLL bll;
    chayKichBanDemo(bll);

    return 0;
}
