#ifndef PLOTTER_H
#define PLOTTER_H

#include <thread>
#include <QWidget>
#include <QLayout>
#include <QPainter>
#include <QMouseEvent>
#include "../Model/graph.h"

class Plotter : public QWidget
{
public:
    explicit Plotter(QWidget* = nullptr, double = 0);

protected:
    void paintEvent(QPaintEvent*) override;

    void paintGraphics(QPainter&) const;

    void mousePressEvent(QMouseEvent*) override;

    void mouseMoveEvent(QMouseEvent*) override;

    void mouseReleaseEvent(QMouseEvent*) override;

    void wheelEvent(QWheelEvent*) override;

private:
    void paintMainAxes(QPainter&) const;

    void paintAuxiliaryAxes(QPainter&) const;

    void paintAdditionalAxes(QPainter&) const;

    void paintCoordinateLines(QPainter&, double) const;

    void paintCoordinateText(QPainter&, int, double, bool) const;

    double calculateX(const QRect&) const;

    double calculateY(const QRect&) const;

    QPointF fromCartesianToWindow(const QPointF&) const;

    bool isInsideField(const QPointF&) const;

    double getLeftLimit() const;

    double getRightLimit() const;

    double getTopLimit() const;

    double getBottomLimit() const;

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
    double fontSize_;

    double segmentSize_;
    double minSegmentSize_;
    double maxSegmentSize_;
    double diffSegmentSize_;

    double zoomCoefficient_;
    double coordinatesFactor_;

    QColor mainAxisColor_;
    QColor auxiliaryAxisColor_;
    QColor additionalAxisColor_;
};

#endif // PLOTTER_H
