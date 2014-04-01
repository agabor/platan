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

template <class Result>
class Transformation
{
public:
    Transformation(int column, bool remove_qout) : column(column), remove_qout(remove_qout)
    {

    }

    Result apply(QAbstractTableModel *table, int row) const
    {
        QVariant Data = table->data(table->index(row, column));
        QString data_string = Data.toString();
        if (remove_qout)
        {
            QChar first = data_string.at(0);
            if (first == '"' || first =='\'')
            {
                const int last_idx = data_string.length() - 1;
                QChar last = data_string.at(last_idx);
                if (first == last)
                {
                    data_string = data_string.mid(1, last_idx - 1);
                }
            }
        }
        return convert(data_string);
    }

protected:
    virtual Result convert(QString Data) const = 0;
    int column;
    bool remove_qout;
};

#endif // TRANSFORMATION_H
