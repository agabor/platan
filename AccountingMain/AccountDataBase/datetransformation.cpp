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

#include "datetransformation.h"
#include <assert.h>
#include <QStringList>

QDate DateTransformation::convert(QString Data) const
{
    return convert(Data, order, separator);
}

QDate DateTransformation::convert(QString Data, DateOrder order, char separator)
{
    QStringList string_list = Data.split(separator);
    if (string_list.length() != 3 && string_list.length() != 4)
        return QDate();
    if(string_list.length() != 3 && string_list.at(4).length() != 0)
        return QDate();

    int year, month, day;

    for (int i = 0; i < 3; ++i)
    {
        int data = string_list[i].toInt();
        switch(part(order, i))
        {
        case YEAR:
            year = data < 100 ? data + 2000 : data;
            break;
        case MONTH:
            month = data;
            break;
        case DAY:
            day = data;
            break;
        }
    }

    return QDate(year, month, day);
}

