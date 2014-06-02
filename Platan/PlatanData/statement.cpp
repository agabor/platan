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

#include "statement.h"
#include <sqlupdate.h>
#include <sqlinsert.h>
#include <sqlitedb.h>
#include <QVector>

QVariant Statement::at(int idx) const
{
    switch (idx)
    {
    case 0:
        return date;
    case 1:
        return type;
    case 2:
        return description;
    case 3:
        return payee;
    case 4:
        return payeeAccount;
    case 5:
        return amount;
    }
    return 0;
}

void Statement::update() const
{
    SQLUpdate update("statements");
    update.set("Date", date.toInt());
    update.set("Type", type);
    update.set("Description", description);
    update.set("Payee", payee);
    update.set("PayeeAccount", payeeAccount);
    update.set("Amount", amount);
    update.set("Class", category);
    update.where(QString("ID = %1").arg(id));
    SQLiteDB::getInstance().Execute(update);
}

void Statement::insert() const
{
    SQLInsert insert{"Statements"};
    insert.set("Date", date.toInt());
    insert.set("Type", type);
    insert.set("Description", description);
    insert.set("Payee", payee);
    insert.set("PayeeAccount", payeeAccount);
    insert.set("Amount", amount);
    insert.set("Class", 0);

    SQLiteDB::getInstance().Execute(insert);
}

QVector<Statement> Statement::getAll(const SQLCondition &cond)
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

    auto data_base = SQLiteDB::getInstance();
    data_base.Prepare(statement, select);

    while (data_base.Step(statement))
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
