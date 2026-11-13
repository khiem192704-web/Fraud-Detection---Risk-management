#pragma once
#include<string>
#include<vector>
#include"core/domain/Customer.h"
#include"IRepository.h"

class CustomerRepo : public IRepository<Customer> {
    private:
        std::vector<Customer> customers;
        std::string file_path;  
    public:
        explicit CustomerRepo(std::string file_path = "data/customers.txt");
        virtual ~CustomerRepo() = default;

        bool add(const Customer&) override;
        Customer* findById(const std::string&) override;
        const std::vector<Customer>& getAll() const override;
        bool update(const Customer&) override;
        bool remove(const std::string& ) override;

        bool loadFromFile(const std::string&);
        bool saveToFile(const std::string&) const;
        int count() const;
        void clear();
};