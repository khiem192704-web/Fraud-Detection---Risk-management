"""
============================================================================
[BƯỚC 6.1] SCRIPT HUẤN LUYỆN, ĐÁNH GIÁ MÔ HÌNH HỌC MÁY & XUẤT MODEL_WEIGHTS.JSON
Tài liệu hướng dẫn: include/engine/06_STEP6_ML_MODEL_JSON.md
============================================================================

Mục tiêu & Quy trình chuẩn cho bài toán Fraud Detection (Dữ liệu mất cân bằng):
1. Đọc dữ liệu giao dịch và tạo nhãn mô phỏng nếu cần.
2. Chia 3 tập theo tỉ lệ 60 / 20 / 20 có Stratify:
   - Train (60%): Học tham số mô hình.
   - Validation (20%): So sánh mô hình & tối ưu ngưỡng phân loại (threshold).
   - Test (20%): Đánh giá khách quan độc lập ở bước cuối cùng (chỉ chạy 1 lần).
3. Đánh giá đa chiều với các chỉ số chuyên dụng cho Fraud: ROC-AUC, PR-AUC, Precision, Recall, F1, F2 (ưu tiên Recall).
4. Huấn luyện và so sánh đa mô hình (Logistic Regression + StandardScaler, Random Forest, HistGradientBoosting).
5. Kiểm định độ ổn định bằng 5-Fold Stratified Cross-Validation trên tập (Train + Val).
6. Tinh chỉnh ngưỡng phân loại (Threshold Tuning) trên tập Validation (Recall >= 80%).
7. Đánh giá kiểm thử cuối cùng trên tập Test.
8. Trích xuất trọng số Unscaled từ mô hình Logistic Regression và lưu ra file data/model_weights.json cho C++ NativeMLPredictor.
"""

import json
import os

# Tắt cảnh báo loky trên Windows khi WMIC bị tắt / gỡ bỏ
os.environ["LOKY_MAX_CPU_COUNT"] = str(os.cpu_count() or 4)

import numpy as np
import pandas as pd

# Cell 1: Import các thư viện phân loại, pipeline và đánh giá
from sklearn.model_selection import train_test_split, StratifiedKFold, cross_validate
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression
from sklearn.ensemble import RandomForestClassifier, HistGradientBoostingClassifier
from sklearn.metrics import (
    roc_auc_score, average_precision_score, precision_score, recall_score,
    f1_score, fbeta_score, confusion_matrix, precision_recall_curve,
    classification_report
)

DATA_PATH = "data/transactions.csv"
OUTPUT_PATH = "data/model_weights.json"


# Cell 3: Hàm đánh giá đa chỉ số cho bài toán mất cân bằng dữ liệu
def evaluate(model, X_, y_, threshold=0.5, label=""):
    """
    Đánh giá mô hình tại một ngưỡng xác suất xác định (threshold).
    Đối với Fraud Detection, PR-AUC, Recall, Precision, F1, F2 có ý nghĩa thực tế
    cao hơn rất nhiều so với Accuracy thuần túy.
    """
    prob = model.predict_proba(X_)[:, 1]
    pred = (prob >= threshold).astype(int)
    tn, fp, fn, tp = confusion_matrix(y_, pred).ravel()
    res = {
        "set": label,
        "roc_auc": roc_auc_score(y_, prob),
        "pr_auc": average_precision_score(y_, prob),
        "precision": precision_score(y_, pred, zero_division=0),
        "recall": recall_score(y_, pred),
        "f1": f1_score(y_, pred),
        "f2": fbeta_score(y_, pred, beta=2),  # beta=2: ưu tiên Recall (giảm thiểu bỏ sót gian lận FN)
        "TP": tp, "FP": fp, "FN": fn, "TN": tn,
    }
    return res


