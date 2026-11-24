from abc import ABC, abstractmethod
import json
import os
import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import roc_auc_score, average_precision_score, precision_score, recall_score, f1_score, precision_recall_curve

class DataLoader:
    def __init__(self, path="data/transactions.csv"):
        self._path = path
        self._features = ["amount", "velocity_count", "geo_distance_km", "is_device_risk"]

    def load_and_split(self):
        df = pd.read_csv(self._path)
        if "is_fraud" not in df.columns or df["is_fraud"].sum() < 30:
            df["is_fraud"] = ((df["amount"] > 2000.0) | (df["velocity_count"] > 5) | (df["is_device_risk"] == 1)).astype(int)
        X, y = df[self._features], df["is_fraud"]
        X_tmp, X_test, y_tmp, y_test = train_test_split(X, y, test_size=0.2, random_state=42, stratify=y)
        X_train, X_val, y_train, y_val = train_test_split(X_tmp, y_tmp, test_size=0.25, random_state=42, stratify=y_tmp)
        return self._features, (X_train, y_train), (X_val, y_val), (X_test, y_test)

class BaseFraudModel(ABC):
    def __init__(self, name):
        self._name = name

    @property
    def name(self):
        return self._name

    @abstractmethod
    def fit(self, X, y):
        pass

    @abstractmethod
    def predict_proba(self, X):
        pass

    def predict(self, X, threshold=0.5):
        return (self.predict_proba(X)[:, 1] >= threshold).astype(int)

class LogisticRegressionFraudModel(BaseFraudModel):
    def __init__(self):
        super().__init__("LogisticRegression")
        self._pipe = Pipeline([
            ("scaler", StandardScaler()),
            ("clf", LogisticRegression(class_weight="balanced", max_iter=1000, random_state=42))
        ])

    def fit(self, X, y):
        self._pipe.fit(X, y)
        return self

    def predict_proba(self, X):
        return self._pipe.predict_proba(X)

    def get_unscaled_params(self):
        scaler, clf = self._pipe.named_steps["scaler"], self._pipe.named_steps["clf"]
        weights = [round(float(w / s), 4) for w, s in zip(clf.coef_[0], scaler.scale_)]
        bias = round(float(clf.intercept_[0] - np.sum(clf.coef_[0] * scaler.mean_ / scaler.scale_)), 4)
        return weights, bias

class RandomForestFraudModel(BaseFraudModel):
    def __init__(self):
        super().__init__("RandomForest")
        self._clf = RandomForestClassifier(n_estimators=100, class_weight="balanced_subsample", min_samples_leaf=3, random_state=42)

    def fit(self, X, y):
        self._clf.fit(X, y)
        return self

    def predict_proba(self, X):
        return self._clf.predict_proba(X)

class FraudModelEvaluator:
    @staticmethod
    def evaluate(model, X, y, threshold=0.5):
        prob = model.predict_proba(X)[:, 1]
        pred = (prob >= threshold).astype(int)
        return {
            "model": model.name,
            "roc_auc": round(float(roc_auc_score(y, prob)), 4),
            "pr_auc": round(float(average_precision_score(y, prob)), 4),
            "precision": round(float(precision_score(y, pred, zero_division=0)), 4),
            "recall": round(float(recall_score(y, pred)), 4),
            "f1": round(float(f1_score(y, pred)), 4)
        }

    @staticmethod
    def get_optimal_threshold(model, X_val, y_val, target_recall=0.80):
        prob = model.predict_proba(X_val)[:, 1]
        prec, rec, thr = precision_recall_curve(y_val, prob)
        mask = rec[:-1] >= target_recall
        return round(float(thr[mask][np.argmax(prec[:-1][mask])]), 4) if mask.any() else 0.5

class ModelExporter:
    @staticmethod
    def export_json(path, features, weights, bias, threshold):
        data = {
            "description": "Calibrated Logistic Regression Weights for EPFD-RAS (Unscaled)",
            "features": features,
            "weights": weights,
            "bias": bias,
            "optimal_threshold": threshold
        }
        with open(path, "w", encoding="utf-8") as f:
            json.dump(data, f, indent=4)
        print(f"[EXPORT] Da xuat trong so C++ ra: {path}")

def main():
    loader = DataLoader()
    features, (X_train, y_train), (X_val, y_val), (X_test, y_test) = loader.load_and_split()
    print(f"[DATA] Train: {len(X_train)}, Val: {len(X_val)}, Test: {len(X_test)} (Fraud: {y_train.sum()})")

    models = [LogisticRegressionFraudModel(), RandomForestFraudModel()]
    evaluator = FraudModelEvaluator()

    print("\n[VALIDATION COMPARISON]")
    for m in models:
        m.fit(X_train, y_train)
        print(evaluator.evaluate(m, X_val, y_val))

    print("\n[TEST EVALUATION WITH OPTIMAL THRESHOLD]")
    opt_thresholds = {}
    for m in models:
        thr = evaluator.get_optimal_threshold(m, X_val, y_val)
        opt_thresholds[m.name] = thr
        res = evaluator.evaluate(m, X_test, y_test, threshold=thr)
        print(f"Model {m.name} (Thr={thr}): {res}")

    lr_model = models[0]
    weights, bias = lr_model.get_unscaled_params()
    ModelExporter.export_json("data/model_weights.json", features, weights, bias, opt_thresholds[lr_model.name])

if __name__ == "__main__":
    main()
