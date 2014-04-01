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

#ifndef DATETRANSFORMATION_H
#define DATETRANSFORMATION_H

#include "transformation.h"
#include <QDate>


enum DatePart
{
    YEAR  = 0x3,
    MONTH = 0x2,
    DAY   = 0x1
};

//The last 12 bit is used as flag:
enum DateOrder
{
    YMD = 0x123,
    YDM = 0x213,
    MYD = 0x132,
    MDY = 0x312,
    DYM = 0x231,
    DMY = 0x321
};

//gets the i. part of the Date format (Year, Month or Day)
inline DatePart part(DateOrder date, int i)
{
    return (DatePart)((date >> (i * 4)) % 0x10);
}

class DateTransformation : public Transformation<QDate>
{
public:
    DateTransformation(int column, DateOrder order, char separator, bool remove_quot)
        : Transformation(column, remove_quot), order(order), separator(separator)
    {

    }

    QDate convert(QString Data) const;
    static QDate convert(QString Data, DateOrder order, char separator);
protected:
    DateOrder order;
    char separator;
};

#endif // DATETRANSFORMATION_H
