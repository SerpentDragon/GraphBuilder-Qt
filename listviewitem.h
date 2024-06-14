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

    explicit ListViewItem(const QString& text, QWidget* parent = nullptr) : QWidget(parent),
        parent_(parent), width_(parent->size().width()), height_(LVI::Height)
    {
        checkBox_ = new QCheckBox(this);
        textEdit_ = new QTextEdit(text, this);
        button_ = new QPushButton(this);

        setupCheckBox();
        setupTextEdit();
        setupButton();

        this->setLayout(setupLayout());
        this->setMinimumSize(width_, height_);
    }

private:

    void setupCheckBox()
    {
        checkBox_->setCheckState(Qt::Checked);
        checkBox_->setFixedSize(LVI::CheckBoxSize, LVI::CheckBoxSize);
    }

    void setupTextEdit()
    {
        textEdit_->setReadOnly(true);
        textEdit_->setWordWrapMode(QTextOption::NoWrap);
        textEdit_->setTextInteractionFlags(Qt::NoTextInteraction);
        textEdit_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        textEdit_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        textEdit_->setFixedWidth(width_ - LVI::CheckBoxSize -
                                 LVI::ButtonSize - 4 * LVI::MarginValue);
    }

    void setupButton()
    {
        button_->setFixedSize(LVI::ButtonSize, LVI::ButtonSize);
    }

    QHBoxLayout* setupLayout()
    {
        QHBoxLayout* layout = new QHBoxLayout(this);

        int vertialMargin = (LVI::Height - textEdit_->geometry().height()) / 2;

        layout->setSpacing(LVI::MarginValue);
        layout->setContentsMargins(LVI::MarginValue, vertialMargin, LVI::MarginValue, vertialMargin);

        layout->addWidget(checkBox_);
        layout->addWidget(textEdit_);
        layout->addWidget(button_);

        return layout;
    }

private:

    QWidget* parent_;

    int width_;
    int height_;

    QCheckBox* checkBox_;
    QTextEdit* textEdit_;
    QPushButton* button_;
};

#endif // LISTVIEWITEM_H
