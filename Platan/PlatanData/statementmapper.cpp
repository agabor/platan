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

#include <statementmapper.h>
#include <sqlcondition.h>
#include <sqlupdate.h>
#include <sqlinsert.h>
#include <sqlitedb.h>
#include <statement.h>
#include <tablestructure.h>

namespace columns
{
QString statements = "Statements";
QString id = "ID";
QString date = "Date";
QString type = "Type";
QString description = "Description";
QString payee = "Payee";
QString payeeAccount = "PayeeAccount";
QString amount = "Amount";
QString category = "Category";
QString rule = "Rule";
}

StatementMapper::StatementMapper(SQLiteDB &db) : data_base(db)
{
}


void StatementMapper::update(Statement &s) const
{
    SQLUpdate update{columns::statements};
    update.set(columns::date, s.date.toInt());
    update.set(columns::type, s.type);
    update.set(columns::description, s.description);
    update.set(columns::payee, s.payee);
    update.set(columns::payeeAccount, s.payeeAccount);
    update.set(columns::amount, s.amount);
    update.set(columns::category, s.category);
    update.where(QString("%1 = %2").arg(columns::id).arg(s.id));
    data_base.execute(update);
}

void StatementMapper::insert(Statement &s) const
{
    SQLInsert insert{columns::statements};
    insert.set(columns::date, s.date.toInt());
    insert.set(columns::type, s.type);
    insert.set(columns::description, s.description);
    insert.set(columns::payee, s.payee);
    insert.set(columns::payeeAccount, s.payeeAccount);
    insert.set(columns::amount, s.amount);
    insert.set(columns::category, 0);

    data_base.execute(insert);
}

QVector<Statement> StatementMapper::getAll(const SQLCondition &cond)
{
    QVector<Statement> result;
    SQLiteStatement statement;

    SQLSelect select{columns::statements};
    select.field(columns::id);
    select.field(columns::date);
    select.field(columns::type);
    select.field(columns::description);
    select.field(columns::payee);
    select.field(columns::payeeAccount);
    select.field(columns::amount);
    select.field(columns::category);
    select.where(QString("%1 < 0").arg(columns::amount));
    if (!cond.isEmpty())
        select.where(cond);

    data_base.prepare(statement, select);

    while (data_base.step(statement))
    {
        Statement row;
        row.id = statement.GetInt(0);
        row.date = statement.GetDate(1);
        row.type = statement.GetText(2);
        row.description = statement.GetText(3);
        row.payee = statement.GetText(4);
        row.payeeAccount = statement.GetText(5);
        row.amount = statement.GetDouble(6);
        row.category = statement.GetInt(7);
        result.push_back(row);
    }
    return result;
}

TableStructure StatementMapper::getStructure()
{
    TableStructure statements{columns::statements};
    statements.addField(columns::id, SQLType::Integer().PK());
    statements.addField(columns::date, SQLType::Integer());
    statements.addField(columns::type, SQLType::Text());
    statements.addField(columns::description, SQLType::Text());
    statements.addField(columns::payee, SQLType::Text());
    statements.addField(columns::payeeAccount, SQLType::Text());
    statements.addField(columns::amount, SQLType::Real());
    statements.addField(columns::category, SQLType::Integer());
    return statements;
}
