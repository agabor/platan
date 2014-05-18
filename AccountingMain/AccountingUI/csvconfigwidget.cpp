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

#include "csvconfigwidget.h"
#include <QFile>
#include <QTextStream>
#include <csvreader.h>
#include <QTableView>
#include <csvpropertieswidget.h>
#include <QVBoxLayout>

CSVConfigWidget::CSVConfigWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    CSVReaderProperties = new CSVPropertiesWidget(this);
    mainLayout->addWidget(CSVReaderProperties);

    connect(CSVReaderProperties, SIGNAL(readerParametersChanged()), this, SLOT(readCSV()));

    tableView = new QTableView(this);
    mainLayout->addWidget(tableView);
}

void CSVConfigWidget::setReader(QString filename, CSVReader *r)
{
    reader = r;
    CSVReaderProperties->setReader(r);
    QFile inFile(filename);
    inFile.open(QIODevice::ReadOnly | QIODevice::Text);
    input = inFile.readAll();
    readCSV();
}

void CSVConfigWidget::readCSV()
{
    QTextStream input_stream(&input);
    tableModel = reader->read(input_stream);
    tableView->setModel(tableModel);
}

CSVTableModel *CSVConfigWidget::getTableModel() const
{
    return tableModel;
}
