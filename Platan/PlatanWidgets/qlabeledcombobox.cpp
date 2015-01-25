#include "qlabeledcombobox.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>

QLabeledComboBox::QLabeledComboBox(QString text, QWidget *parent) :
     QComboBox(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    label = new QLabel(text, this);
    mainLayout->addWidget(label);
    mainLayout->addWidget(dynamic_cast<QComboBox*>(this));
}
