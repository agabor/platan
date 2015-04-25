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

#include <QStringList>
#include <QDate>

#include <assert.h>

#include "datetransformation.h"

QDate DateTransformation::convert(QString Data) const
{
    return convert(Data, order, separator);
}

QDate DateTransformation::convert(QString data, DateOrder order, char separator) const
{
    QStringList string_list = data.split(separator);
    if (string_list.length() != 3 && string_list.length() != 4)
    {
        errorList.push_back(data);
        return QDate();
    }

    if(string_list.length() != 3 && string_list.at(4).length() != 0)
    {
        errorList.push_back(data);
        return QDate();
    }

    int year = 0, month = 0, day = 0;

    bool error = false;
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
            if (month < 1 || month > 12)
                error = true;
            break;
        case DAY:
            day = data;
            if (day < 1 || day > 31)
                error = true;

            break;
        }
    }

    QDate result(year, month, day);

    if (result.daysInMonth() < day)
        error = true;

    if (error)
        errorList.push_back(data);

    return result;
}
DateOrder DateTransformation::getOrder() const
{
    return order;
}

void DateTransformation::setOrder(const DateOrder &value)
{
    order = value;
}
char DateTransformation::getSeparator() const
{
    return separator;
}

void DateTransformation::setSeparator(char value)
{
    separator = value;
}

QString DateTransformation::getErrorMessage() const
{
    QString result = QObject::tr("The following values do not look like correctly formated dates: ");
    return result + errorListString();
}



