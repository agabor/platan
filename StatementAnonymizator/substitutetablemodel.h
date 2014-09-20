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

#ifndef SUBSTITUTETABLEMODEL_H
#define SUBSTITUTETABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QStringList>
#include <QString>
#include <QPair>
#include <validation.h>

class SubstituteTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit SubstituteTableModel(QAbstractTableModel *source);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index, const QVariant &value, int);

    void ReplaceAll(Ereaser &ereaser, int row, int column);

private:
    QVector<QPair<QStringList,QVector<QPair<int, QString>>>> Rows;
};

#endif // SUBSTITUTETABLEMODEL_H
