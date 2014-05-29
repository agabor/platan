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

#include "statementtablemodel.h"
#include <sstream>


StatementTableModel::StatementTableModel(QVector<Statement> rows, QObject *parent) :
    QAbstractTableModel(parent)
{
    Rows = rows;
}

int StatementTableModel::rowCount(const QModelIndex& parent) const
{
    return rowCount();
}

int StatementTableModel::rowCount() const
{
    return Rows.size();
}

int StatementTableModel::columnCount(const QModelIndex& parent) const
{
    return columnCount();
}


int StatementTableModel::columnCount() const
{
    return Statement::size();
}


QVariant StatementTableModel::data(const QModelIndex& index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        return Rows[index.row()].at(index.column());
    }
    return QVariant::Invalid;
}

QVariant StatementTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            switch(section)
            {
            case 0:
                return QString("Date");
            case 1:
                return QString("Type");
            case 2:
                return QString("Description");
            case 3:
                return QString("Payee");
            case 4:
                return QString("PayeeAccount");
            case 5:
                return QString("Amount");
            default:
                return QString("");
            }
        }
        else if(orientation == Qt::Vertical)
        {
            return QString::number(section);
        }
    }

    return QVariant::Invalid;
}



std::pair<QDate, QDate> StatementTableModel::DateRange() const
{
    if (Rows.size() == 0)
        return std::pair<QDate, QDate>();

    QDate min_date  = Rows.at(0).date;
    QDate max_date = min_date;

    for (auto row : Rows)
    {
        if (row.date == start)
            continue;

        if (row.date < min_date)
            min_date = row.date;
        if (row.date > max_date)
            max_date = row.date;
    }

    return std::pair<QDate, QDate>(min_date, max_date);
}

void StatementTableModel::setData(QVector<Statement> rows)
{
    Rows = rows;
    emit layoutChanged ();
}

Statement& StatementTableModel::row(int idx)
{
    return Rows[idx];
}
