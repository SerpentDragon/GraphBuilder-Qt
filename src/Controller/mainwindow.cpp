#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    srand(time(nullptr));

    ui->setupUi(this);

    aboutDeveloper_ = new AboutDeveloper(this);

    createButtonGroup(ui->digitGroupBox, &MainWindow::setDigit);
    createButtonGroup(ui->operationGroupBox, &MainWindow::setOperation);
    createButtonGroup(ui->functionGroupBox, &MainWindow::setFunction);
    createButtonGroup(ui->constGroupBox, &MainWindow::setConst);

    ui->radiansRadioButton->setChecked(true);
    
    plotter_ = new Plotter(ui->widget, WidgetParams::Plotter::SegmentSize);

    itemModel_ = new QStandardItemModel(this);
    ui->listView->setModel(itemModel_);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addItemToListView(const QString& function)
{
    QStandardItem* item = new QStandardItem();
    item->setSizeHint({ ui->listView->size().width(), WidgetParams::ListViewItem::Height });
    itemModel_->appendRow(item);

    ListViewItem* listViewItem = new ListViewItem(function, ui->listView);
    connect(listViewItem, &ListViewItem::checkBoxStateChanged, this, &MainWindow::displayOrHideFunction);
    connect(listViewItem, &ListViewItem::buttonClicked, this, &MainWindow::removeFunction);

    ui->listView->setIndexWidget(item->index(), listViewItem);
}


void MainWindow::displayOrHideFunction(ListViewItem* item, int state)
{
    auto result = getListViewItemRow(item);
    if (!result.has_value()) return;

    int row = result.value();

    if (state == Qt::Checked)
    {
        Graph::getGraph()->displayFunction(row);
    }
    else if (state == Qt::Unchecked)
    {
        Graph::getGraph()->hideFunction(row);
    }

    plotter_->repaint();
}


void MainWindow::removeFunction(ListViewItem* item)
{
    auto result = getListViewItemRow(item);
    if (!result.has_value()) return;

    int row = result.value();

    Graph::getGraph()->removeFunction(row);

    itemModel_->removeRow(row);

    plotter_->repaint();
}


std::optional<int> MainWindow::getListViewItemRow(ListViewItem* item) const
{
    for(int row = 0; row < itemModel_->rowCount(); row++)
    {
        QModelIndex index = itemModel_->index(row, 0);

        if (ui->listView->indexWidget(index) == item)
        {
            return row;
        }
    }

    return std::nullopt;
}


void MainWindow::createButtonGroup(const QGroupBox* bg, void (MainWindow::*func)(const QString&))
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
        if (AllowedSymbols::digitAllowedSymbols.contains(expr.back()))
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
        if (AllowedSymbols::constants.contains(expr.back()) ||
            (expr.back() == '(' && op == '-'))
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
        QChar lastSymbol = expr.back();
        if (func == "^")
        {
            if (AllowedSymbols::constants.contains(lastSymbol))
                ui->expressionLabel->setText(expr + func);
        }
        else if (AllowedSymbols::operators.contains(lastSymbol))
        {
           ui->expressionLabel->setText(expr + func + "(");
        }
        else if (AllowedSymbols::constants.contains(lastSymbol))
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
        if (AllowedSymbols::operators.contains(expr.back()))
        {
            ui->expressionLabel->setText(expr + c);
        }
        else if (AllowedSymbols::constants.contains(expr.back()))
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

    if (expr.back() == '(')
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
        if (expr.back().isDigit()) ui->expressionLabel->setText(expr + ".");
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
                AllowedSymbols::constants.contains(expr.back())
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
        if (AllowedSymbols::operators.contains(expr.back()))
        {
            ui->expressionLabel->setText(expr + "(");
        }
        else if (AllowedSymbols::constants.contains(expr.back()))
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
        if (AllowedSymbols::constants.contains(expr.back()))
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
        if (AllowedSymbols::constants.contains(expr.back()))
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
        if (AllowedSymbols::constants.contains(expr.back()))
        {
            ui->expressionLabel->setText(expr + "%");
        }
    }
}


void MainWindow::on_equalsButton_clicked()
{
    QString expression = ui->expressionLabel->text();

    if (expression.size() == 0) return;

    std::stack<QChar> stk;
    for(const QChar c : expression)
    {
        if (c == '(') stk.push(c);
        else if (c == ')')
        {
            if (stk.empty())
            {
                QMessageBox::critical(this, "Error", "The nesting of parentheses is broken");
                return;
            }

            stk.pop();
        }
    }

    if (!stk.empty())
    {
        QMessageBox::critical(this, "Error", "The nesting of parentheses is broken");
        return;
    }

    if (AllowedSymbols::operators.contains(expression.back()))
    {
        QMessageBox::critical(this, "Error", "Your expression is incorrect!");
        return;
    }

    ui->expressionLabel->clear();

    addItemToListView(expression);

    Graph::getGraph()->addFunction(expression.toStdString(),
                                   ui->radiansRadioButton->isChecked() ? ANGLE::RADIANS : ANGLE::DEGREES);

    plotter_->repaint();
}


void MainWindow::closeEvent(QCloseEvent* event)
{
    auto reply = QMessageBox::question(this, "Exit confirmation",
                                       "Are you shure you want to close the app?",
                                       QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) this->close();
    else event->ignore();
}


void MainWindow::on_actionAbout_Developer_triggered()
{
    QMessageBox::information(this, "About Developer", "This programm was developed by SerpentDragon (dmitryshelyganov@gmail.com)", QMessageBox::Ok);
}


void MainWindow::on_actionAbout_Programm_triggered()
{
    aboutDeveloper_->exec();
}
