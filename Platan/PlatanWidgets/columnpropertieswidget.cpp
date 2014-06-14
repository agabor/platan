// This file is part of Platan.
// Copyright (C) 2014 Gábor Angyal
//
// Platan is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Platan is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Platan.  If not, see <http://www.gnu.org/licenses/>.

#include "columnpropertieswidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <widgethelpers.h>
#include <QMetaType>
#include <qlabeledcombobox.h>
#include <QDebug>

Q_DECLARE_METATYPE(ColumnType)

ColumnPropertiesWidget::ColumnPropertiesWidget(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    QVBoxLayout* labelLayout = new QVBoxLayout();
    mainLayout->addLayout(labelLayout);

    QVBoxLayout* controlLayout = new QVBoxLayout();
    mainLayout->addLayout(controlLayout);

    useAs = new QComboBox(this);
    controlLayout->addWidget(useAs);
    useAs->addItem(tr("None"), qVariantFromValue(ColumnType::None));
    useAs->addItem(tr("Amount"), qVariantFromValue(ColumnType::Amount));
    useAs->addItem(tr("Type"), qVariantFromValue(ColumnType::Type));
    useAs->addItem(tr("Date"), qVariantFromValue(ColumnType::Date));
    useAs->addItem(tr("Payee"), qVariantFromValue(ColumnType::Payee));
    useAs->addItem(tr("Payee account"), qVariantFromValue(ColumnType::PayeeAccount));
    useAs->addItem(tr("Description"), qVariantFromValue(ColumnType::Description));
    connect(useAs, SIGNAL(currentIndexChanged(int)), this, SLOT(setType(int)));

    useAsLabel = new QLabel(tr("Usage"), this);
    useAs->setFixedHeight(useAs->height());
    useAsLabel->setFixedHeight(useAs->height());
    labelLayout->addWidget(useAsLabel);

    format = new QComboBox(this);//tr("Date format"));
    controlLayout->addWidget(format);
    format->addItem(tr("Year - Month - Day"), (int)YMD);
    format->addItem(tr("Month - Day - Year"), (int)MDY);
    format->addItem(tr("Day - Month - Year"), (int)DMY);
    connect(format, SIGNAL(currentIndexChanged(int)), this, SLOT(setDateOrder(int)));

    formatLabel = new QLabel(tr("Date format"), this);
    format->setFixedHeight(format->height());
    formatLabel->setFixedHeight(format->height());
    labelLayout->addWidget(formatLabel);

    separator = new QComboBox(this);//tr("Separator"));
    controlLayout->addWidget(separator);
    separator->addItem(tr("."), QChar('.'));
    separator->addItem(tr(","), QChar(','));
    separator->addItem(tr("None"), QChar('\0'));
    connect(separator, SIGNAL(currentIndexChanged(int)), this, SLOT(setSeparator(int)));

    separatorLabel = new QLabel(tr("Separator"), this);
    separator->setFixedHeight(separator->height());
    separatorLabel->setFixedHeight(separator->height());
    labelLayout->addWidget(separatorLabel);

    explanation = new QLabel(this);
    explanation->setWordWrap(true);
    mainLayout->addWidget(explanation);
    useAs->setCurrentIndex(0);
}

ColumnType ColumnPropertiesWidget::type()
{
    return  useAs->currentData().value<ColumnType>();
}

void ColumnPropertiesWidget::setType(ColumnType type)
{
  for (int i = 0; i < useAs->count(); ++i)
  {
      if (useAs->itemData(i).value<ColumnType>() == type)
      {
          useAs->setCurrentIndex(i);
          break;
      }
  }
}

void ColumnPropertiesWidget::setSeparator(char c)
{
    for (int i = 0; i < separator->count(); ++i)
    {
        if (separator->itemData(i).toChar().toLatin1() == c)
        {
            separator->setCurrentIndex(i);
            break;
        }
    }
}

void ColumnPropertiesWidget::setDateOrder(DateOrder o)
{
    for (int i = 0; i < format->count(); ++i)
    {
        if (format->itemData(i).toInt() == (int)o)
        {
            format->setCurrentIndex(i);
            break;
        }
    }
}


void ColumnPropertiesWidget::setType(int idx)
{
    format->setVisible(idx == 3);
    formatLabel->setVisible(format->isVisible());
    separator->setVisible(idx == 1 || idx == 3);
    separatorLabel->setVisible(separator->isVisible());

    ColumnType currentType = type();

    QString explanationStr;

    switch (currentType) {
    case ColumnType::Amount:
        explanationStr = tr("The amount of the expense or income given in the appropriate "
                            "currency. If the currency uses decimal places, then the appropriate "
                            "separator (. or ,) should be set. (It is usually auto detected.)");
        break;
    case ColumnType::Date:
        explanationStr = tr("The date when the transaction was made. A full date with year, month "
                            "and day value is neccesarry. The date order should be set approprietly "
                            "(It is usually autodetected.)");
        break;
    case ColumnType::Description:
        explanationStr = tr("Usually a longer text describing the purpose of the transaction.");
        break;
    case ColumnType::None:
        explanationStr = tr("This column is not imported.");
        break;
    case ColumnType::Payee:
        explanationStr = tr("The person, company or organization for whom the payment is for.");
        break;
    case ColumnType::PayeeAccount:
        explanationStr = tr("The bank account involved in the transaction.");
        break;
    case ColumnType::Type:
        explanationStr = tr("Type of the transaction. This field is used to ditinguish between the "
                            "seperate transaction type, for exampple using a bank automat, making a "
                            "bank transfer, paying with your card etc ...");
        break;
    }

    explanation->setText(explanationStr);

    emit typeChanged(currentType);
}

void ColumnPropertiesWidget::setSeparator(int idx)
{
    emit separatorChanged(separator->itemData(idx).toChar().toLatin1());
}

void ColumnPropertiesWidget::setDateOrder(int idx)
{
   DateOrder o;
   switch(idx)
   {
   case 0:
     o = YMD;
     break;
   case 1:
     o = MDY;
     break;
   case 2:
     o = DMY;
     break;
   }
   emit formatChanged(o);
}
