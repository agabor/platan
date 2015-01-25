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
#include <QVector>

#include <memory>

#include <transformation.h>
#include <statement.h>

template <class T1, class T2>
struct QPair;
class QDate;

class StatementTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StatementTableModel(QObject *parent = 0);
    int rowCount(const QModelIndex&) const;
    int rowCount() const;
    int columnCount(const QModelIndex&) const;
    int columnCount() const;
    QVariant data(const QModelIndex& index, int role) const;
    Statement &row(int idx);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QPair<QDate, QDate> DateRange() const;
    void setData(QVector<std::shared_ptr<Statement> > rows);
protected:
    QVector<std::shared_ptr<Statement>> Rows;
};

#endif // STATEMENTTABLEMODEL_H
