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

#include "statementextracttablemodel.h"
#include <sstream>

StatementExtractTableModel::StatementExtractTableModel(QVector<StatementExtractRow> rows, QObject *parent) :
    QAbstractTableModel(parent)
{
    Rows = rows;
}

int StatementExtractTableModel::rowCount(const QModelIndex &parent) const
{
    return Rows.size();
}

int StatementExtractTableModel::rowCount() const
{
    return Rows.size();
}

int StatementExtractTableModel::columnCount(const QModelIndex &parent) const
{
    return StatementExtractRow::size();
}

QVariant StatementExtractTableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        return Rows[index.row()].at(index.column());
    }
    return QVariant::Invalid;
}

QVariant StatementExtractTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        std::stringstream ss;
        if(orientation == Qt::Horizontal)
        {
            switch(section)
            {
            case 0:
                return QString("Date");
            case 1:
                return QString("Payee");
            case 2:
                return QString("Amount");
            default:
                return QString("");
            }
        }
        else if(orientation == Qt::Vertical)
        {
            ss << section;
            return QString(ss.str().c_str());
        }
    }

    return QVariant::Invalid;
}

void StatementExtractTableModel::setData(QVector<StatementExtractRow> rows)
{
    Rows = rows;
    emit layoutChanged();
}
