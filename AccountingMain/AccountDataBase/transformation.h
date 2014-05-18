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
    TransformationBase() : column(-1)
    {

    }

    int getColumn() const
    {
        return column;
    }
    
    void setColumn(int value)
    {
        column = value;
    }
    
    bool configured() const
    {
        return column != -1;
    }

    QVector<QString> getErrorList() const
    {
        return errorList;
    }


    void clearErrorList() const
    {
        errorList.clear();
    }

    virtual QString getErrorMessage() const = 0;

    
protected:
    int column;
    mutable QVector<QString> errorList;
    QString errorListString() const
    {
        QString values;
        for (QString val : errorList)
        {
            if (!values.isEmpty())
                values += ", ";
            values += val;
        }

        return values;
    }
};

template <class Result>
class Transformation : public TransformationBase
{
public:
    Transformation()
    {

    }

    Result apply(QAbstractTableModel *table, int row) const
    {
        QVariant Data = table->data(table->index(row, column));
        QString data_string = Data.toString();

        return convert(data_string);
    }

protected:
    virtual Result convert(QString Data) const = 0;
};

#endif // TRANSFORMATION_H

