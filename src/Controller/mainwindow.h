#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stack>
#include <QLayout>
#include <optional>
#include <QGroupBox>
#include "settings.h"
#include <QListWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <QButtonGroup>
#include "listviewitem.h"
#include "../View/plotter.h"
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    void addItemToListView(const QString&);

    void displayOrHideFunction(ListViewItem*, int);

    void removeFunction(ListViewItem*);

    std::optional<int> getListViewItemRow(ListViewItem*) const;

    void createButtonGroup(const QGroupBox*, void(MainWindow::*)(const QString&));

    void createDigitButtonGroup();

    void createOperationButtonGroup();

    void createFunctionButtonGroup();

    void setDigit(const QString&);

    void setOperation(const QString&);

    void setFunction(const QString&);

    void setConst(const QString&);

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_eraseButton_clicked();

    void on_deleteButton_clicked();

    void on_pointButton_clicked();

    void on_commaButton_clicked();

    void on_openBracketButton_clicked();

    void on_closeBracketButton_clicked();

    void on_xButton_clicked();

    void on_modButton_clicked();

    void on_equalsButton_clicked();

private:
    Ui::MainWindow* ui;

    Plotter* plotter_;

    QStandardItemModel* itemModel_;
};
#endif // MAINWINDOW_H
