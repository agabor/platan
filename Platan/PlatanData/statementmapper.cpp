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

namespace db_statement
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
QString ruleId = "RuleID";
}

StatementMapper::StatementMapper(SQLiteDB &db) : data_base(db)
{
}


void StatementMapper::update(Statement &s) const
{
    SQLUpdate update{db_statement::statements};
    update.set(db_statement::date, s.date.toInt());
    update.set(db_statement::type, s.type);
    update.set(db_statement::description, s.description);
    update.set(db_statement::payee, s.payee);
    update.set(db_statement::payeeAccount, s.payeeAccount);
    update.set(db_statement::amount, s.amount);
    update.set(db_statement::category, s.category);
    update.set(db_statement::ruleId, s.ruleId);
    update.where(QString("%1 = %2").arg(db_statement::id).arg(s.id));
    data_base.execute(update);
}

void StatementMapper::insert(Statement &s) const
{
    SQLInsert insert{db_statement::statements};
    insert.set(db_statement::date, s.date.toInt());
    insert.set(db_statement::type, s.type);
    insert.set(db_statement::description, s.description);
    insert.set(db_statement::payee, s.payee);
    insert.set(db_statement::payeeAccount, s.payeeAccount);
    insert.set(db_statement::amount, s.amount);
    insert.set(db_statement::category, 0);
    insert.set(db_statement::ruleId, -1);

    data_base.execute(insert);
}

QVector<Statement> StatementMapper::getAll(const SQLCondition &cond)
{
    QVector<Statement> result;
    SQLiteStatement statement;

    SQLSelect select{db_statement::statements};
    select.field(db_statement::id);
    select.field(db_statement::date);
    select.field(db_statement::type);
    select.field(db_statement::description);
    select.field(db_statement::payee);
    select.field(db_statement::payeeAccount);
    select.field(db_statement::amount);
    select.field(db_statement::category);
    select.field(db_statement::ruleId);
    select.where(QString("%1 < 0").arg(db_statement::amount));
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
        row.ruleId = statement.GetInt(8);
        result.push_back(row);
    }
    return result;
}

TableStructure StatementMapper::getStructure()
{
    TableStructure statements{db_statement::statements};
    statements.addField(db_statement::id, SQLType::Integer().PK());
    statements.addField(db_statement::date, SQLType::Integer());
    statements.addField(db_statement::type, SQLType::Text());
    statements.addField(db_statement::description, SQLType::Text());
    statements.addField(db_statement::payee, SQLType::Text());
    statements.addField(db_statement::payeeAccount, SQLType::Text());
    statements.addField(db_statement::amount, SQLType::Real());
    statements.addField(db_statement::category, SQLType::Integer());
    statements.addField(db_statement::ruleId, SQLType::Integer());
    return statements;
}
