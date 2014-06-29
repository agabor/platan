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

#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <QAbstractTableModel>
#include <QVector>

class TransformationBase
{
public:
    TransformationBase();

    int getColumn() const;
    
    void setColumn(int value);
    
    bool configured() const;

    QVector<QString> getErrorList() const;

    void clearErrorList() const;

    virtual QString getErrorMessage() const = 0;

    QString getData(int row, QAbstractTableModel *table) const;
    
protected:
    int column;
    mutable QVector<QString> errorList;
    QString errorListString() const;
};

template <class Result>
class Transformation : public TransformationBase
{
public:
    Result apply(QAbstractTableModel *table, int row) const
    {
        return convert(getData(row, table));
    }

protected:
    virtual Result convert(QString Data) const = 0;
};

#endif // TRANSFORMATION_H

