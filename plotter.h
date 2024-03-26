#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QLayout>
#include <QPainter>

class Plotter : public QWidget
{
public:
    Plotter(QWidget* = nullptr);

protected:
    void paintEvent(QPaintEvent*) override;
};

#endif // PLOTTER_H
