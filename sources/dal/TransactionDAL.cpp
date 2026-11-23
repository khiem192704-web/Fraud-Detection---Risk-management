#include "dal/TransactionDAL.h"
#include <fstream>
#include <sstream>
#include <chrono>

TransactionDAL::TransactionDAL(std::string filePath)
    : file_path(std::move(filePath)) {}

bool TransactionDAL::add(const Transaction& entity) {
    transactions.push_back(entity);
    return true;
}

Transaction* TransactionDAL::findById(const std::string& id) {
    for (size_t i = 0; i < transactions.size(); ++i) {
        if (transactions[i].getTransactionId() == id) {
            return &transactions[i];
        }
    }
    return nullptr;
}

const std::vector<Transaction>& TransactionDAL::getAll() const {
    return transactions;
}

bool TransactionDAL::update(const Transaction& entity) {
    for (size_t i = 0; i < transactions.size(); ++i) {
        if (transactions[i].getTransactionId() == entity.getTransactionId()) {
            transactions[i] = entity;
            return true;
        }
    }
    return false;
}

bool TransactionDAL::remove(const std::string& id) {
    for (size_t i = 0; i < transactions.size(); ++i) {
        if (transactions[i].getTransactionId() == id) {
            transactions.erase(transactions.begin() + i);
            return true;
        }
    }
    return false;
}

std::vector<Transaction> TransactionDAL::findByAccountId(const std::string& accountId) const {
    std::vector<Transaction> result;
    for (size_t i = 0; i < transactions.size(); ++i) {
        if (transactions[i].getAccountId() == accountId) {
            result.push_back(transactions[i]);
        }
    }
    return result;
}

std::vector<Transaction> TransactionDAL::findByCustomerId(const std::string& customerId) const {
    std::vector<Transaction> result;
    for (size_t i = 0; i < transactions.size(); ++i) {
        if (transactions[i].getCustomerId() == customerId) {
            result.push_back(transactions[i]);
        }
    }
    return result;
}

std::vector<Transaction> TransactionDAL::getRecentTransactions(const std::string& accountId, int windowSeconds) const {
    std::vector<Transaction> result;
    auto now = std::chrono::system_clock::now();

    for (size_t i = 0; i < transactions.size(); ++i) {
        if (transactions[i].getAccountId() == accountId) {
            auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - transactions[i].getTimestamp()).count();
            if (diff >= 0 && diff <= windowSeconds) {
                result.push_back(transactions[i]);
            }
        }
    }
    return result;
}

bool TransactionDAL::loadFromDisk(const std::string& customPath) {
    std::string path = customPath.empty() ? file_path : customPath;
    std::ifstream ifs(path);
    if (!ifs.is_open()) return false;

    transactions.clear();
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty() || line[0] == '#' || line.rfind("tx_id", 0) == 0) continue;
        std::stringstream ss(line);
        std::string txId, typeStr, custId, accId, amtStr, curr, tsStr, city, country;
        std::string latStr, lonStr, devId, devIp, cardNum, mId, mName, mMcc, statusStr;
        std::string velocityStr, distStr, devRiskStr, fraudStr;

        if (std::getline(ss, txId, ',') &&
            std::getline(ss, typeStr, ',') &&
            std::getline(ss, custId, ',') &&
            std::getline(ss, accId, ',') &&
            std::getline(ss, amtStr, ',') &&
            std::getline(ss, curr, ',') &&
            std::getline(ss, tsStr, ',') &&
            std::getline(ss, city, ',') &&
            std::getline(ss, country, ',') &&
            std::getline(ss, latStr, ',') &&
            std::getline(ss, lonStr, ',') &&
            std::getline(ss, devId, ',') &&
            std::getline(ss, devIp, ',') &&
            std::getline(ss, cardNum, ',') &&
            std::getline(ss, mId, ',') &&
            std::getline(ss, mName, ',') &&
            std::getline(ss, mMcc, ',') &&
            std::getline(ss, statusStr, ',')) {

            double amt = 0.0, lat = 0.0, lon = 0.0;
            int typeInt = 0, statusInt = 0;
            try {
                amt = std::stod(amtStr);
                lat = std::stod(latStr);
                lon = std::stod(lonStr);
                typeInt = std::stoi(typeStr);
                statusInt = std::stoi(statusStr);
            } catch (...) {}

            Timestamp timePoint = std::chrono::system_clock::now();
            try {
                long long epochSec = std::stoll(tsStr);
                timePoint = Timestamp(std::chrono::seconds(epochSec));
            } catch (...) {}

            Location loc(country, city, lat, lon);
            Device dev(devId, devIp);
            Merchant mer(mId, mName, mMcc, 5.0);

            Transaction tx(txId, static_cast<TransactionType>(typeInt),
                            custId, accId, amt, curr, loc, dev, cardNum, mer, timePoint);
            tx.setStatus(static_cast<TransactionStatus>(statusInt));
            transactions.push_back(tx);
        }
    }
    return true;
}

bool TransactionDAL::syncToDisk(const std::string& customPath) const {
    std::string path = customPath.empty() ? file_path : customPath;
    std::ofstream ofs(path, std::ios::trunc);
    if (!ofs.is_open()) return false;

    ofs << "tx_id,type,customer_id,account_id,amount,currency,timestamp,city,country,latitude,longitude,device_id,device_ip,card_number,merchant_id,merchant_name,merchant_mcc,status,velocity_count,geo_distance_km,is_device_risk,is_fraud\n";
    for (size_t i = 0; i < transactions.size(); ++i) {
        const auto& t = transactions[i];
        long long epochSec = std::chrono::duration_cast<std::chrono::seconds>(
            t.getTimestamp().time_since_epoch()).count();

        int isFraud = (t.getStatus() == TransactionStatus::BLOCKED) ? 1 : 0;
        int isDevRisk = t.getDevice().isHighRisk() ? 1 : 0;

        ofs << t.getTransactionId() << ","
            << static_cast<int>(t.getType()) << ","
            << t.getCustomerId() << ","
            << t.getAccountId() << ","
            << t.getAmount() << ","
            << t.getCurrency() << ","
            << epochSec << ","
            << t.getLocation().getCity() << ","
            << t.getLocation().getCountry() << ","
            << t.getLocation().getLatitude() << ","
            << t.getLocation().getLongitude() << ","
            << t.getDevice().getDeviceId() << ","
            << t.getDevice().getIpAddress() << ","
            << t.getRawCardNumber() << ","
            << t.getMerchant().getMerchantId() << ","
            << t.getMerchant().getName() << ","
            << t.getMerchant().getMcc() << ","
            << static_cast<int>(t.getStatus()) << ","
            << 1 << "," // velocity_count
            << 10.0 << "," // geo_distance_km
            << isDevRisk << ","
            << isFraud << "\n";
    }
    return true;
}
