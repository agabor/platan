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

#include <QTableView>
#include <QVBoxLayout>

#include "csvimportwidget.h"
#include <columnpropertieswidget.h>
#include <widgethelpers.h>
#include <csvtablemodel.h>
#include <statement.h>

CSVImportWidget::CSVImportWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayount = new QVBoxLayout(this);
    columnPropertiesWidget = new ColumnPropertiesWidget(this);
    columnPropertiesWidget->setFixedHeight(200);
    mainLayount->addWidget(columnPropertiesWidget);
    columnPropertiesWidget->setEnabled(false);
    tableView = new QTableView(this);
    mainLayount->addWidget(tableView);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectColumns);
    tableView->setWordWrap(true);
    connect(columnPropertiesWidget, SIGNAL(typeChanged(ColumnType)), this, SLOT(typeChanged(ColumnType)));
    connect(columnPropertiesWidget, SIGNAL(separatorChanged(char)), this, SLOT(separatorChanged(char)));
    connect(columnPropertiesWidget, SIGNAL(formatChanged(DateOrder)), this, SLOT(dateOrderChanged(DateOrder)));
    connect(tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(currentCellChanged(QModelIndex)));
}

void CSVImportWidget::setTableModel(CSVTableModel *model)
{
    tableModel = model;
    tableView->setModel(model);
    tableModel->setShowUsageHeaders(true);
    tableModel->setUsageHeaders(usageHeaders());
    resizeToContents(tableView);
}

QVector<Statement> CSVImportWidget::getImportedStatements() const
{
    return transformer.transform(tableModel);
}

const TableTransformer &CSVImportWidget::getTransformer() const
{
    return transformer;
}

QStringList CSVImportWidget::usageHeaders()
{
    int column_count = tableModel->columnCount();
    QStringList result;
    for (int i = 0; i < column_count; ++i)
        result.append(toString(transformer.getColumnType(i)));

    return result;
}

void CSVImportWidget::typeChanged(ColumnType type)
{
    int column = currentColumn();
    if (column != -1)
    {
        transformer.setColumnType(column, type);
        tableModel->setUsageHeaders(usageHeaders());
    }
}

void CSVImportWidget::separatorChanged(char c)
{
    if (columnPropertiesWidget->type() == ColumnType::Amount)
        transformer.Amount.setDecimal(c);
    else if (columnPropertiesWidget->type() == ColumnType::Date)
        transformer.Date.setSeparator(c);
}

void CSVImportWidget::dateOrderChanged(DateOrder o)
{
    transformer.Date.setOrder(o);
}

void CSVImportWidget::currentCellChanged(QModelIndex)
{
   columnPropertiesWidget->setEnabled(true);
   ColumnType type = transformer.getColumnType(currentColumn());
   columnPropertiesWidget->setType(type);
   if (type == ColumnType::Amount)
   {
       columnPropertiesWidget->setSeparator(transformer.Amount.getDecimal());
   }
   else if (type == ColumnType::Date)
   {
       columnPropertiesWidget->setSeparator(transformer.Date.getSeparator());
       columnPropertiesWidget->setDateOrder(transformer.Date.getOrder());
   }
}

int CSVImportWidget::currentColumn() const
{
    return tableView->currentIndex().column();
}
