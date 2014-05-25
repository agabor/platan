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

#ifndef STATEMENTTABLEMODEL_H
#define STATEMENTTABLEMODEL_H

#include <QAbstractTableModel>
#include "statementrow.h"
#include <vector>
#include <QStringList>
#include "transformation.h"
#include <utility>

class StatementTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StatementTableModel(QVector<StatementRow> rows, QObject *parent = 0);
    int rowCount(const QModelIndex& parent) const;
    int rowCount() const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    StatementRow row(int idx);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    std::pair<QDate, QDate> DateRange() const;
protected:
    std::vector<StatementRow> Rows;
};

#endif // STATEMENTTABLEMODEL_H
