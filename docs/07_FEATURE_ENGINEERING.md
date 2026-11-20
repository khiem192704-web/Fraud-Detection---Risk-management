# 07 — Feature Engineering & Feature Extraction Module

> 📘 **Tài liệu hướng dẫn thực hành chi tiết theo lộ trình**:  
> Vui lòng tham khảo file: [include/engine/07_STEP7_FEATURE_EXTRACTION.md](file:///d:/Fraud-Detection---Risk-management/include/engine/07_STEP7_FEATURE_EXTRACTION.md)

## C++ FeatureExtractor candidates
```text
transaction_amount
transaction_frequency
transactions_last_5min
transactions_last_1hour
average_transaction_amount
amount_deviation
new_device
new_country
distance_from_previous_transaction
failed_attempt_count
merchant_frequency
customer_risk_history
```

Each feature must document:
- business meaning
- data type
- calculation
- source
- whether it supports rules, ML, or both

Avoid meaningless feature proliferation.

## 4 Core Features for Online Machine Learning Inference
Mô hình `NativeMLPredictor` và script `python/train_model.py` sử dụng chính xác 4 đặc trưng:
1. `amount`: Số tiền giao dịch (`tx.GetAmount()`).
2. `velocity_count`: Tần suất quẹt thẻ gần đây (`Sliding Window Deque`).
3. `geo_distance_km`: Khoảng cách từ điểm quẹt thẻ đến nhà khách hàng (`tx.GetLocation().distanceTo(customer.getHomeLocation())`).
4. `is_device_risk`: Cờ cảnh báo thiết bị nghi vấn/máy ảo/root (`dev.GetIs_Virtual() || dev.GetIs_Hacker() || !dev.is_Safe()`).

## Important ML rule
Features must be generated without target leakage and without using future information relative to the transaction timestamp.
