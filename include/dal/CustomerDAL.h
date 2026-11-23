#pragma once
#include <string>
#include <vector>
#include "core/models/Customer.h"

class CustomerDAL {
private:
    std::vector<Customer> customers;
    std::string file_path;

public:
    explicit CustomerDAL(std::string file_path = "data/customers.txt");
    ~CustomerDAL() = default;

    // Các thao tác dữ liệu chuẩn DAL
    bool insert(const Customer& customer);
    Customer* getById(const std::string& id);
    const std::vector<Customer>& getAll() const;
    bool update(const Customer& otherCustomer);
    bool deleteById(const std::string& id);

    // Alias tương thích ngược
    bool add(const Customer& customer) { return insert(customer); }
    Customer* findById(const std::string& id) { return getById(id); }
    bool remove(const std::string& id) { return deleteById(id); }

    // Thao tác đọc/ghi file
    bool loadFromFile(const std::string& filePath = "");
    bool saveToFile(const std::string& filePath = "") const;
    int count() const;
    void clear();
};
