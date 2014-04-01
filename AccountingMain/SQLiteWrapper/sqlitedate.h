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

#ifndef SQLITEDATE_H
#define SQLITEDATE_H

#include <string>
#include <QDate>
#include <QString>

const QDate start(1970, 1, 1);

class SQLiteDate : public QDate
{
public:
    SQLiteDate(){}
    SQLiteDate(int value)
    {
        QDate qdate = start.addDays(value);
        setDate(qdate.year(), qdate.month(), qdate.day());
    }
    SQLiteDate(const QDate &qdate) : QDate(qdate.year(), qdate.month(), qdate.day()) {}
    inline int toInt() const;
};

//inline definitions

int SQLiteDate::toInt() const
{
    return start.daysTo(*this);
}





#endif // SQLITEDATE_H
