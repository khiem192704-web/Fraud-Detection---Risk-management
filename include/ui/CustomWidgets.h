#pragma once

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <vector>
#include <QString>
#include "ui/I18n.h"

/**
 * @brief Widget vẽ biểu đồ đường tia (Sparkline) nhỏ gọn bên trong các thẻ KPI
 */
class SparklineWidget : public QWidget {
private:
    std::vector<double> data_;
    QColor lineColor_;
    QColor fillColor_;

public:
    explicit SparklineWidget(const std::vector<double>& data, QColor color, QWidget* parent = nullptr)
        : QWidget(parent), data_(data), lineColor_(color) {
        fillColor_ = lineColor_;
        fillColor_.setAlpha(35);
        setFixedHeight(38);
        setMinimumWidth(80);
    }

    void setData(const std::vector<double>& data) {
        data_ = data;
        update();
    }

protected:
    void paintEvent(QPaintEvent* /*event*/) override {
        if (data_.size() < 2) return;
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::TextAntialiasing);

        double minV = data_[0], maxV = data_[0];
        for (double v : data_) {
            if (v < minV) minV = v;
            if (v > maxV) maxV = v;
        }
        if (maxV == minV) maxV += 1.0;

        int w = width() - 8;
        int h = height() - 8;
        int pad = 4;

        QPainterPath path;
        QPainterPath areaPath;

        for (size_t i = 0; i < data_.size(); ++i) {
            double x = pad + (static_cast<double>(i) / (data_.size() - 1)) * w;
            double norm = (data_[i] - minV) / (maxV - minV);
            double y = pad + (1.0 - norm) * h;

            if (i == 0) {
                path.moveTo(x, y);
                areaPath.moveTo(x, height());
                areaPath.lineTo(x, y);
            } else {
                path.lineTo(x, y);
                areaPath.lineTo(x, y);
            }
        }
        areaPath.lineTo(pad + w, height());
        areaPath.closeSubpath();

        // Vẽ mảng bóng mờ dưới đường
        p.fillPath(areaPath, fillColor_);

        // Vẽ đường viền
        QPen pen(lineColor_, 2.0);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        p.setPen(pen);
        p.drawPath(path);
    }
};

/**
 * @brief Widget vẽ biểu đồ đường "Giao dịch theo thời gian" với 2 đường hợp lệ & nghi vấn
 */
class TrendChartWidget : public QWidget {
private:
    std::vector<QString> dates_{"17/06", "18/06", "19/06", "20/06", "21/06", "22/06", "23/06", "24/06"};
    std::vector<double> validSeries_{95, 105, 130, 125, 140, 135, 155, 195};
    std::vector<double> fraudSeries_{10, 12, 18, 15, 20, 25, 22, 38};

public:
    explicit TrendChartWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumHeight(175);
    }

    void setSeries(const std::vector<double>& valid, const std::vector<double>& fraud) {
        validSeries_ = valid;
        fraudSeries_ = fraud;
        update();
    }

protected:
    void paintEvent(QPaintEvent* /*event*/) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::TextAntialiasing);

        int leftMargin = 32;
        int bottomMargin = 26;
        int topMargin = 12;
        int rightMargin = 16;

        int plotW = width() - leftMargin - rightMargin;
        int plotH = height() - topMargin - bottomMargin;

        double maxY = 250.0;
        int yTicks = 5; // 0, 50, 100, 150, 200, 250

        // 1. Vẽ các đường lưới ngang và nhãn trục Y
        QFont font("Segoe UI", 9);
        p.setFont(font);

        for (int i = 0; i <= yTicks; ++i) {
            double val = (maxY / yTicks) * i;
            int y = topMargin + plotH - static_cast<int>((val / maxY) * plotH);

            p.setPen(QPen(QColor("#1E2D5A"), 1, Qt::DashLine));
            p.drawLine(leftMargin, y, leftMargin + plotW, y);

            p.setPen(QColor("#64748B"));
            p.drawText(0, y - 6, leftMargin - 6, 14, Qt::AlignRight | Qt::AlignVCenter, QString::number(static_cast<int>(val)));
        }

        // 2. Vẽ nhãn trục X
        int nPoints = static_cast<int>(dates_.size());
        for (int i = 0; i < nPoints; ++i) {
            int x = leftMargin + static_cast<int>((static_cast<double>(i) / (nPoints - 1)) * plotW);
            p.setPen(QColor("#64748B"));
            p.drawText(x - 22, height() - bottomMargin + 4, 44, 18, Qt::AlignCenter, dates_[i]);
        }

        auto drawCurve = [&](const std::vector<double>& series, QColor color, bool fillGradient) {
            if (series.size() < 2) return;
            QPainterPath path;
            QPainterPath fillPath;

            for (size_t i = 0; i < series.size(); ++i) {
                int x = leftMargin + static_cast<int>((static_cast<double>(i) / (series.size() - 1)) * plotW);
                double norm = series[i] / maxY;
                if (norm > 1.0) norm = 1.0;
                int y = topMargin + plotH - static_cast<int>(norm * plotH);

                if (i == 0) {
                    path.moveTo(x, y);
                    fillPath.moveTo(x, topMargin + plotH);
                    fillPath.lineTo(x, y);
                } else {
                    path.lineTo(x, y);
                    fillPath.lineTo(x, y);
                }
            }
            int lastX = leftMargin + plotW;
            fillPath.lineTo(lastX, topMargin + plotH);
            fillPath.closeSubpath();

            if (fillGradient) {
                QLinearGradient grad(0, topMargin, 0, topMargin + plotH);
                QColor cStart = color;
                cStart.setAlpha(60);
                QColor cEnd = color;
                cEnd.setAlpha(0);
                grad.setColorAt(0.0, cStart);
                grad.setColorAt(1.0, cEnd);
                p.fillPath(fillPath, grad);
            }

            // Vẽ đường chính
            p.setPen(QPen(color, 2.4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            p.drawPath(path);

            // Vẽ các chấm tròn trên điểm dữ liệu
            for (size_t i = 0; i < series.size(); ++i) {
                int x = leftMargin + static_cast<int>((static_cast<double>(i) / (series.size() - 1)) * plotW);
                double norm = series[i] / maxY;
                int y = topMargin + plotH - static_cast<int>(norm * plotH);

                p.setBrush(color);
                p.setPen(QPen(QColor("#0B132B"), 1.8));
                p.drawEllipse(QPoint(x, y), 3, 3);
            }
        };

        // Vẽ chuỗi Giao dịch hợp lệ (Xanh dương)
        drawCurve(validSeries_, QColor("#0284C7"), true);

        // Vẽ chuỗi Giao dịch đáng ngờ (Đỏ)
        drawCurve(fraudSeries_, QColor("#EF4444"), true);
    }
};

