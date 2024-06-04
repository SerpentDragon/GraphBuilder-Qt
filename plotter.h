#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QLayout>
#include "graph.h"
#include <QPainter>
#include <QMouseEvent>

extern Graph graph;

class Plotter : public QWidget
{
public:
    Plotter(QWidget* = nullptr, double = 0);

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

    double getLeftLimit() const;

    double getRightLimit() const;

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
    double functionLineWidth_;

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
