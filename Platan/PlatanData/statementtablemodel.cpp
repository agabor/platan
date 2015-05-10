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

#include <QSharedPointer>

#include <statementtablemodel.h>
#include <statement.h>
#include <statements.h>


StatementTableModel::StatementTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int StatementTableModel::rowCount(const QModelIndex&) const
{
    return rowCount();
}

int StatementTableModel::rowCount() const
{
    return Rows.size();
}

int StatementTableModel::columnCount(const QModelIndex&) const
{
    return columnCount();
}


int StatementTableModel::columnCount() const
{
    return 7;
}


QVariant StatementTableModel::data(const QModelIndex& index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant::Invalid;
    auto value = Rows[index.row()]->at(index.column());
    if (index.column() == 6)
        return Statements::categoryList()[value.toInt()];
    return value;
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
            return tr("Date");
        case 1:
            return tr("Type");
        case 2:
            return tr("Description");
        case 3:
            return tr("Payee");
        case 4:
            return tr("PayeeAccount");
        case 5:
            return tr("Amount");
        case 6:
            return tr("Category");
        default:
            return QString{};
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
        SQLiteDate date = row->date();
        if (date == start || !date.isValid())
            continue;

        if (!min_date.isValid() || date < min_date)
            min_date = date;
        if (!max_date.isValid() || date > max_date)
            max_date = date;
    }

    return QPair<QDate, QDate>(min_date, max_date);
}

void StatementTableModel::setData(QVector<QSharedPointer<Statement>> rows)
{
    Rows = rows;
    emit layoutChanged ();
}

Statement& StatementTableModel::row(int idx)
{
    return *Rows[idx];
}
