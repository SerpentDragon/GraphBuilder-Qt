#include "plotter.h"

Plotter::Plotter(QWidget* parent, double segmentSize) : QWidget(parent)
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
        graphLineWidth_ = 2.8;
        fontSize_ = 12.0;

        zoomCoefficient_ = 2.0;
        coordinatesFactor_ = 1.0;

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
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);

    paintMainAxes(painter);
    paintAuxiliaryAxes(painter);
    paintAdditionalAxes(painter);
    paintGraphics(painter);
}

void Plotter::paintGraphics(QPainter& painter) const
{
    auto result = Graph::getGraph()->calculateFunctions(getLeftLimit(), getRightLimit(), coordinatesFactor_);

    for(const auto& function : result)
    {
        int index = function.first;
        QColor color = function.second.first;
        auto points = function.second.second;

        painter.setPen(QPen{ color, graphLineWidth_ });

        for(int i = 0; i < points.size() - 1; i++)
        {
            if (qIsNaN(points[i].y()) || qIsNaN(points[i + 1].y()) ||
                qIsInf(points[i].y()) || qIsInf(points[i + 1].y())) continue;

            if (!isInsideField(points[i]) && !isInsideField(points[i + 1])) continue;

            QPointF start = fromCartesianToWindow(points[i]);
            QPointF end = fromCartesianToWindow(points[i + 1]);

            if (std::abs(end.y() - start.y()) > segmentSize_ / 30)
            {
                auto gt = Graph::getGraph();
                auto brokenInterval = Graph::getGraph()->calculateBrokenInterval(index, points[i].x(), points[i + 1].x());

                for(int i = 0; i < brokenInterval.size(); i += 2)
                {
                    start = fromCartesianToWindow(brokenInterval[i]);
                    end = fromCartesianToWindow(brokenInterval[i + 1]);

                    painter.drawLine(start, end);
                }
            }
            else painter.drawLine(start, end);
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
    static QFont font = painter.font();

    QString number = QString::number(counter * coordinatesFactor_);
    QRect boundingRect = painter.fontMetrics().boundingRect(number);

    if (boundingRect.width() >= minSegmentSize_ / 2)
    {
        font.setPointSizeF(2 * fontSize_ / 3);
        boundingRect = painter.fontMetrics().boundingRect(number);
    }
    else font.setPointSizeF(fontSize_);

    painter.setFont(font);

    QPen prevPen = painter.pen();
    painter.setPen(Qt::black);

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

QPointF Plotter::fromCartesianToWindow(const QPointF& point) const
{
    return {
            center_.x + point.x() * segmentSize_ / coordinatesFactor_,
            center_.y - point.y() * segmentSize_ / coordinatesFactor_
    };
}

bool Plotter::isInsideField(const QPointF& point) const
{
    return getLeftLimit() < point.x() && point.x() < getRightLimit()
           && getBottomLimit() < point.y() && point.y() < getTopLimit();
}

double Plotter::getLeftLimit() const
{
    return (-center_.x / segmentSize_) * coordinatesFactor_;
}

double Plotter::getRightLimit() const
{
    return ((width_ - center_.x) / segmentSize_) * coordinatesFactor_;
}

double Plotter::getTopLimit() const
{
    return (center_.y / segmentSize_) * coordinatesFactor_ + 1;
}

double Plotter::getBottomLimit() const
{
    return ((center_.y - height_) / segmentSize_) * coordinatesFactor_ - 1;
}
