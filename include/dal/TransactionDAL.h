#pragma once
#include <string>
#include <vector>
#include "core/models/Transaction.h"

class TransactionDAL {
private:
    std::vector<Transaction> transactions;
    std::string file_path;

public:
    explicit TransactionDAL(std::string filePath = "data/transactions.csv");

    bool add(const Transaction& entity);
    bool insert(const Transaction& entity) { return add(entity); }
    Transaction* findById(const std::string& id);
    Transaction* getById(const std::string& id) { return findById(id); }
    const std::vector<Transaction>& getAll() const;
    bool update(const Transaction& entity);
    bool remove(const std::string& id);
    bool deleteById(const std::string& id) { return remove(id); }

    std::vector<Transaction> findByAccountId(const std::string& accountId) const;
    std::vector<Transaction> findByCustomerId(const std::string& customerId) const;
    std::vector<Transaction> getRecentTransactions(const std::string& accountId, int windowSeconds) const;

    bool loadFromDisk(const std::string& customPath = "");
    bool loadFromFile(const std::string& customPath = "") { return loadFromDisk(customPath); }
    bool syncToDisk(const std::string& customPath = "") const;
    bool saveToFile(const std::string& customPath = "") const { return syncToDisk(customPath); }

    int count() const { return static_cast<int>(transactions.size()); }
    void clear() { transactions.clear(); }
};

using TransactionRepository = TransactionDAL;
