#include "engine/FeatureExtractor.h"
#include <cmath>

FeatureExtractor::FeatureExtractor(std::chrono::seconds duration)
    : window_duration(duration) {}

void FeatureExtractor::reset() {
    sliding_window.clear();
}

std::vector<std::string> FeatureExtractor::getFeatureNames() const {
    std::vector<std::string> names;
    names.push_back("amount");
    names.push_back("velocity_count");
    names.push_back("geo_distance_km");
    names.push_back("is_device_risk");
    return names;
}

double FeatureExtractor::extractAmount(const Transaction& tx) const {
    return tx.GetAmount();
}

double FeatureExtractor::extractVelocity(const Transaction& tx) {
    Timestamp now = tx.GetTimestamp();

    while (!sliding_window.empty()) {
        auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - sliding_window.front()).count();
        if (diff > window_duration.count()) {
            sliding_window.pop_front();
        } else {
            break;
        }
    }

    sliding_window.push_back(now);
    return static_cast<double>(sliding_window.size());
}

double FeatureExtractor::extractGeoDistance(const Transaction& tx, const Customer& customer) const {
    const Location& tx_loc = tx.GetLocation();
    const Location& home_loc = customer.getHomeLocation();

    if (!tx_loc.isValid() || !home_loc.isValid()) {
        return 0.0;
    }

    try {
        return tx_loc.distanceTo(home_loc);
    } catch (...) {
        return 0.0;
    }
}

double FeatureExtractor::extractDeviceRisk(const Transaction& tx) const {
    const Device& dev = tx.GetDevice();
    if (dev.GetIs_Virtual() || dev.GetIs_Hacker() || !dev.is_Safe()) {
        return 1.0;
    }
    return 0.0;
}

std::vector<double> FeatureExtractor::extract(const Transaction& tx, const Customer& customer) {
    std::vector<double> features;
    features.push_back(extractAmount(tx));                // [0] amount
    features.push_back(extractVelocity(tx));              // [1] velocity_count
    features.push_back(extractGeoDistance(tx, customer)); // [2] geo_distance_km
    features.push_back(extractDeviceRisk(tx));            // [3] is_device_risk
    return features;
}
