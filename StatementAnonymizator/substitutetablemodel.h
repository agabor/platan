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

#include <tuple>

#include <QAbstractTableModel>
#include <QVector>
#include <QStringList>
#include <QString>
#include <QPair>
#include <validation.h>

struct PlaceHolder
{
    int start;
    int end;
    QString value;
};

class SubstituteTableModel : public QAbstractTableModel
{
    Q_OBJECT
    template<typename T>
    using Table = QVector<QVector<T>>;
    using CellValue = std::tuple<QString,QVector<PlaceHolder>>;
public:
    explicit SubstituteTableModel(QAbstractTableModel *source);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &idx = QModelIndex()) const;
    int columnCount(const QModelIndex &idx = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);

    void ReplaceAll(Ereaser &ereaser, int row, int column);

    bool overlaps(PlaceHolder ph, const QVector<PlaceHolder> &data) const;
    QStringList getHeaders() const;
    void setHeaders(const QStringList &value);

    int getStartSection() const;
    void setStartSection(int value);
    void deleteRow(int idx);

private:
    Table<CellValue> rows;
    QStringList headers;
    int startSection;
};

#endif // SUBSTITUTETABLEMODEL_H
