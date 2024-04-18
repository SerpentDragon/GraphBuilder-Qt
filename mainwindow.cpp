#include <QLayout>
#include "plotter.h"
#include <QListWidget>
#include "mainwindow.h"
#include <QButtonGroup>
#include "./ui_mainwindow.h"
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createDigitButtonGroup();
    createOperationButtonGroup();
    createFunctionButtonGroup();

    ui->radianRadioButton->setChecked(true);

    QStandardItemModel* model = new QStandardItemModel(this);

    QStandardItem *item = new QStandardItem("Text");
    item->setCheckable(true);
    item->setCheckState(Qt::Checked);
    item->setEditable(false);
    item->setSizeHint(QSize(item->sizeHint().rwidth(), 30));

    model->appendRow(item);

    ui->listView->setModel(model);

    Plotter* plotter = new Plotter(ui->widget, 60);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createDigitButtonGroup()
{
    QButtonGroup* digitGroup = new QButtonGroup(this);

    QList<QPushButton*> buttons = this->findChildren<QPushButton*>();
    for (auto button : buttons)
    {
        if (button != nullptr)
        {
            bool res = false;
            QString(button->text()).toInt(&res);
            if (res)
            {
                digitGroup->addButton(button);
            }
        }
    }

    connect(digitGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, [this](QAbstractButton *button) { setDigit(button->text()); });
}

void MainWindow::createOperationButtonGroup()
{
    QButtonGroup* operationGroup = new QButtonGroup(this);

    operationGroup->addButton(ui->sumButton);
    operationGroup->addButton(ui->subtractButton);
    operationGroup->addButton(ui->mulButton);
    operationGroup->addButton(ui->divButton);

    connect(operationGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, [this](QAbstractButton *button) { setOperation(button->text()); });
}

void MainWindow::createFunctionButtonGroup()
{
    QButtonGroup* functionGroup = new QButtonGroup(this);

    // logic here

    connect(functionGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
           this, [this](QAbstractButton *button) { setFunction(button->text()); });
}

void MainWindow::setDigit(const QString& digit)
{
    QString expr = ui->expressionLabel->text();

    if (expr.size() == 0) ui->expressionLabel->setText(expr + digit);
    else
    {
        QChar lastSymbol = expr[expr.size() - 1];
        if (QString("+-*/%(^.,1234567890").contains(lastSymbol))
        {
            ui->expressionLabel->setText(expr + digit);
        }
    }
}

void MainWindow::setOperation(const QString& op)
{
    QString expr = ui->expressionLabel->text();
    if (expr.size() != 0)
    {
        QChar lastSymbol = expr[expr.size() - 1];
        if (QString("1234567890)xeπ").contains(lastSymbol) ||
            (lastSymbol == '(' && op == '-'))
        {
            ui->expressionLabel->setText(expr + op);
        }
    }
    else if (op == "-") ui->expressionLabel->setText("-");
}

void MainWindow::setFunction(const QString& func)
{
    QString expr = ui->expressionLabel->text();
    if (expr.size() == 0)
    {
        if (func != "^") ui->expressionLabel->setText(expr + func + "(");
    }
    else
    {
        QChar lastSymbol = expr[expr.size() - 1];
        if (func == "^")
        {
            if (QString("1234567890exπ)").contains(lastSymbol))
                ui->expressionLabel->setText(expr + func);
        }
        else
        {
            if (QString("+-*/%^(,").contains(lastSymbol))
                ui->expressionLabel->setText(expr + func + "(");
        }
    }
}
