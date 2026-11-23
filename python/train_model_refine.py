"""
============================================================================
[BƯỚC 6.1] SCRIPT HUẤN LUYỆN MÔ HÌNH HỌC MÁY & XUẤT FILE MODEL_WEIGHTS.JSON
Tài liệu hướng dẫn: include/engine/06_STEP6_ML_MODEL_JSON.md
============================================================================

Mục tiêu:
- Huấn luyện mô hình Logistic Regression từ tập dữ liệu data/transactions.csv
- Trích xuất trọng số (weights) và hệ số chặn (bias)
- Xuất kết quả ra file data/model_weights.json để C++ NativeMLPredictor nạp vào
"""

import json
import os
import pandas as pd
import numpy as np
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
from sklearn.metrics import classification_report, roc_auc_score
# Đường dẫn file dữ liệu và file xuất
DATA_PATH = "data/transactions.csv"
OUTPUT_PATH = "data/model_weights.json"
def main():
    print("=" * 60)
    print("  QUY TRINH HUAN LUYEN MO HINH HOC MAY & XUAT JSON")
    print("=" * 60)
    # TODO 1: Đọc dữ liệu từ file CSV bằng pandas
    # Gợi ý:
    if not os.path.exists(DATA_PATH):
        print(f"Loi: Khong tim thay file {DATA_PATH}")
        return
    df = pd.read_csv(DATA_PATH)
    print(f"-> Da doc {len(df)} dong du lieu tu {DATA_PATH}")
    # TODO 2: Chuẩn bị danh sách đặc trưng X và nhãn gian lận y
    # - Các đặc trưng số học: 'amount', 'velocity_count', 'geo_distance_km', 'is_device_risk'
    # - Nhãn mục tiêu: 'is_fraud'
    # (Lưu ý: Nếu tập dữ liệu mô phỏng chưa có nhãn vi phạm 1, ta tự gán cờ vi phạm dựa trên luật để huấn luyện)
    # Gợi ý:
    feature_cols = ['amount', 'velocity_count', 'geo_distance_km', 'is_device_risk']
    if 'is_fraud' not in df.columns or df['is_fraud'].nunique() < 2:
        print("-> Canh bao: Tap du lieu chua co nhan gian lan (is_fraud=1), khoi tao nhan mo phong...")
        # Gan nhan gian lan mo phong dua tren cac bat thuong ro ret
        df['is_fraud'] = ((df['amount'] > 2000.0) | (df['velocity_count'] > 5) | (df['is_device_risk'] == 1)).astype(int)
    X = df[feature_cols]
    y = df['is_fraud']
    print(f"-> Phan bo nhan: {dict(y.value_counts())}")
    # TODO 3: Huấn luyện mô hình Logistic Regression cân bằng trọng số (class_weight='balanced')
    # Gợi ý:
    X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42, stratify=y)
    model = LogisticRegression(class_weight='balanced', max_iter=1000)
    model.fit(X_train, y_train)
    y_pred_prob = model.predict_proba(X_test)[:, 1]
    print(f"-> ROC-AUC danh gia tren tap Test: {roc_auc_score(y_test, y_pred_prob):.4f}")
    # TODO 4: Trích xuất các trọng số mô hình (Weights và Bias)
    # Gợi ý:
    weights = [round(float(w), 4) for w in model.coef_[0]]
    bias = round(float(model.intercept_[0]), 4)
    print(f"-> Trong so weights [w1, w2, w3, w4]: {weights}")
    print(f"-> He so chan bias (w0): {bias}")
    # TODO 5: Đóng gói và ghi ra file JSON định dạng chuẩn
    # Gợi ý:
    export_data = {
        "description": "Calibrated Logistic Regression Weights for EPFD-RAS",
        "features": feature_cols,
        "weights": weights,
        "bias": bias,
        "optimal_threshold": 0.5
    }
    with open(OUTPUT_PATH, 'w', encoding='utf-8') as f:
        json.dump(export_data, f, indent=4)
    print(f"[THANH CONG] Da xuat file trong so ra: {OUTPUT_PATH}")


if __name__ == "__main__":
    main()
