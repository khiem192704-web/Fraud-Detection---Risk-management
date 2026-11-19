#include "engine/NativeMLPredictor.h"
#include <cmath>
#include <fstream>
NativeMLPredictor::NativeMLPredictor() {
    // TODO 1: Khởi tạo mô hình Logistic Regression với bộ trọng số mặc định đã tiền huấn luyện:
    // - bias w0 = -2.8
    // - w1 = 0.002 (Số tiền giao dịch)
    // - w2 = 1.45  (Tần suất giao dịch)
    // - w3 = 2.10  (Rủi ro thiết bị: 0 hoặc 1)
    //
    // Gợi ý:
    bias = -2.8;
    weights.push_back(0.002);
    weights.push_back(1.45);
    weights.push_back(2.10);
    feature_names.push_back("amount");
    feature_names.push_back("velocity");
    feature_names.push_back("device_risk");
    ready = true;
}
NativeMLPredictor::NativeMLPredictor(double bias, const Vector<double>& weights) {
    // TODO 2: Khởi tạo với bias và danh sách weights truyền vào từ bên ngoài
    // Gợi ý:
    this->bias = bias;
    this->weights = weights;
    this->ready = true;
}
bool NativeMLPredictor::loadModel(const std::string& filepath) {
    // TODO 3: Nạp trọng số mô hình từ file cấu hình JSON (vd: data/model_weights.json):
    // 1. Mở file bằng std::ifstream file(filepath);
    // 2. Nếu file không tồn tại: giữ nguyên bộ trọng số mặc định và return false;
    // 3. Đọc từng dòng để trích xuất giá trị "bias" và mảng "weights":
    //    - Dòng chứa "\"bias\":": đọc số thực sau dấu hai chấm
    //    - Dòng chứa "\"weights\":": đọc các số thực kế tiếp vào danh sách trọng số mới
    // 4. Cập nhật this->bias và this->weights nếu đọc thành công
    // 5. Đặt ready = true; và return true;
    //
    // Gợi ý:
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }
    std::string line;
    bool in_weights = false;
    Vector<double> new_weights;
    while (std::getline(file, line)) {
        if (line.find("\"bias\":") != std::string::npos) {
            size_t pos = line.find(':');
            this->bias = std::stod(line.substr(pos + 1));
        }
        if (line.find("\"weights\":") != std::string::npos) {
            in_weights = true;
            continue;
        }
        if (in_weights) {
            if (line.find(']') != std::string::npos) {
                in_weights = false;
            } else {
                try {
                    size_t start = line.find_first_of("-0123456789.");
                    if (start != std::string::npos) {
                        new_weights.push_back(std::stod(line.substr(start)));
                    }
                } catch (...) {}
            }
        }
    }
    if (!new_weights.empty()) {
        this->weights = new_weights;
    }
    this->ready = true;
    return true;
}

double NativeMLPredictor::predictProbability(const Vector<double>& features) const {
    // TODO 4: Tính toán xác suất gian lận P(fraud) bằng công thức Sigmoid:
    // 1. Kiểm tra trạng thái: nếu !ready hoặc features rỗng, trả về xác suất an toàn mặc định (vd: 0.15)
    // 2. Tính tổng tổ hợp tuyến tính z = bias + sum(w_i * x_i)
    // 3. Áp dụng hàm Sigmoid: P = 1.0 / (1.0 + exp(-z))
    // 4. Giới hạn xác suất trong đoạn [0.0, 1.0] và trả về P
    //
    // Gợi ý:
    if (!ready || features.empty()) {
        return 0.15;
    }
    double z = bias;
    int n = (features.size() < weights.size()) ? features.size() : weights.size();
    for (int i = 0; i < n; ++i) {
        z += weights[i] * features[i];
    }
    double prob = 1.0 / (1.0 + std::exp(-z));
    if (prob < 0.0) prob = 0.0;
    if (prob > 1.0) prob = 1.0;
    return prob;


}

double NativeMLPredictor::predictScore(const Vector<double>& features) const {
    // TODO 5: Quy đổi xác suất gian lận [0.0, 1.0] sang thang điểm rủi ro [0.0, 100.0]
    // Gợi ý:
    return predictProbability(features) * 100.0;


}

bool NativeMLPredictor::isReady() const {
    // TODO 6: Kiểm tra mô hình đã sẵn sàng suy luận hay chưa
    // Gợi ý:
    return ready;


}

void NativeMLPredictor::setReady(bool state) {
    // TODO 7: Cập nhật cờ trạng thái sẵn sàng của mô hình
    // Gợi ý:
    this->ready = state;


}

const std::string& NativeMLPredictor::getModelName() const {
    // TODO 8: Trả về tên định danh mô hình
    // Gợi ý:
    return model_name;


}

const std::string& NativeMLPredictor::getModelVersion() const {
    // TODO 9: Trả về phiên bản mô hình
    // Gợi ý:
    return model_version;


}
