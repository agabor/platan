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

#ifndef CSVCONFIGWIDGET_H
#define CSVCONFIGWIDGET_H

#include <QWidget>

class CSVReader;
class CSVTableModel;
class CSVPropertiesWidget;
class QTableView;
class QString;

class CSVConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSVConfigWidget(QWidget *parent);
    void setReader(QString filename, CSVReader *r);
    CSVTableModel *getTableModel() const;
private slots:
    void readCSV();
private:
    CSVReader *reader;
    QByteArray input;
    CSVPropertiesWidget *CSVReaderProperties;
    QTableView *tableView;
    CSVTableModel *tableModel;
};

#endif // CSVCONFIGWIDGET_H
