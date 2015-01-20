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
#include <exportrules.h>


RuleMapper::RuleMapper(SQLiteDB &db) : data_base(db)
{
}


void RuleMapper::insert(Rule &r) const
{
    SQLInsert insert = getInsertQuery(r);

    data_base.execute(insert);
}

QVector<Rule> RuleMapper::getAll(int _country) const
{
    SQLSelect select{db_rule::rules};
    select.field(db_rule::id);
    select.field(db_rule::column);
    select.field(db_rule::value);
    select.field(db_rule::category);
    select.field(db_rule::type);
    if (_country != -1)
        select.where(QString("%1 = %2").arg(db_rule::country).arg(_country));

    SQLiteStatement statement;

    data_base.prepare(statement, select);

    QVector<Rule> result;

    while (data_base.step(statement))
    {
        int id = statement.GetInt(0);
        int column = statement.GetInt(1);
        QString value = statement.GetText(2);
        int category = statement.GetInt(3);
        Rule::Type type = static_cast<Rule::Type>(statement.GetInt(4));
        result.push_back(Rule(id, column, value, category, type));
    }

    return result;
}

TableStructure RuleMapper::getStructure()
{
    TableStructure rules{db_rule::rules};
    rules.addField(db_rule::id, SQLType::Integer().PK().NotNull());
    rules.addField(db_rule::column, SQLType::Integer().NotNull());
    rules.addField(db_rule::value, SQLType::Text().NotNull());
    rules.addField(db_rule::category, SQLType::Integer().NotNull());
    rules.addField(db_rule::type, SQLType::Integer().NotNull());
    return rules;
}

TableStructure RuleMapper::getStructureWithCountry()
{
    TableStructure rules = getStructure();
    rules.addField(db_rule::country, SQLType::Integer());
    return rules;
}

void RuleMapper::remove(Rule &r)
{
    SQLDelete del{db_rule::rules};
    del.where(QString("%1 = %2").arg(db_rule::id).arg(r.id));
    data_base.execute(del);
}

void RuleMapper::update(Rule &r)
{
    SQLUpdate update{db_rule::rules};
    update.set(db_rule::column, r.column);
    update.set(db_rule::value, r.value);
    update.set(db_rule::category, r.category);
    update.set(db_rule::type, static_cast<int>(r.type));
    update.where(QString("%1 = %2").arg(db_rule::id).arg(r.id));
    data_base.execute(update);
}