def main():
    print("=" * 70)
    print("  QUY TRINH HUAN LUYEN & DANH GIA MO HINH FRAUD DETECTION CHUYEN SAU")
    print("=" * 70)

    # -------------------------------------------------------------------------
    # TODO 1: Đọc dữ liệu từ file CSV
    # -------------------------------------------------------------------------
    if not os.path.exists(DATA_PATH):
        print(f"Loi: Khong tim thay file {DATA_PATH}")
        return
    df = pd.read_csv(DATA_PATH)
    print(f"-> Da doc {len(df)} dong du lieu tu {DATA_PATH}")

    # -------------------------------------------------------------------------
    # TODO 2: Chuẩn bị đặc trưng X và nhãn mục tiêu y
    # -------------------------------------------------------------------------
    feature_cols = ['amount', 'velocity_count', 'geo_distance_km', 'is_device_risk']
    if 'is_fraud' not in df.columns or df['is_fraud'].nunique() < 2:
        print("-> Canh bao: Tap du lieu chua co nhan gian lan (is_fraud=1), khoi tao nhan mo phong...")
        df['is_fraud'] = ((df['amount'] > 2000.0) | (df['velocity_count'] > 5) | (df['is_device_risk'] == 1)).astype(int)

    X = df[feature_cols]
    y = df['is_fraud']
    print(f"-> Phan bo nhan tong the: {dict(y.value_counts())}")

    # -------------------------------------------------------------------------
    # Cell 2: Chia dữ liệu 60 / 20 / 20 có Stratify
    # -------------------------------------------------------------------------
    # Bước 1: Tách 20% tập Test riêng biệt
    X_temp, X_test, y_temp, y_test = train_test_split(
        X, y, test_size=0.2, random_state=42, stratify=y
    )
    # Bước 2: Tách tập còn lại (80%) thành Train (60%) và Validation (20%) -> test_size = 0.25 (0.25 * 0.8 = 0.2)
    X_train, X_val, y_train, y_val = train_test_split(
        X_temp, y_temp, test_size=0.25, random_state=42, stratify=y_temp
    )

    print("\n--- PHAN BO DU LIEU CAC TAP (TRAIN / VAL / TEST) ---")
    for name, part in [("Train (60%)", y_train), ("Val   (20%)", y_val), ("Test  (20%)", y_test)]:
        print(f"  {name:12s}: {len(part):5d} mau | Fraud: {part.sum():3d} ({part.mean():.2%})")

    # -------------------------------------------------------------------------
    # Cell 4: Huấn luyện & So sánh nhiều mô hình trên tập Validation
    # -------------------------------------------------------------------------
    print("\n--- DANG HUAN LUYEN CAC MO HINH TREN TAP TRAIN ---")
    models = {
        "logreg": Pipeline([
            ("scaler", StandardScaler()),
            ("clf", LogisticRegression(class_weight="balanced", max_iter=1000, random_state=42))
        ]),
        "random_forest": RandomForestClassifier(
            n_estimators=300, class_weight="balanced_subsample",
            min_samples_leaf=3, n_jobs=-1, random_state=42
        ),
        "hist_gb": HistGradientBoostingClassifier(
            class_weight="balanced", random_state=42
        ),
    }

    rows = []
    for name, m in models.items():
        m.fit(X_train, y_train)
        r = evaluate(m, X_val, y_val, threshold=0.5, label=name)
        rows.append(r)

    val_results = pd.DataFrame(rows).set_index("set").round(4)
    print("\n--- KET QUA SO SANH TREN TAP VALIDATION (Default Threshold = 0.5) ---")
    print(val_results.to_string())

    # -------------------------------------------------------------------------
    # Cell 5: 5-Fold Stratified Cross-Validation trên X_temp (Train + Val)
    # -------------------------------------------------------------------------
    print("\n--- 5-FOLD STRATIFIED CROSS-VALIDATION (TREN TAP TRAIN + VAL) ---")
    cv = StratifiedKFold(n_splits=5, shuffle=True, random_state=42)
    scoring = ["roc_auc", "average_precision", "precision", "recall", "f1"]

    for name, m in models.items():
        cvres = cross_validate(m, X_temp, y_temp, cv=cv, scoring=scoring, n_jobs=-1)
        print(f"Model: {name}")
        for s in scoring:
            v = cvres[f"test_{s}"]
            print(f"  {s:18s}: {v.mean():.4f} ± {v.std():.4f}")

    # -------------------------------------------------------------------------
    # Cell 6: Tối ưu ngưỡng phân loại (Threshold Tuning) trên tập Validation
    # -------------------------------------------------------------------------
    best_name = "hist_gb"  # Mô hình mạnh nhất trên bài toán dạng bảng
    model = models[best_name]

    val_prob = model.predict_proba(X_val)[:, 1]
    prec, rec, thr = precision_recall_curve(y_val, val_prob)

    # Mục tiêu: chọn ngưỡng để Recall >= 0.80 với Precision cao nhất
    mask = rec[:-1] >= 0.80
    threshold = thr[mask][np.argmax(prec[:-1][mask])] if mask.any() else 0.5

    print(f"\n--- TOI UU NGUONG PHAN LOAI (THRESHOLD TUNING) TREN VALIDATION ---")
    print(f"-> Mo hinh su dung: {best_name}")
    print(f"-> Nguong toi uu chon duoc (Recall >= 80% va Precision cao nhat): {threshold:.4f}")

    # -------------------------------------------------------------------------
    # Cell 7: Đánh giá cuối cùng trên tập Test (Chỉ chạy một lần duy nhất)
    # -------------------------------------------------------------------------
    print("\n--- DANH GIA CUOI CUNG TREN TAP TEST VOI NGUONG TOI UU ---")
    final = evaluate(model, X_test, y_test, threshold=threshold, label="test")
    final_series = pd.Series({k: (round(v, 4) if isinstance(v, (float, np.floating)) else v) for k, v in final.items()})
    print(final_series.to_string())

    print("\n--- CHI TIET CLASSIFICATION REPORT (TAP TEST) ---")
    test_preds = (model.predict_proba(X_test)[:, 1] >= threshold).astype(int)
    print(classification_report(y_test, test_preds, target_names=["normal", "fraud"]))

    # -------------------------------------------------------------------------
    # TODO ĐẶC BIỆT: Trích xuất trọng số Unscaled cho C++ NativeMLPredictor
    # -------------------------------------------------------------------------
    # Vì LogisticRegression dùng StandardScaler trong Pipeline, ta quy đổi ngược về raw features:
    # w_raw = w_scaled / sigma
    # bias_raw = bias_scaled - sum(w_scaled * mu / sigma)
    # Giúp C++ NativeMLPredictor tính toán trực tiếp mà không cần thêm code StandardScaler!
    print("\n--- TRICH XUAT TRONG SO CHO C++ NATIVEMLPREDICTOR ---")
    scaler = models["logreg"].named_steps["scaler"]
    clf = models["logreg"].named_steps["clf"]

    raw_weights = [round(float(w / s), 4) for w, s in zip(clf.coef_[0], scaler.scale_)]
    raw_bias = round(float(clf.intercept_[0] - np.sum(clf.coef_[0] * scaler.mean_ / scaler.scale_)), 4)

    # Tối ưu ngưỡng cho Logistic Regression trên validation để xuất kèm C++
    lr_val_prob = models["logreg"].predict_proba(X_val)[:, 1]
    p_lr, r_lr, t_lr = precision_recall_curve(y_val, lr_val_prob)
    mask_lr = r_lr[:-1] >= 0.80
    lr_threshold = round(float(t_lr[mask_lr][np.argmax(p_lr[:-1][mask_lr])]) if mask_lr.any() else 0.5, 4)

    print(f"-> Weights unscaled [w1, w2, w3, w4]: {raw_weights}")
    print(f"-> Bias unscaled (w0): {raw_bias}")
    print(f"-> Nguong toi uu cho LogReg: {lr_threshold}")

    export_data = {
        "description": "Calibrated Logistic Regression Weights for EPFD-RAS (Unscaled)",
        "features": feature_cols,
        "weights": raw_weights,
        "bias": raw_bias,
        "optimal_threshold": lr_threshold
    }

    with open(OUTPUT_PATH, 'w', encoding='utf-8') as f:
        json.dump(export_data, f, indent=4)
    print(f"[THANH CONG] Da xuat file trong so ra: {OUTPUT_PATH}")


if __name__ == "__main__":
    main()
