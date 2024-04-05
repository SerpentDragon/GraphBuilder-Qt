#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QLayout>
#include <QPainter>
#include <QMouseEvent>

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

            maxZoom_ = 1920;
            minZoom_ = 2;
            zoomCoefficient_ = 2;
            coordinatesFactor_ = 1;

            segmentSize_ = segmentSize;
            minSegmentSize_ = segmentSize;
            maxSegmentSize_ = 2 * zoomCoefficient_ * minSegmentSize_;
            diffSegmentSize_ = minSegmentSize_ * 0.1;

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

        QPointF cursorPos = event->position();

        double dx = cursorPos.x() - center_.x;
        double dy = cursorPos.y() - center_.y;

        if (angle > 0)
        {
            segmentSize_ += diffSegmentSize_;
            if (segmentSize_ > maxSegmentSize_)
            {
                segmentSize_ = minSegmentSize_;
                coordinatesFactor_ /= zoomCoefficient_;

                dx /= (zoomCoefficient_);
                dy /= (zoomCoefficient_);
            }
            else
            {
                dx -= dx / (segmentSize_ - diffSegmentSize_) * segmentSize_;
                dy -= dy / (segmentSize_ - diffSegmentSize_) * segmentSize_;
            }
        }
        else
        {
            segmentSize_ -= diffSegmentSize_;
            if (segmentSize_ < minSegmentSize_)
            {
                segmentSize_ = maxSegmentSize_;
                coordinatesFactor_ *= zoomCoefficient_;

                dx *= (1 - zoomCoefficient_);
                dy *= (1 - zoomCoefficient_);
            }
            else
            {
                dx -= dx / (segmentSize_ + diffSegmentSize_) * segmentSize_;
                dy -= dy / (segmentSize_ + diffSegmentSize_) * segmentSize_;
            }
        }

        center_.x += dx;
        center_.y += dy;

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

        paintCoordinateLines(painter, segmentSize_);
    }

    void paintAdditionalAxes(QPainter& painter)
    {
        painter.setPen(QPen{ additionalAxisColor_, additionalAxisWidth_ });

        paintCoordinateLines(painter, segmentSize_ / 5);
    }

    void paintCoordinateLines(QPainter& painter, double step)
    {
        int counter;
        double x, y;
        double penWidth = painter.pen().widthF();

        counter = 0;
        x = center_.x - step;
        while (x > 0)
        {
            painter.drawLine(x, 0, x, height_);
            if (penWidth == auxiliaryAxisWidth_)
            {
                counter--;
                drawCoordinateText(painter, counter, x, true);
            }

            x -= step;
        }

        counter = 0;
        x = center_.x + step;
        while (x < width_)
        {
            painter.drawLine(x, 0, x, height_);
            if (penWidth == auxiliaryAxisWidth_)
            {
                counter++;
                drawCoordinateText(painter, counter, x, true);
            }
            x += step;
        }

        counter = 0;
        y = center_.y - step;
        while (y > 0)
        {
            painter.drawLine(0, y, width_, y);
            if (penWidth == auxiliaryAxisWidth_)
            {
                counter++;
                drawCoordinateText(painter, counter, y, false);
            }
            y -= step;
        }

        counter = 0;
        y = center_.y + step;
        while (y < height_)
        {
            painter.drawLine(0, y, width_, y);
            if (penWidth == auxiliaryAxisWidth_)
            {
                counter--;
                drawCoordinateText(painter, counter, y, false);
            }
            y += step;
        }
    }

    void drawCoordinateText(QPainter& painter, int counter, double coord, bool horizontal)
    {
        QPen prevPen = painter.pen();
        painter.setPen(Qt::black);

        double value = counter * coordinatesFactor_;
        QString number = QString::number(value);
        QRect boundingRect = painter.fontMetrics().boundingRect(number);

        if (boundingRect.width() > 3 * minSegmentSize_ / 4)
        {
            // number = QString::number(value, 'g', 3);
            number = QString::number(value);
            boundingRect = painter.fontMetrics().boundingRect(number);
        }

        horizontal ?
            painter.drawText(coord - boundingRect.width() / 2, calculateY(boundingRect), number)
            : painter.drawText(calculateX(boundingRect), coord + boundingRect.height() / 3, number);

        painter.setPen(prevPen);
    }

    double calculateX(const QRect& boundingRect)
    {
        if (center_.x > width_) return width_ - boundingRect.width() - 2;
        else if (center_.x < 0) return 0;
        else return center_.x + 2;
    }

    double calculateY(const QRect& boundingRect)
    {
        if (center_.y > height_) return height_;
        else if (center_.y < 0) return boundingRect.height();
        else return center_.y - 2;
    }

private:

    struct Point
    {
        double x;
        double y;
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
    double diffSegmentSize_;

    double maxZoom_;
    double minZoom_;
    double zoomCoefficient_;
    double coordinatesFactor_;

    QColor mainAxisColor_;
    QColor auxiliaryAxisColor_;
    QColor additionalAxisColor_;
};

#endif // PLOTTER_H
