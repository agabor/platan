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

#ifndef STATEMENTROW_H
#define STATEMENTROW_H

#include <QDateTime>
#include <string>
#include <QVariant>
#include <sqlitedate.h>


struct StatementRow
{
    SQLiteDate Date;
    QString Type;
    QString Description;
    QString Payee;
    QString PayeeAccount;
    float Amount;
    int Class;

    int size() const
    {
        return 6;
    }
    QVariant at(int idx) const
    {
        switch (idx)
        {
        case 0:
            return Date;
        case 1:
            return Type;
        case 2:
            return Description;
        case 3:
            return Payee;
        case 4:
            return PayeeAccount;
        case 5:
            return Amount;
        }
        return 0;
    }
};

#endif // STATEMENTROW_H
