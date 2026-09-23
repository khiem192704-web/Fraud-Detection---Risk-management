#include"core/domain/location/location.h"
#include<stdexcept>

Location::Location(){};
Location::Location(const std::string& country, const std::string& city, const std::string& postal_code, double latitude, double longitude): country(country), city(city), postal_code(postal_code), latitude(latitude), longitude(longitude) {}

void Location::setCountry(const std::string& country) {
    this->country = country;
}

void Location::setCity(const std::string& city) {
    this->city = city;
}

void Location::setPostalCode(const std::string& postal_code) {
    this->postal_code = postal_code;
}

void Location::setCoordinates(double latitude, double longitude) {
    if(latitude < -90.0 || latitude > 90.0) {
        throw std::invalid_argument("Latitude must be between -90 and 90 degrees.");
    }
    if(longitude < -180.0 || longitude > 180.0) {
        throw std::invalid_argument("Longitude must be between -180 and 180 degrees.");
    }
    this->latitude = latitude;
    this->longitude = longitude;
}

std::string& Location::getCountry() const { return country; }
std::string& Location::getCity() const{return city;}
std::string& Location::getPostalCode() const{return postal_code;}
double Location::getLatitude() const{return latitude;}
double Location::getLongitude() const{return longitude;}

bool Location::isValid() const {
    return !country.empty() &&(latitude >= -90.0 && latitude <= 90.0)&&(longitude >= -180.0 && longitude <= 180.0);
}

double Location::distanceTo(const Location& other) const {
    if(!isValid() || !other.isValid()) {
        throw std::invalid_argument("Both locations must be valid to calculate distance.");
    }
    const double M_PI = acos(-1.0);
    // đổi vĩ độ và kinh độ sang radian
    double lat1_rad = latitude * M_PI / 180.0;
    double lon1_rad = longitude * M_PI / 180.0;
    double lat2_rad = other.latitude * M_PI / 180.0;
    double lon2_rad = other.longitude * M_PI / 180.0;
    // Haversine formula
    double dlat = lat2_rad - lat1_rad;
    double dlon = lon2_rad - lon1_rad;

    double a = std::sin(dlat/2) * std::sin(dlat/2) + std::cos(lat1_rad) * std::cos(lat2_rad) * std::sin(dlon/2) * std::sin(dlon/2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    // Ban kinh cua Trai Dat
    const double R = 6371.0;
    return R * c; // khoang cach(km)
}

std::string Location::summary() const {
    return "City: " + city +"[" + std::to_string(latitude) + ", " + std::to_string(longitude) + "]";
}