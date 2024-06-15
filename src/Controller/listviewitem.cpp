#include "listviewitem.h"

ListViewItem::ListViewItem(const QString &text, QWidget *parent) : QWidget(parent),
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

void ListViewItem::onCheckBoxStatedChanged(int state)
{
    emit checkBoxStateChanged(this, state);
}

void ListViewItem::onButtonClicked()
{
    emit buttonClicked(this);
}

void ListViewItem::setupCheckBox()
{
    checkBox_->setCheckState(Qt::Checked);
    checkBox_->setFixedSize(LVI::CheckBoxSize, LVI::CheckBoxSize);

    connect(checkBox_, &QCheckBox::stateChanged, this, &ListViewItem::onCheckBoxStatedChanged);
}

void ListViewItem::setupTextEdit()
{
    textEdit_->setReadOnly(true);
    textEdit_->setWordWrapMode(QTextOption::NoWrap);
    textEdit_->setTextInteractionFlags(Qt::NoTextInteraction);
    textEdit_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    textEdit_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    textEdit_->setFixedWidth(width_ - LVI::CheckBoxSize -
                             LVI::ButtonSize - 4 * LVI::MarginValue);
}

void ListViewItem::setupButton()
{
    button_->setFixedSize(LVI::ButtonSize, LVI::ButtonSize);
    button_->setIcon(QIcon::fromTheme("edit-clear"));
    button_->setIconSize(button_->size());

    connect(button_, &QPushButton::clicked, this, &ListViewItem::onButtonClicked);
}

QHBoxLayout* ListViewItem::setupLayout()
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
