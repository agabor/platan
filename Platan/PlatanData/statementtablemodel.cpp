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

#include <QDate>

#include <memory>

#include <statementtablemodel.h>
#include <statement.h>


StatementTableModel::StatementTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
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
    return 6;
}


QVariant StatementTableModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant::Invalid;
    return Rows[index.row()]->at(index.column());
}

QVariant StatementTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant::Invalid;

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

    return QString::number(section);

}



QPair<QDate, QDate> StatementTableModel::DateRange() const
{
    if (Rows.size() == 0)
        return QPair<QDate, QDate>();

    QDate min_date;
    QDate max_date;

    for (auto row : Rows)
    {
        SQLiteDate date = row->date;
        if (date == start || !date.isValid())
            continue;

        if (!min_date.isValid() || date < min_date)
            min_date = date;
        if (!max_date.isValid() || date > max_date)
            max_date = date;
    }

    return QPair<QDate, QDate>(min_date, max_date);
}

void StatementTableModel::setData(QVector<std::shared_ptr<Statement>> rows)
{
    Rows = rows;
    emit layoutChanged ();
}

Statement& StatementTableModel::row(int idx)
{
    return *Rows[idx];
}
