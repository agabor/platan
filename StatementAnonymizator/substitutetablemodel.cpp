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
#include <QColor>

#include "substitutetablemodel.h"

SubstituteTableModel::SubstituteTableModel(QAbstractTableModel *source) :
    QAbstractTableModel(nullptr)
{
    for (int r = 0; r < source->rowCount(); ++r)
    {
        QStringList l;
        for (int c = 0; c < source->columnCount(); ++c)
            l.append(source->data(source->index(r,c)).toString());
        QVector<CellValue> data;
        for (auto str : l)
        {
            QVector<PlaceHolder> empty;
            data.push_back(CellValue{str, empty});
        }
        rows.append(data);
    }
    startSection = 20;
}

int SubstituteTableModel::rowCount(const QModelIndex &) const
{
    return rows.count();
}

int SubstituteTableModel::columnCount(const QModelIndex &) const
{
    if (rows.isEmpty())
        return 0;
    return rows[0].count();
}

bool SubstituteTableModel::overlapsWithPrevious(PlaceHolder ph, const QVector<PlaceHolder> &data, int i) const
{
    for (int j = 0; j < i; ++j)
    {
        auto jph = data[j];
        if (!(jph.end < ph.start
            || ph.end < jph.start))
            return true;
    }
    return false;
}
QStringList SubstituteTableModel::getHeaders() const
{
    return headers;
}

void SubstituteTableModel::setHeaders(const QStringList &value)
{
    headers = value;
}
int SubstituteTableModel::getStartSection() const
{
    return startSection;
}

void SubstituteTableModel::setStartSection(int value)
{
    startSection = value;
    emit layoutChanged ();
}

void SubstituteTableModel::deleteRow(int idx)
{
    rows.remove(idx);
    emit layoutChanged ();
}




QVariant SubstituteTableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        auto &data = rows[index.row()][index.column()];
        QString val{data.first};
        int shift = 0;
        for (int i = 0; i < data.second.size(); ++i)
        {
            PlaceHolder ph = data.second[i];
            if(overlapsWithPrevious(ph, data.second, i))
                continue;
            int length = ph.end - ph.start + 1;
            val.replace(ph.start + shift, length, ph.value);
            shift += ph.value.length() - length;
        }
        return val;
    } else if (role == Qt::BackgroundColorRole)
    {
        if (index.row() < startSection)
            return QColor(230,255,230);
    }
    return QVariant::Invalid;
}

QVariant SubstituteTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            if (headers.length() > section)
                return headers.at(section);
        } else if(orientation == Qt::Vertical)
        {
            return section;
        }
    }
    return QVariant::Invalid;
}

bool SubstituteTableModel::setData(const QModelIndex &index, const QVariant &value, int )
{
    int row = index.row();
    int column = index.column();
    rows[row][column].first = value.toString();
    return true;
}

void SubstituteTableModel::ReplaceAll(Ereaser &ereaser, int row, int column)
{
    auto &val = rows[row][column].first;
    for(auto match : ereaser.getMatches(val))
    {
        PlaceHolder ph;
        ph.start = match.first;
        ph.end = match.first + match.second.length() - 1;
        ph.value = ereaser.getTag(match.second);
        rows[row][column].second.push_back(ph);
    }
}
