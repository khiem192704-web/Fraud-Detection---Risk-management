#pragma once
#include<string>
#include<vector>
#include"core/domain/Account.h"
#include"IRepository.h"

class AccountRepo : public IRepository<Account> {
    private:
        std::vector<Account> accounts;
        std::string file_path;
    public:
        explicit AccountRepo(std::string file_path = "");
        virtual ~AccountRepo() = default;

        bool add(const Account& entity) override;
        Account* findById(const std::string& id) override;
        const std::vector<Account>& getAll() const override;
        bool update(const Account& entity) override;
        bool remove(const std::string& id) override;

        std::vector<Account*> findByCustomerId(const std::string& customer_id);
        
        bool loadFromFile(const std::string&);
        bool saveToFile(const std::string&) const;
        int count() const;
        void clear();
};