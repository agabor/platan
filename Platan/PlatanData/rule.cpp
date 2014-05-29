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

#include "rule.h"
#include <sqlinsert.h>
#include <sqlitedb.h>
#include <QVector>

Rule::Rule(int _column, QString _value, int _category)
    :column(_column), value(_value), category(_category)
{
}

Rule::Rule()
{
}

void Rule::insert() const
{
    SQLInsert insert("rules");
    insert.set("Column", column);
    insert.set("Value", value);
    insert.set("Class", category);

    SQLiteDB::getInstance().Execute(insert);
}

QVector<Rule> Rule::getAll(int column)
{
    SQLSelect select{"rules"};
    select.field("Column");
    select.field("Value");
    select.field("Class");
    if (column != -1)
        select.where(QString("Column = %1").arg(column));

    SQLiteStatement statement;

    auto data_base = SQLiteDB::getInstance();

    data_base.Prepare(statement, select);

    QVector<Rule> result;

    while (data_base.Step(statement))
    {
        result.push_back(Rule(statement.GetInt(0), statement.GetText(1), statement.GetInt(2)));
    }

    return result;
}
