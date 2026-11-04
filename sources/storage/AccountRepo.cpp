#include"include/storage/AccountRepo.h"

AccountRepo::AccountRepo(std::string file_path = "") : file_path(file_path) {}

bool AccountRepo::add(const Account& entity) {
    this->accounts.push_back(entity);
    return true;
}

Account* AccountRepo::findById(const std::string& id) {
    for (int i = 0; i < this->accounts.size(); ++i) {
        if (this->accounts[i].getAccountId() == id) {
            return &this->accounts[i];
        }
    }
    return nullptr;
}

const std::vector<Account>& AccountRepo::getAll() const {
    return this->accounts;
}

bool AccountRepo::update(const Account& entity) {
    for (int i = 0; i < this->accounts.size(); ++i) {
        if (this->accounts[i].getAccountId() == entity.getAccountId()) {
            this->accounts[i] = entity;
            return true;
        }
    }
    return false;
}
bool AccountRepo::remove(const std::string& id) {
    for (int i = 0; i < this->accounts.size(); ++i) {
        if (this->accounts[i].getAccountId() == id) {
            this->accounts.erase(this->accounts.begin() + i);
            return true;
        }
    }
    return false;
}

std::vector<Account*> AccountRepo::findByCustomerId(const std::string& customer_id) {
    std::vector<Account*> result;
    for (int i = 0; i < this->accounts.size(); ++i) {
        if (this->  accounts[i].getCustomerId() == customer_id) {
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
        std::istringstream iss(line);
        if(line.empty() || line[0] == '#') continue; 
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

bool AccountRepo::saveToFile(const std::string& filePath) const {
    std::string path = filePath.empty() ? this->file_path : filePath;
    std::ofstream ofile(path);
    if (!ofile.is_open()) return false;
    for (int i = 0; i < this->accounts.size(); ++i) {
        ofile << this->accounts[i].getAccountId() << '|' << this->accounts[i].getCustomerId() << '|' << this->accounts[i].getBalance() << '|' << this->accounts[i].getCurrency() << '|' << static_cast<int>(this->accounts[i].getStatus()) << '\n';
    }
    return true;
}

int AccountRepo::count() const {
    return this->accounts.size();
}

void AccountRepo::clear() {
    this->accounts.clear();
}