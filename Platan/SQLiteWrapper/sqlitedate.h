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
#include <sqlcondition.h>

const QDate start(1970, 1, 1);

class SQLiteDate : public QDate
{
public:
    SQLiteDate(){}

	explicit SQLiteDate(int value);

    SQLiteDate(const QDate& qdate);
	inline int toInt() const;
    SimpleCondition greater(QString field) const;
    SimpleCondition less(QString field) const;
};

//inline definitions

int SQLiteDate::toInt() const
{
    return start.daysTo(*this);
}


#endif // SQLITEDATE_H
