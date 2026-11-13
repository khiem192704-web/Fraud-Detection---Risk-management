#include"include/storage/CustomerRepo.h"

CustomerRepo::CustomerRepo(std::string file_path = "") : file_path(file_path) {}

bool CustomerRepo::add(const Customer& customer){
    if(this->findById(customer.getCustomerId()) != nullptr) {
        return false; // Customer with the same ID already exists
    }
    this->customers.push_back(customer);
    return true;
}
Customer* CustomerRepo::findById(const std::string& id){
    for (int i = 0; i < this->customers.size(); ++i) {
        if (this->customers[i].getCustomerId() == id) {
            return &this->customers[i];
        }
    }
    return nullptr;
}
const std::vector<Customer>& CustomerRepo::getAll() const{
    return this->customers;
}
bool CustomerRepo::update(const Customer& otherCustomer){
    for (int i = 0; i < this->customers.size(); ++i) {
        if (this->customers[i].getCustomerId() == otherCustomer.getCustomerId()) {
            this->customers[i] = otherCustomer;
            return true;
        }
    }
    return false;
}
bool CustomerRepo::remove(const std::string& id){
    for (int i = 0; i < this->customers.size(); ++i) {
        if (this->customers[i].getCustomerId() == id) {
            this->customers.erase(this->customers.begin() + i);
            return true;
        }
    }
    return false;
}

bool CustomerRepo::loadFromFile(const std::string& filePath){
    std::string path = filePath.empty() ? this->file_path : filePath;
    std::ifstream ifile(path);
    if (!ifile.is_open()) return false;
    this->customers.clear();
    std::string line;
    while(std::getline(ifile, line)) {
        std::istringstream iss(line);
        std::string id, name, email, phone, country, city, latStr, lonStr;
        std::string tierStr, baselineRiskStr, kycStr, linkedAccsStr, trustedDevsStr;
        if (std::getline(iss, id, '|') &&
            std::getline(iss, name, '|') &&
            std::getline(iss, email, '|') &&
            std::getline(iss, phone, '|') &&
            std::getline(iss, city, '|') &&
            std::getline(iss, country, '|') &&
            std::getline(iss, latStr, '|') &&
            std::getline(iss, latStr, '|')) {

            double lat = 0.0, lon = 0.0;
            try {
                lat = std::stod(latStr);
                lon = std::stod(latStr);
            } catch (...) {}

            Location loc(country, city, lat, lon);
            Customer cust(id, name, email, phone, loc);

            if (std::getline(iss, tierStr, ',')) {
                try {
                    cust.setRiskTier(static_cast<CustomerRiskTier>(std::stoi(tierStr)));
                } catch (...) {}
            }
            if (std::getline(iss, baselineRiskStr, ',')) {
                try {
                    cust.setBaselineRiskScore(std::stod(baselineRiskStr));
                } catch (...) {}
            }
            if (std::getline(iss, kycStr, ',')) {
                try {
                    cust.setKycVerified(std::stoi(kycStr) != 0);
                } catch (...) {}
            }
            if (std::getline(iss, linkedAccsStr, ',')) {
                std::stringstream accSs(linkedAccsStr);
                std::string accId;
                while (std::getline(accSs, accId, ';')) {
                    if (!accId.empty()) cust.addLinkedAccount(accId);
                }
            }
            if (std::getline(iss, trustedDevsStr, ',')) {
                std::stringstream devSs(trustedDevsStr);
                std::string devId;
                while (std::getline(devSs, devId, ';')) {
                    if (!devId.empty()) cust.addTrustedDevice(devId);
                }
            }

            this->customers.push_back(cust);
        }
    }
}
bool CustomerRepo::saveToFile(const std::string& filePath) const{
    std::string path = filePath.empty() ? this->file_path : filePath;
    std::ofstream ofile(path);
    if (!ofile.is_open()) return false;
    for(const auto& customer : this->customers) {
        ofile << customer.getCustomerId() << '|'
              << customer.getFullName() << '|'
              << customer.getEmail() << '|'
              << customer.getPhoneNumber() << '|'
              << customer.getHomeLocation().getCity() << '|'
              << customer.getHomeLocation().getCountry() << '|'
              << customer.getHomeLocation().getLatitude() << '|'
              << customer.getHomeLocation().getLongitude() << '|'
              << static_cast<int>(customer.getRiskTier()) << ','
              << customer.getBaselineRiskScore() << ','
              << (customer.isKycVerified() ? 1 : 0) << ',';

        const auto& linkedAccounts = customer.getLinkedAccountIds();
        for (size_t i = 0; i < linkedAccounts.size(); ++i) {
            ofile << linkedAccounts[i];
            if (i < linkedAccounts.size() - 1) ofile << ';';
        }
        ofile << ',';

        const auto& trustedDevices = customer.getTrustedDeviceIds();
        for (size_t i = 0; i < trustedDevices.size(); ++i) {
            ofile << trustedDevices[i];
            if (i < trustedDevices.size() - 1) ofile << ';';
        }
        ofile << '\n';
    }
}

int CustomerRepo::count() const{
    return this->customers.size();
}

void CustomerRepo::clear(){ 
    this->customers.clear();
}