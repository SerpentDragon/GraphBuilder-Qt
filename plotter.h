#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QLayout>
#include <QPainter>
#include "settings.h"
#include <QMouseEvent>

namespace pl = WidgetParams::Plotter;

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
            graphLineWidth_ = 2.5;

            segmentSize_ = segmentSize;

            mainAxisColor_ = Qt::black;
            auxiliaryAxisColor_ = QColor::fromRgb(180, 180, 180);

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
            if (segmentSize_ < pl::maxZoom)
                segmentSize_ *= pl::zoomCoefficient;
        }
        else
        {
            if (segmentSize_ > pl::minZoom)
                segmentSize_ /= pl::zoomCoefficient;
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
        painter.setPen(QPen{ auxiliaryAxisColor_, auxiliaryAxisWidth_});

        for(int x = center_.x - segmentSize_; x > 0; x -= segmentSize_)
        {
            painter.drawLine(x, 0, x, height_);
        }

        for(int x = center_.x + segmentSize_; x < width_; x += segmentSize_)
        {
            painter.drawLine(x, 0, x, height_);
        }

        for(int y = center_.y - segmentSize_; y > 0; y -= segmentSize_)
        {
            painter.drawLine(0, y, width_, y);
        }

        for(int y = center_.y + segmentSize_; y < width_; y += segmentSize_)
        {
            painter.drawLine(0, y, width_, y);
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
    double graphLineWidth_;

    double segmentSize_;

    QColor mainAxisColor_;
    QColor auxiliaryAxisColor_;
};

#endif // PLOTTER_H
