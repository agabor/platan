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

#ifndef CSVTABLEMODEL_H
#define CSVTABLEMODEL_H

#include <QAbstractTableModel>
#include <QString>
#include <QStringList>
#include <vector>

class CSVTableModel : public QAbstractTableModel
{
public:
    CSVTableModel();
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    int columnCount(const QModelIndex& parent) const;
    int columnCount() const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setHeaders(QStringList headers);
    void setUsageHeaders(QStringList headers);
    void addRow(QStringList row);
    void increaseColumnCountTo(int columnCount);
    void pad(int columnCount, QStringList &stringList);
    void pad(QStringList &stringList, int columnCount);
    void setShowUsageHeaders(bool value);
private:
    std::vector<QStringList> Rows;
    QStringList headers;
    QStringList usageHeaders;
    bool showUsageHeaders;
    int _columnCount;
};

#endif // CSVTABLEMODEL_H
