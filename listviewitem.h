#ifndef LISTVIEWITEM_H
#define LISTVIEWITEM_H

#include <QKeyEvent>
#include <QCheckBox>
#include <QTextEdit>
#include "settings.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QStandardItem>

namespace LVI = WidgetParams::ListViewItem;

class ListViewItem : public QWidget
{
    Q_OBJECT

public:

    explicit ListViewItem(const QString&, QModelIndex, QWidget* = nullptr);

    QModelIndex getRowIndex() const;

signals:

    void checkBoxStateChanged(QModelIndex, int);

    void buttonClicked(QModelIndex);

private slots:

    void onCheckBoxStatedChanged(int);

    void onButtonClicked();

private:

    void setupCheckBox();

    void setupTextEdit();

    void setupButton();

    QHBoxLayout* setupLayout();

private:

    QWidget* parent_;

    int width_;
    int height_;

    QModelIndex rowIndex_;

    QCheckBox* checkBox_;
    QTextEdit* textEdit_;
    QPushButton* button_;
};

#endif // LISTVIEWITEM_H
