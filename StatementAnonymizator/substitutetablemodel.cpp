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

#include <QVariant>

#include "substitutetablemodel.h"

SubstituteTableModel::SubstituteTableModel(QAbstractTableModel *source) :
    QAbstractTableModel(nullptr)
{
    for (int r = 0; r < source->rowCount(); ++r)
    {
        QStringList l;
        for (int c = 0; c < source->columnCount(); ++c)
            l.append(source->data(source->index(r,c)).toString());
        QPair<QStringList,QVector<QPair<int, QString>>> data;
        data.first = l;
        Rows.append(data);
    }
}

int SubstituteTableModel::rowCount(const QModelIndex &) const
{
    return Rows.count();
}

int SubstituteTableModel::columnCount(const QModelIndex &) const
{
    if (Rows.isEmpty())
        return 0;
    return Rows[0].first.count();
}

QVariant SubstituteTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant::Invalid;
    return Rows[index.row()].first[index.column()];
}

bool SubstituteTableModel::setData(const QModelIndex &index, const QVariant &value, int )
{
    int row = index.row();
    int column = index.column();
    Rows[row].first[column] = value.toString();
    return true;
}

void SubstituteTableModel::ReplaceAll(Ereaser &ereaser, int row, int column)
{
    auto &val = Rows[row].first[column];
    for(auto match : ereaser.getMatches(val))
    {
        Rows[row].second.push_back(match);
    }
}
