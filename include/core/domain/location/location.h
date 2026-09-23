#pragma once
#include<string>
#include<cmath>

class Location {
    private:
        std::string country;
        std::string city;
        std::string postal_code;
        double latitude{0.0}; // vĩ độ
        double longitude{0.0}; // kinh độ
    public:
        Location(){};
        Location(const std::string&, const std::string&, const std::string&, double, double);
        void setCountry(const std::string&);
        void setCity(const std::string&);
        void setPostalCode(const std::string&);
        void setCoordinates(double, double);

        std::string& getCountry() const;
        std::string& getCity() const;
        std::string& getPostalCode() const;
        double getLatitude() const;
        double getLongitude() const;

        bool isValid() const;
        double distanceTo(const Location&) const;
        std::string summary() const;
};