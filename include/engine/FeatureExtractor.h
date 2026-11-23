#pragma once

#include <string>
#include <chrono>
#include <vector>
#include <deque>
#include "core/common/Types.h"
#include "core/models/Transaction.h"
#include "core/models/Customer.h"

class FeatureExtractor {
private:
    std::deque<Timestamp> sliding_window;
    std::chrono::seconds window_duration{60}; // Cửa sổ kiểm tra mặc định: 60 giây

public:
    explicit FeatureExtractor(std::chrono::seconds duration = std::chrono::seconds(60));

    // Trích xuất vector 4 đặc trưng: [amount, velocity_count, geo_distance_km, is_device_risk]
    std::vector<double> extract(const Transaction& tx, const Customer& customer);

    void reset();
    std::vector<std::string> getFeatureNames() const;

private:
    double extractAmount(const Transaction& tx) const;
    double extractVelocity(const Transaction& tx);
    double extractGeoDistance(const Transaction& tx, const Customer& customer) const;
    double extractDeviceRisk(const Transaction& tx) const;
};
