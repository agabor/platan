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

#include <QVector>
#include <QString>

#include <rulemapper.h>
#include <sqlupdate.h>
#include <sqlinsert.h>
#include <sqldelete.h>

#include <tablestructure.h>
#include <sqlitedb.h>
#include <rule.h>

RuleMapper::RuleMapper(SQLiteDB &db) : data_base(db)
{
}

void RuleMapper::insert(Rule &r) const
{
    SQLInsert insert("rules");
    insert.set("Column", r.column);
    insert.set("Value", r.value);
    insert.set("Class", r.category);

    data_base.execute(insert);
}

QVector<Rule> RuleMapper::getAll(int country) const
{
    SQLSelect select{"rules"};
    select.field("Column");
    select.field("Value");
    select.field("Class");
    if (country != -1)
        select.where(QString("Country = %1").arg(country));

    SQLiteStatement statement;

    data_base.prepare(statement, select);

    QVector<Rule> result;

    while (data_base.step(statement))
    {
        result.push_back(Rule(statement.GetInt(0), statement.GetText(1), statement.GetInt(2)));
    }

    return result;
}

TableStructure RuleMapper::getStructure()
{
    TableStructure rules{"rules"};
    rules.addField("Id", SQLType::DefaultPK());
    rules.addField("Column", SQLType::Integer());
    rules.addField("Value", SQLType::Text());
    rules.addField("Class", SQLType::Integer());
    return rules;
}

TableStructure RuleMapper::getStructureWithCountry()
{
    TableStructure rules = getStructure();
    rules.addField("Country", SQLType::Integer());
    return rules;
}

void RuleMapper::deleteRule(Rule rule)
{
    SQLDelete del("rules");
    del.where("");
}
