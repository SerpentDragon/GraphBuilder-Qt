#include "aboutdeveloper.h"
#include "ui_aboutdeveloper.h"

AboutDeveloper::AboutDeveloper(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDeveloper)
{
    ui->setupUi(this);
}

AboutDeveloper::~AboutDeveloper()
{
    delete ui;
}
