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

#include "csvtablemodel.h"
#include <sstream>

CSVTableModel::CSVTableModel() :
    QAbstractTableModel()
{
    _columnCount = 0;
}

int CSVTableModel::rowCount(const QModelIndex& parent) const
{
    return Rows.size();
}

int CSVTableModel::columnCount(const QModelIndex& parent) const
{
    return _columnCount;
}

QVariant CSVTableModel::data(const QModelIndex& index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        return Rows[index.row()].at(index.column());
    }
    return QVariant::Invalid;
}

QVariant CSVTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        std::stringstream ss;
        if(orientation == Qt::Horizontal)
        {
            if (headers.length() <= section)
            {
                return QString{};
            }
            return headers.at(section);
        }
        else if(orientation == Qt::Vertical)
        {
            ss << section;
            return QString(ss.str().c_str());
        }
    }

    return QVariant::Invalid;
}

void CSVTableModel::addRow(QStringList row)
{
    pad(row, _columnCount);
    Rows.push_back(row);
}

void CSVTableModel::pad(QStringList &stringList, int columnCount)
{
    _columnCount = columnCount;
    const int addc = _columnCount - stringList.length();
    for (int i = 0; i < addc; ++i)
        stringList.append(QString());
}

void CSVTableModel::increaseColumnCountTo(int columnCount)
{
    pad(headers, columnCount);
    for(QStringList &r : Rows)
        pad(r, columnCount);
}

void CSVTableModel::setHeaders(QStringList headers)
{
    this->headers = headers;
    _columnCount = headers.length();
}
