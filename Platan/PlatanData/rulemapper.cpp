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

namespace columns
{
QString rules = "Rules";
QString column = "Column";
QString id = "Id";
QString value = "Value";
QString category = "Category";
QString type = "Type";
QString country = "Country";
}

RuleMapper::RuleMapper(SQLiteDB &db) : data_base(db)
{
}

void RuleMapper::insert(Rule &r) const
{
    SQLInsert insert(columns::rules);
    insert.set(columns::column, r.column);
    insert.set(columns::value, r.value);
    insert.set(columns::category, r.category);
    insert.set(columns::type, static_cast<int>(r.type));

    data_base.execute(insert);
}

QVector<Rule> RuleMapper::getAll(int _country) const
{
    SQLSelect select{columns::rules};
    select.field(columns::id);
    select.field(columns::column);
    select.field(columns::value);
    select.field(columns::category);
    select.field(columns::type);
    if (_country != -1)
        select.where(QString("%1 = %2").arg(columns::country).arg(_country));

    SQLiteStatement statement;

    data_base.prepare(statement, select);

    QVector<Rule> result;

    while (data_base.step(statement))
    {
        int id = statement.GetInt(0);
        int column = statement.GetInt(1);
        QString value = statement.GetText(2);
        int category = statement.GetInt(3);
        Rule::Type type = static_cast<Rule::Type>(statement.GetInt(3));
        result.push_back(Rule(id, column, value, category, type));
    }

    return result;
}

TableStructure RuleMapper::getStructure()
{
    TableStructure rules{columns::rules};
    rules.addField(columns::id, SQLType::DefaultPK());
    rules.addField(columns::column, SQLType::Integer().NotNull());
    rules.addField(columns::value, SQLType::Text().NotNull());
    rules.addField(columns::category, SQLType::Integer().NotNull());
    rules.addField(columns::type, SQLType::Integer().NotNull());
    return rules;
}

TableStructure RuleMapper::getStructureWithCountry()
{
    TableStructure rules = getStructure();
    rules.addField(columns::country, SQLType::Integer());
    return rules;
}

void RuleMapper::remove(Rule &r)
{
    SQLDelete del{columns::rules};
    del.where(QString("%1 = %2").arg(columns::id).arg(r.id));
}

void RuleMapper::update(Rule &r)
{
    SQLUpdate update{columns::rules};
    update.set(columns::column, r.column);
    update.set(columns::value, r.value);
    update.set(columns::category, r.category);
    update.set(columns::type, static_cast<int>(r.type));
    update.where(QString("%1 = %2").arg(columns::id).arg(r.id));
}
