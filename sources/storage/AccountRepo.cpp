#include"include/storage/AccountRepo.h"

AccountRepo::AccountRepo(std::string file_path = "") : file_path(file_path) {}

bool AccountRepo::add(const Account& entity) {
    accounts.push_back(entity);
    return true;
}

Account* AccountRepo::findById(const std::string& id) {
    for (int i = 0; i < accounts.size(); ++i) {
        if (accounts[i].getAccountId() == id) {
            return &accounts[i];
        }
    }
    return nullptr;
}

const std::vector<Account>& AccountRepo::getAll() const {
    return accounts;
}

bool AccountRepo::update(const Account& entity) {
    for (int i = 0; i < accounts.size(); ++i) {
        if (accounts[i].getAccountId() == entity.getAccountId()) {
            accounts[i] = entity;
            return true;
        }
    }
    return false;
}
bool AccountRepo::remove(const std::string& id) {
    for (int i = 0; i < accounts.size(); ++i) {
        if (accounts[i].getAccountId() == id) {
            accounts.erase(accounts.begin() + i);
            return true;
        }
    }
    return false;
}

std::vector<Account*> AccountRepo::findByCustomerId(const std::string& customer_id) {
    std::vector<Account*> result;
    for (int i = 0; i < accounts.size(); ++i) {
        if (accounts[i].getCustomerId() == customer_id) {
            result.push_back(&accounts[i]);
        }
    }
    return result;
}

bool AccountRepo::loadFromFile(const std::string& filePath) {
    std::string path = filePath.empty() ? this->file_path : filePath;
    std::ifstream ifs(path);
    if (!ifs.is_open()) return false;
    this->accounts.clear();
    std::string line;
    while(std::getline(ifs, line)) {
        std::istringstream iss(line);
        if(line.empty() || line[0] == '#') continue; // Skip empty lines and comments
        std::stringstream ss(line);
        std::string account_id, customer_id, balance, currency, status;
        if(std::getline(ss, account_id, '|') &&
           std::getline(ss, customer_id, '|') &&
           std::getline(ss, balance, '|') &&
           std::getline(ss, currency, '|') &&
           std::getline(ss, status, '|')) {
            Account acc(account_id, customer_id, std::stod(balance), currency);
            int statusType = std::stoi(status);
            if (statusType == static_cast<int>(AccountStatus::FROZEN)) {
                acc.freeze();
            } else if (statusType == static_cast<int>(AccountStatus::CLOSED)) {
                acc.close();
            }
            this->accounts.push_back(acc);
        }
    }
    return true;
}

bool AccountRepo::saveToFile(const std::string& path) const {
    // Implementation for saving accounts to file
    return true;
}

int AccountRepo::count() const {
    return accounts.size();
}

void AccountRepo::clear() {
    accounts.clear();
}