#pragma once
#include <string>
#include <vector>
#include "core/models/Account.h"

class AccountDAL {
private:
    std::vector<Account> accounts;
    std::string file_path;

public:
    explicit AccountDAL(std::string file_path = "data/accounts.txt");
    ~AccountDAL() = default;

    // Các thao tác dữ liệu chuẩn DAL
    bool insert(const Account& account);
    Account* getById(const std::string& id);
    std::vector<Account*> getByCustomerId(const std::string& customer_id);
    const std::vector<Account>& getAll() const;
    bool update(const Account& account);
    bool deleteById(const std::string& id);

    // Alias tương thích ngược
    bool add(const Account& account) { return insert(account); }
    Account* findById(const std::string& id) { return getById(id); }
    std::vector<Account*> findByCustomerId(const std::string& customer_id) { return getByCustomerId(customer_id); }
    bool remove(const std::string& id) { return deleteById(id); }

    // Nghiệp vụ tài khoản trực tiếp
    bool freezeAccount(const std::string& accountId);
    bool unfreezeAccount(const std::string& accountId);

    // Thao tác đọc/ghi file
    bool loadFromFile(const std::string& filePath = "");
    bool saveToFile(const std::string& filePath = "") const;
    bool loadFromDisk(const std::string& filePath = "") { return loadFromFile(filePath); }
    bool syncToDisk(const std::string& filePath = "") const { return saveToFile(filePath); }
    int count() const;
    void clear();
};

using AccountRepository = AccountDAL;
