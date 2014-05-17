#include "columnpropertieswidget.h"
#include <QVBoxLayout>
#include <QComboBox>
#include <widgethelpers.h>

ColumnPropertiesWidget::ColumnPropertiesWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    useAs = AddLabeledWidget<QComboBox>(mainLayout, tr("Usage"));
    useAs->addItem(tr("None"));
    useAs->addItem(tr("Ammount"));
    useAs->addItem(tr("Type"));
    useAs->addItem(tr("Date"));
    useAs->addItem(tr("Payee"));
    useAs->addItem(tr("Payee account"));
    useAs->addItem(tr("Description"));
    connect(useAs, SIGNAL(currentIndexChanged(int)), this, SLOT(setType(int)));

    format = AddLabeledWidget<QComboBox>(mainLayout, tr("Date format"));
    format->addItem(tr("Year - Month - Day"));
    format->addItem(tr("Month - Day - Year"));
    format->addItem(tr("Day - Month - Year"));
    format->setVisible(false);

    separator = AddLabeledWidget<QComboBox>(mainLayout, tr("Separator"));
    separator->addItem(tr("."));
    separator->addItem(tr(","));
    separator->addItem(tr("None"));
    separator->setVisible(false);
}

void ColumnPropertiesWidget::setType(int idx)
{
    format->setVisible(idx == 3);
    separator->setVisible(idx == 1);
}
