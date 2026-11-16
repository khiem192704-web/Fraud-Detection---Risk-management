#pragma once
#include <string>
#include "DSA/vector.h"
class NativeMLPredictor {
private:
    // TODO 1: Khai báo thông tin phiên bản mô hình mặc định và trạng thái sẵn sàng
    std::string model_name{"Native_Logistic_Predictor"};
    std::string model_version{"1.0.0"};
    bool ready{true};
    // TODO 2: Khai báo hệ số tự do / độ lệch cơ sở (bias w0, mặc định -2.8)
    double bias{-2.8};
    // TODO 3: Mảng động lưu trữ các trọng số đặc trưng [w1, w2, w3,...] (dùng template Vector tự viết)
    Vector<double>weights;
    // TODO 4: Danh sách tên các đặc trưng (amount, velocity, device_risk,...)
    Vector<std::string>feature_names;
public:
    // TODO 5: Các hàm khởi tạo (Constructor mặc định và Constructor nạp trước trọng số)
    NativeMLPredictor();
    NativeMLPredictor(double bias,const Vector<double>&weights);
    // TODO 6: Hàm nạp file trọng số JSON xuất ra từ script huấn luyện Python
    bool loadModel(const std::string&filepath);
    // TODO 7: Hàm tính xác suất gian lận P(fraud) nằm trong đoạn [0.0, 1.0] bằng hàm Sigmoid
    double predictProbability(const Vector<double>&features) const;
    // TODO 8: Hàm quy đổi xác suất sang thang điểm rủi ro [0.0, 100.0]
    double predictScore(const Vector<double>& features) const;
    // TODO 9: Các phương thức kiểm tra trạng thái và thông tin mô hình
    bool isReady() const;
    void setReady(bool state);
    const std::string& getModelName() const;
    const std::string& getModelVersion() const;
};
