#ifndef ABOUTDEVELOPER_H
#define ABOUTDEVELOPER_H

#include <QDialog>

namespace Ui {
class AboutDeveloper;
}

class AboutDeveloper : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDeveloper(QWidget *parent = nullptr);
    ~AboutDeveloper();

private:
    Ui::AboutDeveloper *ui;
};

#endif // ABOUTDEVELOPER_H
