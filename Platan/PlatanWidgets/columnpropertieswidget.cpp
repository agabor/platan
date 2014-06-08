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
#include <QComboBox>
#include <widgethelpers.h>

ColumnPropertiesWidget::ColumnPropertiesWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    useAs = AddNamedWidget<QComboBox>(mainLayout, tr("Usage"));
    useAs->addItem(tr("None"), (int)ColumnType::None);
    useAs->addItem(tr("Amount"), (int)ColumnType::Amount);
    useAs->addItem(tr("Type"), (int)ColumnType::Type);
    useAs->addItem(tr("Date"), (int)ColumnType::Date);
    useAs->addItem(tr("Payee"), (int)ColumnType::Payee);
    useAs->addItem(tr("Payee account"), (int)ColumnType::PayeeAccount);
    useAs->addItem(tr("Description"), (int)ColumnType::Description);
    connect(useAs, SIGNAL(currentIndexChanged(int)), this, SLOT(setType(int)));

    auto data = AddLabeledWidget<QComboBox>(mainLayout, tr("Date format"));
    format = data.first;
    formatLabel = data.second;
    format->addItem(tr("Year - Month - Day"), (int)YMD);
    format->addItem(tr("Month - Day - Year"), (int)MDY);
    format->addItem(tr("Day - Month - Year"), (int)DMY);
    format->setEnabled(false);
    formatLabel->setEnabled(false);
    connect(format, SIGNAL(currentIndexChanged(int)), this, SLOT(setDateOrder(int)));

    data = AddLabeledWidget<QComboBox>(mainLayout, tr("Separator"));
    separator = data.first;
    separatorLabel = data.second;
    separator->addItem(tr("."), QChar('.'));
    separator->addItem(tr(","), QChar(','));
    separator->addItem(tr("None"), QChar('\0'));
    separator->setEnabled(false);
    separatorLabel->setEnabled(false);
    connect(separator, SIGNAL(currentIndexChanged(int)), this, SLOT(setSeparator(int)));
}

ColumnType ColumnPropertiesWidget::type()
{
    return (ColumnType)useAs->currentData().toInt();
}

void ColumnPropertiesWidget::setType(ColumnType type)
{
  for (int i = 0; i < useAs->count(); ++i)
  {
      if (useAs->itemData(i).toInt() == (int)type)
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
    format->setEnabled(idx == 3);
    formatLabel->setEnabled(format->isVisible());
    separator->setEnabled(idx == 1 || idx == 3);
    separatorLabel->setEnabled(separator->isVisible());
    emit typeChanged(type());
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
