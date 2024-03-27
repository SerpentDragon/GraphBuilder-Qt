#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QLayout>
#include <QPainter>
#include <QMouseEvent>

// Point mouseStart_, mouseEnd_;

// double diffminSegmentSize_;

// double maxZoom_;
// double minZoom_;
// // double zoomCoefficient_ = 2;

// QColor mainAxisColor_;
// QColor auxiliaryAxisColor_;

class Plotter : public QWidget
{
public:
    Plotter(QWidget* parent = nullptr, double segmentSize = 0) : QWidget(parent)
    {
        if (parent != nullptr)
        {
            width_ = parent->size().width();
            height_ = parent->size().height();

            center_.x = width_ / 2;
            center_.y = height_ / 2;

            mainAxisWidth_ = 1.5;
            auxiliaryAxisWidth_ = 0.5;
            additionalAxisWidth_ = 0.25;
            graphLineWidth_ = 2.5;

            segmentSize_ = segmentSize;
            minSegmentSize_ = segmentSize;
            maxSegmentSize_ = 4 * minSegmentSize_;
            diffminSegmentSize_ = 3 * minSegmentSize_ / 30;

            maxZoom_ = 1920;
            minZoom_ = 2;
            zoomCoefficient_ = 2;
            numbersFactor_ = 1;

            mainAxisColor_ = Qt::black;
            auxiliaryAxisColor_ = QColor::fromRgb(100, 100, 100);
            additionalAxisColor_ = QColor::fromRgb(180, 180, 180);

            setGeometry(0, 0, parent->size().width(), parent->size().height());
        }
    }

protected:
    void paintEvent(QPaintEvent* event) override
    {
        QWidget::paintEvent(event);

        QPainter painter(this);
        painter.fillRect(rect(), Qt::white);

        paintMainAxes(painter);
        paintAuxiliaryAxes(painter);
        paintAdditionalAxes(painter);
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            mouseStart_.x = event->pos().x();
            mouseStart_.y = event->pos().y();
        }

        QWidget::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (event->buttons() & Qt::LeftButton)
        {
            mouseEnd_.x = event->pos().x();
            mouseEnd_.y = event->pos().y();

            int dx = mouseEnd_.x - mouseStart_.x;
            int dy = mouseEnd_.y - mouseStart_.y;

            center_.x += dx;
            center_.y += dy;

            mouseStart_ = mouseEnd_;

            update();
        }

        QWidget::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            mouseStart_.x = mouseStart_.y = 0;
        }

        QWidget::mouseReleaseEvent(event);
    }

    void wheelEvent(QWheelEvent* event) override
    {
        int angle = event->angleDelta().y();

        if (angle > 0)
        {
            segmentSize_ += diffminSegmentSize_;
            if (segmentSize_ > maxSegmentSize_)
            {
                segmentSize_ = minSegmentSize_;
                numbersFactor_ /= zoomCoefficient_;
            }
        }
        else
        {
            segmentSize_ -= diffminSegmentSize_;
            if (segmentSize_ < minSegmentSize_)
            {
                segmentSize_ = maxSegmentSize_;
                numbersFactor_ *= zoomCoefficient_;
            }
        }

        QWidget::wheelEvent(event);

        update();
    }

private:
    void paintMainAxes(QPainter& painter)
    {
        painter.setPen(QPen{ mainAxisColor_, mainAxisWidth_ });

        painter.drawLine(0, center_.y, width_, center_.y);
        painter.drawLine(center_.x, 0, center_.x, height_);
    }

    void paintAuxiliaryAxes(QPainter& painter)
    {
        painter.setPen(QPen{ auxiliaryAxisColor_, auxiliaryAxisWidth_ });
        painter.setBrush(QBrush{ Qt::black });

        int counter = 0;
        for(int x = center_.x - segmentSize_; x > 0; x -= segmentSize_)
        {
            painter.drawLine(x, 0, x, height_);
            painter.drawText(x + 2, center_.y + 15, QString::number(--counter * numbersFactor_));
        }

        counter = 0;
        for(int x = center_.x + segmentSize_; x < width_; x += segmentSize_)
        {
            painter.drawLine(x, 0, x, height_);
            painter.drawText(x - 2, center_.y + 15, QString::number(++counter * numbersFactor_));
        }

        counter = 0;
        for(int y = center_.y - segmentSize_; y > 0; y -= segmentSize_)
        {
            painter.drawLine(0, y, width_, y);
            painter.drawText(center_.x, y - 2, QString::number(++counter * numbersFactor_));
        }

        counter = 0;
        for(int y = center_.y + segmentSize_; y < width_; y += segmentSize_)
        {
            painter.drawLine(0, y, width_, y);
            painter.drawText(center_.x, y - 2, QString::number(++counter * numbersFactor_));
        }
    }

    void paintAdditionalAxes(QPainter& painter)
    {
        painter.setPen(QPen{ additionalAxisColor_, additionalAxisWidth_ });

        double step = segmentSize_ / 5;
        double x, y;

        x = center_.x - step;
        while (x > 0)
        {
            painter.drawLine(x, 0, x, height_);
            x -= step;
        }

        x = center_.x + step;
        while (x < width_)
        {
            painter.drawLine(x, 0, x, height_);
            x += step;
        }

        y = center_.y - step;
        while (y > 0)
        {
            painter.drawLine(0, y, width_, y);
            y -= step;
        }

        y = center_.y + step;
        while (y < height_)
        {
            painter.drawLine(0, y, width_, y);
            y += step;
        }
    }

private:

    struct Point
    {
        int x;
        int y;
    } center_, mouseStart_, mouseEnd_;

    int width_;
    int height_;

    double mainAxisWidth_;
    double auxiliaryAxisWidth_;
    double additionalAxisWidth_;
    double graphLineWidth_;

    double segmentSize_;
    double minSegmentSize_;
    double maxSegmentSize_;
    double diffminSegmentSize_;

    double maxZoom_;
    double minZoom_;
    double zoomCoefficient_;
    double numbersFactor_;

    QColor mainAxisColor_;
    QColor auxiliaryAxisColor_;
    QColor additionalAxisColor_;
};

#endif // PLOTTER_H
