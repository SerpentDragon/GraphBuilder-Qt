#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QListWidget>
#include <QStandardItemModel>

QStandardItemModel *model = nullptr;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);

    QStandardItem *item = new QStandardItem("Text");
    item->setCheckable(true);
    item->setCheckState(Qt::Checked);
    item->setEditable(false);
    item->setSizeHint(QSize(item->sizeHint().rwidth(), 30));

    model->appendRow(item);

    ui->listView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;

    for(size_t i = 0; i < model->rowCount(); i++)
    {
        delete model->item(i);
    }
    delete model;
}
