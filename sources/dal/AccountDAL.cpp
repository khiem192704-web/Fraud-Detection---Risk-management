#include "dal/AccountDAL.h"
#include <fstream>
#include <sstream>

AccountDAL::AccountDAL(std::string file_path) : file_path(file_path) {}

bool AccountDAL::insert(const Account& account) {
    if (this->getById(account.getAccountId()) != nullptr) {
        return false; // Tài khoản với ID này đã tồn tại
    }
    this->accounts.push_back(account);
    return true;
}

Account* AccountDAL::getById(const std::string& id) {
    for (size_t i = 0; i < this->accounts.size(); ++i) {
        if (this->accounts[i].getAccountId() == id) {
            return &this->accounts[i];
        }
    }
    return nullptr;
}

std::vector<Account*> AccountDAL::getByCustomerId(const std::string& customer_id) {
    std::vector<Account*> result;
    for (size_t i = 0; i < this->accounts.size(); ++i) {
        if (this->accounts[i].getCustomerId() == customer_id) {
            result.push_back(&this->accounts[i]);
        }
    }
    return result;
}

const std::vector<Account>& AccountDAL::getAll() const {
    return this->accounts;
}

bool AccountDAL::update(const Account& account) {
    for (size_t i = 0; i < this->accounts.size(); ++i) {
        if (this->accounts[i].getAccountId() == account.getAccountId()) {
            this->accounts[i] = account;
            return true;
        }
    }
    return false;
}

bool AccountDAL::deleteById(const std::string& id) {
    for (size_t i = 0; i < this->accounts.size(); ++i) {
        if (this->accounts[i].getAccountId() == id) {
            this->accounts.erase(this->accounts.begin() + i);
            return true;
        }
    }
    return false;
}

bool AccountDAL::freezeAccount(const std::string& accountId) {
    Account* acc = getById(accountId);
    if (acc) {
        acc->freeze();
        return true;
    }
    return false;
}

bool AccountDAL::unfreezeAccount(const std::string& accountId) {
    Account* acc = getById(accountId);
    if (acc) {
        acc->unfreeze();
        return true;
    }
    return false;
}

bool AccountDAL::loadFromFile(const std::string& filePath) {
    std::string path = filePath.empty() ? this->file_path : filePath;
    std::ifstream ifile(path);
    if (!ifile.is_open()) return false;
    this->accounts.clear();
    std::string line;
    while (std::getline(ifile, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string account_id, customer_id, balanceStr, currency, statusStr, timeStr;
        if (std::getline(ss, account_id, '|') &&
            std::getline(ss, customer_id, '|') &&
            std::getline(ss, balanceStr, '|') &&
            std::getline(ss, currency, '|') &&
            std::getline(ss, statusStr, '|')) {
            double balance = 0.0;
            int statusInt = 0;
            try {
                balance = std::stod(balanceStr);
                statusInt = std::stoi(statusStr);
            } catch (...) {}
            Timestamp time = std::chrono::system_clock::now();
            if (std::getline(ss, timeStr, '|')) {
                try {
                    long long t = std::stoll(timeStr);
                    time = Timestamp(std::chrono::seconds(t));
                } catch (...) {}
            }
            Account acc(account_id, customer_id, balance, currency, static_cast<AccountStatus>(statusInt), time);
            this->accounts.push_back(acc);
        }
    }
    return true;
}

bool AccountDAL::saveToFile(const std::string& filePath) const {
    std::string path = filePath.empty() ? this->file_path : filePath;
    std::ofstream ofile(path);
    if (!ofile.is_open()) return false;
    for (size_t i = 0; i < this->accounts.size(); ++i) {
        long long time = std::chrono::duration_cast<std::chrono::seconds>(
            this->accounts[i].getCreatedAt().time_since_epoch()).count();
        ofile << this->accounts[i].getAccountId() << '|'
              << this->accounts[i].getCustomerId() << '|'
              << this->accounts[i].getBalance() << '|'
              << this->accounts[i].getCurrency() << '|'
              << static_cast<int>(this->accounts[i].getStatus()) << '|'
              << time << '\n';
    }
    return true;
}

int AccountDAL::count() const {
    return static_cast<int>(this->accounts.size());
}

void AccountDAL::clear() {
    this->accounts.clear();
}
