#include"storage/AccountRepo.h"
#include<fstream>
#include<sstream>

AccountRepo::AccountRepo(std::string file_path) : file_path(file_path) {}

bool AccountRepo::add(const Account& account) {
    if(this->findById(account.getAccountId()) != nullptr) {
        return false; // Account with the same ID already exists
    }
    this->accounts.push_back(account);
    return true;
}

Account* AccountRepo::findById(const std::string& id) {
    for (int i = 0; i < (int)this->accounts.size(); ++i) {
        if (this->accounts[i].getAccountId() == id) {
            return &this->accounts[i];
        }
    }
    return nullptr;
}

const std::vector<Account>& AccountRepo::getAll() const {
    return this->accounts;
}

bool AccountRepo::update(const Account& account) {
    for (int i = 0; i < (int)this->accounts.size(); ++i) {
        if (this->accounts[i].getAccountId() == account.getAccountId()) {
            this->accounts[i] = account;
            return true;
        }
    }
    return false;
}

bool AccountRepo::remove(const std::string& id) {
    for (int i = 0; i < (int)this->accounts.size(); ++i) {
        if (this->accounts[i].getAccountId() == id) {
            this->accounts.erase(this->accounts.begin() + i);
            return true;
        }
    }
    return false;
}

std::vector<Account*> AccountRepo::findByCustomerId(const std::string& customer_id) {
    std::vector<Account*> result;
    for (int i = 0; i < (int)this->accounts.size(); ++i) {
        if (this->accounts[i].getCustomerId() == customer_id) {
            result.push_back(&this->accounts[i]);
        }
    }
    return result;
}

bool AccountRepo::loadFromFile(const std::string& filePath) {
    std::string path = filePath.empty() ? this->file_path : filePath;
    std::ifstream ifile(path);
    if (!ifile.is_open()) return false;
    this->accounts.clear();
    std::string line;
    while(std::getline(ifile, line)) {
        if(line.empty() || line[0] == '#') continue;
        std::stringstream ss(line);
        std::string account_id, customer_id, balanceStr, currency, statusStr, timeStr;
        if(std::getline(ss, account_id, '|') &&
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

bool AccountRepo::saveToFile(const std::string& filePath) const {
    std::string path = filePath.empty() ? this->file_path : filePath;
    std::ofstream ofile(path);
    if (!ofile.is_open()) return false;
    for (int i = 0; i < (int)this->accounts.size(); ++i) {
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

int AccountRepo::count() const {
    return (int)this->accounts.size();
}

void AccountRepo::clear() {
    this->accounts.clear();
}
