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
public:

    explicit ListViewItem(const QString&, QWidget* = nullptr);

private:

    void setupCheckBox();

    void setupTextEdit();

    void setupButton();

    QHBoxLayout* setupLayout();

private:

    QWidget* parent_;

    int width_;
    int height_;

    QCheckBox* checkBox_;
    QTextEdit* textEdit_;
    QPushButton* button_;
};

#endif // LISTVIEWITEM_H
