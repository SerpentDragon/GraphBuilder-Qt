#include "plotter.h"

Plotter::Plotter(QWidget *parent, double segmentSize) : QWidget(parent)
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
        functionLineWidth_ = 3.0;

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

void Plotter::paintEvent(QPaintEvent *event)
{
    // QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    paintMainAxes(painter);
    paintAuxiliaryAxes(painter);
    paintAdditionalAxes(painter);
    paintGraphics(painter);
}

void Plotter::paintGraphics(QPainter& painter) const
{
    auto result = graph.calculateFunctions(getLeftLimit(), getRightLimit());

    for(const auto& function : result)
    {
        QColor color = function.first;
        auto points = function.second;

        painter.setPen(QPen{ color, functionLineWidth_ });

        for(int i = 0; i < points.size() - 1; i++)
        {
            double startX = center_.x + points[i].x() * segmentSize_ / coordinatesFactor_;
            double startY = center_.y - points[i].y() * segmentSize_ / coordinatesFactor_;

            double endX = center_.x + points[i + 1].x() * segmentSize_ / coordinatesFactor_;
            double endY = center_.y - points[i + 1].y() * segmentSize_ / coordinatesFactor_;

            painter.drawLine(startX, startY, endX, endY);
        }
    }
}

void Plotter::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        mouseStart_.x = event->pos().x();
        mouseStart_.y = event->pos().y();
    }

    QWidget::mousePressEvent(event);
}

void Plotter::mouseMoveEvent(QMouseEvent* event)
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

void Plotter::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        mouseStart_.x = mouseStart_.y = 0;
    }

    QWidget::mouseReleaseEvent(event);
}

void Plotter::wheelEvent(QWheelEvent* event)
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

            dx -= dx / maxSegmentSize_ * segmentSize_ * zoomCoefficient_;
            dy -= dy / maxSegmentSize_ * segmentSize_ * zoomCoefficient_;
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

            dx -= dx / minSegmentSize_ * segmentSize_ / zoomCoefficient_;
            dy -= dy / minSegmentSize_ * segmentSize_ / zoomCoefficient_;
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

void Plotter::paintMainAxes(QPainter& painter) const
{
    painter.setPen(QPen{ mainAxisColor_, mainAxisWidth_ });

    painter.drawLine(0, center_.y, width_, center_.y);
    painter.drawLine(center_.x, 0, center_.x, height_);
}

void Plotter::paintAuxiliaryAxes(QPainter& painter) const
{
    painter.setPen(QPen{ auxiliaryAxisColor_, auxiliaryAxisWidth_ });

    paintCoordinateLines(painter, segmentSize_);
}

void Plotter::paintAdditionalAxes(QPainter& painter) const
{
    painter.setPen(QPen{ additionalAxisColor_, additionalAxisWidth_ });

    paintCoordinateLines(painter, segmentSize_ / 5);
}

void Plotter::paintCoordinateLines(QPainter& painter, double step) const
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
            paintCoordinateText(painter, counter, x, true);
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
            paintCoordinateText(painter, counter, x, true);
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
            paintCoordinateText(painter, counter, y, false);
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
            paintCoordinateText(painter, counter, y, false);
        }
        y += step;
    }
}

void Plotter::paintCoordinateText(QPainter& painter, int counter, double coord, bool horizontal) const
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

double Plotter::calculateX(const QRect& boundingRect) const
{
    if (center_.x > width_) return width_ - boundingRect.width() - 2;
    else if (center_.x < 0) return 0;
    else return center_.x + 2;
}

double Plotter::calculateY(const QRect& boundingRect) const
{
    if (center_.y > height_) return height_;
    else if (center_.y < 0) return boundingRect.height();
    else return center_.y - 2;
}

double Plotter::getLeftLimit() const
{
    return (-center_.x / segmentSize_) * coordinatesFactor_;
}

double Plotter::getRightLimit() const
{
    return ((width_ - center_.x) / segmentSize_) * coordinatesFactor_;
}
