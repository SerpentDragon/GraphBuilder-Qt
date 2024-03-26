#include "plotter.h"

Plotter::Plotter(QWidget* parent) : QWidget(parent)
{
    if (parent != nullptr)
        setGeometry(0, 0, parent->size().width(), parent->size().height());

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(1, 1, 1, 1); // устанавливаем отступы для рамки
    layout->addWidget(new QWidget(this));
}

void Plotter::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.fillRect(rect(), Qt::lightGray);
}
