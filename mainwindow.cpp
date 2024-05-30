#include "mainwindow.h"
#include "./ui_mainwindow.h"

Plotter* plotter = nullptr;
ParseTree parser;
const QString constants("1234567890)eπx");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    createButtonGroup(ui->digitGroupBox, &MainWindow::setDigit);
    createButtonGroup(ui->operationGroupBox, &MainWindow::setOperation);
    createButtonGroup(ui->functionGroupBox, &MainWindow::setFunction);
    createButtonGroup(ui->constGroupBox, &MainWindow::setConst);

    ui->radianRadioButton->setChecked(true);

    QStandardItemModel* model = new QStandardItemModel(this);

    QStandardItem *item = new QStandardItem("Text");
    item->setCheckable(true);
    item->setCheckState(Qt::Checked);
    item->setEditable(false);
    item->setSizeHint(QSize(item->sizeHint().rwidth(), 30));

    model->appendRow(item);

    ui->listView->setModel(model);

    plotter = new Plotter(ui->widget, 60);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete plotter;
}


void MainWindow::createButtonGroup(const QGroupBox* bg, void(MainWindow::*func)(const QString&))
{
    QList<QAbstractButton*> buttons = bg->findChildren<QAbstractButton*>();

    for (QAbstractButton* button : buttons)
    {
        connect(button, &QPushButton::clicked, this,
                [this, button, func]() { (this->*func)(button->text()); });
    }
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
        if (constants.contains(lastSymbol) ||
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
            if (constants.contains(lastSymbol))
                ui->expressionLabel->setText(expr + func);
        }
        else if (QString("+-*/%^(,").contains(lastSymbol))
        {
           ui->expressionLabel->setText(expr + func + "(");
        }
        else if (constants.contains(lastSymbol))
        {
            ui->expressionLabel->setText(expr + "*" + func + "(");
        }
    }
}


void MainWindow::setConst(const QString& c)
{
    QString expr = ui->expressionLabel->text();
    if (expr.size() == 0) ui->expressionLabel->setText(c);
    else
    {
        QChar lastSymbol = expr[expr.size() - 1];
        if (QString("+-*/%^(,").contains(lastSymbol))
        {
            ui->expressionLabel->setText(expr + c);
        }
        else if (constants.contains(lastSymbol))
        {
            ui->expressionLabel->setText(expr + "*" + c);
        }
    }
}


void MainWindow::on_eraseButton_clicked()
{
    ui->expressionLabel->clear();
}


void MainWindow::on_deleteButton_clicked()
{
    QString expr = ui->expressionLabel->text();

    if (expr.size() == 0) return;

    QChar lastSymbol = expr[expr.size() - 1];
    if (lastSymbol == '(')
    {
        if (expr.size() == 1) ui->expressionLabel->clear();
        else
        {
            int i = expr.size() - 2;
            while (i >= 0 && (expr[i].isLetter() || expr[i] == QChar(0x221A))) i--;
            ui->expressionLabel->setText(expr.mid(0, ++i));
        }
    }
    else
    {
        ui->expressionLabel->setText(expr.mid(0, expr.size() - 1));
    }
}


void MainWindow::on_pointButton_clicked()
{
    QString expr = ui->expressionLabel->text();

    if (expr.size() != 0)
    {
        QChar lastSymbol = expr[expr.size() - 1];
        if (lastSymbol.isDigit()) ui->expressionLabel->setText(expr + ".");
    }
}


void MainWindow::on_commaButton_clicked()
{
    QString expr = ui->expressionLabel->text();

    if (expr.size() != 0)
    {
        int pos = expr.lastIndexOf("log(");

        if (pos != -1)
        {
            pos += 3;
            int bracketCounter = 0;
            bool commaCounter = false;

            do
            {
                if (expr[pos] == '(') bracketCounter++;
                else if (expr[pos] == ')') bracketCounter--;
                else if (expr[pos] == ',')
                {
                    commaCounter = true;
                    break;
                }
                pos++;
            }
            while (pos < expr.size() && bracketCounter > 0);

            if (bracketCounter > 0 &&
                constants.contains(expr[expr.size() - 1])
                && !commaCounter)
            {
                ui->expressionLabel->setText(expr + ",");
            }
        }
    }
}


void MainWindow::on_openBracketButton_clicked()
{
    QString expr = ui->expressionLabel->text();

    if (expr.size() == 0) ui->expressionLabel->setText("(");
    else
    {
        QChar lastSymbol = expr[expr.size() - 1];
        if (QString("+-*/%^(,").contains(lastSymbol))
        {
            ui->expressionLabel->setText(expr + "(");
        }
        else if (constants.contains(lastSymbol))
        {
            ui->expressionLabel->setText(expr + "*(");
        }
    }
}


void MainWindow::on_closeBracketButton_clicked()
{
    QString expr = ui->expressionLabel->text();

    if (expr.size() != 0)
    {
        QChar lastSymbol = expr[expr.size() - 1];
        if (constants.contains(lastSymbol))
        {
            ui->expressionLabel->setText(expr + ")");
        }
    }
}


void MainWindow::on_xButton_clicked()
{
    QString expr = ui->expressionLabel->text();

    if (expr.size() == 0) ui->expressionLabel->setText(expr + "x");
    else
    {
        QChar lastSymbol = expr[expr.size() - 1];
        if (constants.contains(lastSymbol))
        {
            ui->expressionLabel->setText(expr + "*x");
        }
        else setConst("x");
    }
}


void MainWindow::on_modButton_clicked()
{
    QString expr = ui->expressionLabel->text();

    if (expr.size() != 0)
    {
        QChar lastSymbol = expr[expr.size() - 1];
        if (constants.contains(lastSymbol))
        {
            ui->expressionLabel->setText(expr + "%");
        }
    }
}


void MainWindow::on_equalsButton_clicked()
{
    std::string expression = ui->expressionLabel->text().toStdString();
    parser.setExpression(expression);

    // calculation logic
}
