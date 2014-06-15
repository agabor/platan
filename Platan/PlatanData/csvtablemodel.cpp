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
#include <QColor>

CSVTableModel::CSVTableModel() :
    QAbstractTableModel(),
    showUsageHeaders(false)
{
    _columnCount = 0;
}

int CSVTableModel::rowCount(const QModelIndex&) const
{
    return Rows.size();
}

int CSVTableModel::columnCount(const QModelIndex&) const
{
    return _columnCount;
}

int CSVTableModel::columnCount() const
{
    return _columnCount;
}

QVariant CSVTableModel::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    int column = index.column();

    switch(role)
    {
    case Qt::DisplayRole:
        if (showUsageHeaders)
        {
            if (row == 0)
            {
                if (usageHeaders.length() > column)
                    return usageHeaders[column];
                else
                    return QString();
            }
            return Rows[row-1].at(column);
        } else
            return Rows[row].at(column);
    case Qt::UserRole:
        return Rows[row].at(column);
    case Qt::ForegroundRole:
        if (showUsageHeaders && row == 0)
            return QColor(Qt::blue);
        return QVariant::Invalid;
    default:
        return QVariant::Invalid;
    }
}

QVariant CSVTableModel::columnHeaderData(int role, int section) const
{
    if(role == Qt::DisplayRole)
    {
        if (headers.length() > section)
        return headers.at(section);
    }

    return QVariant::Invalid;
}

QVariant CSVTableModel::rowHeaderData(int role, int section) const
{
    switch(role)
    {
    case Qt::DisplayRole:
        if (showUsageHeaders)
        {
            if (section == 0)
                return QString(tr("Usage"));
            return QString::number(section-1);
        }
        return QString::number(section);

    case Qt::ForegroundRole:
        if (showUsageHeaders && section == 0)
            return QColor(Qt::blue);
        return QVariant::Invalid;
    case Qt::UserRole:
        return QString::number(section);
    default:
        return QVariant::Invalid;
    }
}

QVariant CSVTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal)
    {
        return columnHeaderData(role, section);
    }
    else if(orientation == Qt::Vertical)
    {
        return rowHeaderData(role, section);
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

void CSVTableModel::setShowUsageHeaders(bool value)
{
    showUsageHeaders = value;
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

void CSVTableModel::setUsageHeaders(QStringList headers)
{
    usageHeaders = headers;
    emit layoutChanged ();
}
