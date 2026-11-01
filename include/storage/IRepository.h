#pragma once
#include <string>
#include <vector>

template<typename T>
class IRepository {
public:
    virtual ~IRepository() = default;
    virtual bool add(const T& entity) = 0;
    virtual T* findById(const std::string& id) = 0;
    virtual const vector<T>& getAll() const = 0;
    virtual bool update(const T& entity) = 0;
    virtual bool remove(const std::string& id) = 0;
};