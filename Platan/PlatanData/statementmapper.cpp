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

StatementMapper::StatementMapper(SQLiteDB &db) : data_base(db)
{
}


void StatementMapper::update(Statement &s) const
{
    SQLUpdate update{"Statements"};
    update.set("Date", s.date.toInt());
    update.set("Type", s.type);
    update.set("Description", s.description);
    update.set("Payee", s.payee);
    update.set("PayeeAccount", s.payeeAccount);
    update.set("Amount", s.amount);
    update.set("Class", s.category);
    update.where(QString("ID = %1").arg(s.id));
    data_base.execute(update);
}

void StatementMapper::insert(Statement &s) const
{
    SQLInsert insert{"Statements"};
    insert.set("Date", s.date.toInt());
    insert.set("Type", s.type);
    insert.set("Description", s.description);
    insert.set("Payee", s.payee);
    insert.set("PayeeAccount", s.payeeAccount);
    insert.set("Amount", s.amount);
    insert.set("Class", 0);

    data_base.execute(insert);
}

QVector<Statement> StatementMapper::getAll(const SQLCondition &cond)
{
    QVector<Statement> result;
    SQLiteStatement statement;

    SQLSelect select{"Statements"};
    select.field("ID");
    select.field("Date");
    select.field("Type");
    select.field("Description");
    select.field("Payee");
    select.field("PayeeAccount");
    select.field("Amount");
    select.field("Class");
    select.where("Amount < 0");
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
    TableStructure statements{"statements"};
    statements.addField("ID", SQLType::Integer().PK());
    statements.addField("Date", SQLType::Integer());
    statements.addField("Type", SQLType::Text());
    statements.addField("Description", SQLType::Text());
    statements.addField("Payee", SQLType::Text());
    statements.addField("PayeeAccount", SQLType::Text());
    statements.addField("Amount", SQLType::Real());
    statements.addField("Class", SQLType::Integer());
    return statements;
}