/**
 * @brief Widget vẽ biểu đồ tròn khuyết (Donut Chart) "Phân bố quyết định"
 */
class DonutChartWidget : public QWidget {
private:
    int approved_{4612};
    int review_{276};
    int blocked_{112};

public:
    explicit DonutChartWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumSize(140, 140);
    }

    void setValues(int approved, int review, int blocked) {
        approved_ = approved;
        review_ = review;
        blocked_ = blocked;
        update();
    }

protected:
    void paintEvent(QPaintEvent* /*event*/) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setRenderHint(QPainter::TextAntialiasing);

        int total = approved_ + review_ + blocked_;
        if (total == 0) total = 1;

        int side = qMin(width(), height()) - 10;
        int x = (width() - side) / 2;
        int y = (height() - side) / 2;
        QRectF outerRect(x, y, side, side);

        double angleApprove = (static_cast<double>(approved_) / total) * 360.0 * 16.0;
        double angleReview = (static_cast<double>(review_) / total) * 360.0 * 16.0;
        double angleBlock = 360.0 * 16.0 - angleApprove - angleReview;

        double startAngle = 90.0 * 16.0;

        p.setPen(Qt::NoPen);

        // Cung APPROVE (Xanh lá)
        p.setBrush(QColor("#10B981"));
        p.drawPie(outerRect, static_cast<int>(startAngle), static_cast<int>(-angleApprove));

        // Cung REVIEW (Vàng cam)
        p.setBrush(QColor("#F59E0B"));
        p.drawPie(outerRect, static_cast<int>(startAngle - angleApprove), static_cast<int>(-angleReview));

        // Cung BLOCK (Đỏ)
        p.setBrush(QColor("#EF4444"));
        p.drawPie(outerRect, static_cast<int>(startAngle - angleApprove - angleReview), static_cast<int>(-angleBlock));

        // Vẽ lỗ rỗng ở giữa (Inner Circle tạo hình Donut)
        double innerRatio = 0.68;
        int innerSide = static_cast<int>(side * innerRatio);
        int inX = (width() - innerSide) / 2;
        int inY = (height() - innerSide) / 2;
        QRectF innerRect(inX, inY, innerSide, innerSide);

        p.setBrush(QColor("#111C38")); // Khớp với màu nền thẻ
        p.drawEllipse(innerRect);

        // Chữ ở tâm vòng tròn
        QFont fontVal("Segoe UI", 14, QFont::Bold);
        p.setFont(fontVal);
        p.setPen(QColor("#FFFFFF"));
        QRectF textValRect(inX, inY + innerSide * 0.20, innerSide, 26);
        p.drawText(textValRect, Qt::AlignCenter, QString::number(total));

        QFont fontSub("Segoe UI", 9, QFont::DemiBold);
        p.setFont(fontSub);
        p.setPen(QColor("#94A3B8"));
        QRectF textSubRect(inX, inY + innerSide * 0.48, innerSide, 18);
        p.drawText(textSubRect, Qt::AlignCenter, I18n::instance().t(QString::fromUtf8("Tổng giao dịch"), "Total Txns"));
        QFont fontDemo("Segoe UI", 8);
        p.setFont(fontDemo);
        QRectF textDemoRect(inX, inY + innerSide * 0.67, innerSide, 14);
        p.drawText(textDemoRect, Qt::AlignCenter, "(demo)");
    }
};
