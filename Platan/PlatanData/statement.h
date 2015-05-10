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

#ifndef STATEMENT_H
#define STATEMENT_H

#include <QString>
#include <sqlitedate.h>

class QVariant;



class Statement
{
public:
    int id;
    SQLiteDate date;
    QString type;
    QString description;
    QString payee;
    QString payeeAccount;
    float amount;
    int category;
    int ruleId;

    enum class Column
    {
      Date = 0,
      Type = 1,
      Description = 2,
      Payee = 3,
      PayeeAccount = 4,
      Amount = 5,
      Category = 6,
      RuleId = 7,
    };

    QVariant at(int idx) const;
    QVariant at(Column idx) const;
};

#endif // STATEMENTROW_H
