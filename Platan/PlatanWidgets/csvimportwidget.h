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

#ifndef CSVIMPORTWIDGET_H
#define CSVIMPORTWIDGET_H

#include <QWidget>

#include <tabletransformer.h>

class QTableView;
class ColumnPropertiesWidget;
class CSVTableModel;
class Statement;

class CSVImportWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSVImportWidget(QWidget *parent = 0);
    void setTableModel (CSVTableModel *model);
    QVector<Statement> getImportedStatements() const;
    const TableTransformer &getTransformer() const;

    QStringList usageHeaders();
private slots:
    void typeChanged(ColumnType type);
    void separatorChanged(char c);
    void dateOrderChanged(DateOrder o);
    void currentCellChanged(QModelIndex idx);

private:
    QTableView *tableView;
    CSVTableModel *tableModel;
    ColumnPropertiesWidget *columnPropertiesWidget;
    int currentColumn() const;
    TableTransformer transformer;
};

#endif // CSVIMPORTWIDGET_H
