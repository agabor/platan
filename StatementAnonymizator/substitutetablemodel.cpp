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

#include <tuple>
#include <algorithm>

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

bool SubstituteTableModel::overlaps(PlaceHolder ph, const QVector<PlaceHolder> &data) const
{
    for (auto jph : data)
    {
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
    int rc = rowCount();
    if (startSection < rc)
        return startSection;
    return rc-1;
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


bool less(const PlaceHolder &p1, const PlaceHolder &p2)
{
    return p1.start < p2.start;
}

QVariant SubstituteTableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        QString val;
        QVector<PlaceHolder> placeholders;
        std::tie(val, placeholders) = rows[index.row()][index.column()];
        if (placeholders.isEmpty())
            return val;

        QString list;

        int lastEnd = 0;
        for (auto ph : placeholders)
        {
            list += val.mid(lastEnd, ph.start-lastEnd);
            list += ph.value;
            lastEnd = ph.end + 1;
        }
        list += val.mid(lastEnd);
        return list;
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
            return section + 1;
        }
    }
    return QVariant::Invalid;
}

bool SubstituteTableModel::setData(const QModelIndex &index, const QVariant &value, int )
{
    int row = index.row();
    int column = index.column();
    std::get<0>(rows[row][column]) = value.toString();
    return true;
}

void SubstituteTableModel::ReplaceAll(Ereaser &ereaser, int row, int column)
{
    auto &val = std::get<0>(rows[row][column]);
    auto &phs = std::get<1>(rows[row][column]);
    for(auto match : ereaser.getMatches(val))
    {
        PlaceHolder ph;
        ph.start = match.first;
        ph.end = match.first + match.second.length() - 1;
        ph.value = ereaser.getTag(match.second);
        if (!overlaps(ph, phs))
            phs.push_back(ph);

    }
    std::sort(phs.begin(), phs.end(), less);
}
