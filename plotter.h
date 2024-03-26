#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QLayout>
#include <QPainter>

class Plotter : public QWidget
{
public:
    Plotter(QWidget* parent = nullptr, int segmentSize = 0) : QWidget(parent)
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
        QWidget::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        QWidget::mouseReleaseEvent(event);
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
    } center_;

    int width_;
    int height_;

    double mainAxisWidth_;
    double auxiliaryAxisWidth_;
    double graphLineWidth_;

    int segmentSize_;

    QColor mainAxisColor_;
    QColor auxiliaryAxisColor_;
};

#endif // PLOTTER_H
