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

#include "accdatabase.h"
#include <iostream>
#include <string>
#include <sstream>
#include "datetransformation.h"
#include "sqlinsert.h"
#include "sqlupdate.h"
#include "sqlselect.h"

using namespace std;


AccDataBase::AccDataBase()
{
    time_interval_set = false;
}

AccDataBase::~AccDataBase()
{
    SQLiteDB::getInstance().Close();
}

void AccDataBase::setPath(QString data_base_path)
{
    auto data_base = SQLiteDB::getInstance();
    data_base.SetPath(data_base_path);
    if(data_base.isOpen())
        data_base.Close();
    data_base.Open();
}

void AccDataBase::create(QString data_base_path)
{
    auto data_base = SQLiteDB::getInstance();
    setPath(data_base_path);
    data_base.Create();
    data_base.ExecuteScript("../rules.sql");
}

void AccDataBase::insertData(StatementTableModel &model)
{
    auto data_base = SQLiteDB::getInstance();
    data_base.BeginTransaction();

    const int row_count = model.rowCount();
    for(int i = 0; i < row_count; ++i)
    {
        Statement row = model.row(i);
        SQLInsert insert("Statements");
        insert.set("Date", row.Date.toInt());
        insert.set("Type", row.Type);
        insert.set("Description", row.Description);
        insert.set("Payee", row.Payee);
        insert.set("PayeeAccount", row.PayeeAccount);
        insert.set("Amount", row.Amount);
        insert.set("Class", 0);

        data_base.Execute(insert);
    }
    data_base.EndTransaction();
}


void AccDataBase::readData(QVector<Statement> &model)
{
    model.clear();
    SQLiteStatement statement;

    SQLSelect select{"statements"};
    select.field("ID");
    select.field("Date");
    select.field("Type");
    select.field("Description");
    select.field("Payee");
    select.field("PayeeAccount");
    select.field("Amount");
    select.field("Class");
    select.where("Amount < 0");
    setTimeInterval(select);

    auto data_base = SQLiteDB::getInstance();
    data_base.Prepare(statement, select);

    while (data_base.Step(statement))
    {
        Statement row;
        row.id = statement.GetInt(0);
        row.Date = statement.GetDate(1);
        row.Type = statement.GetText(2);
        row.Description = statement.GetText(3);
        row.Payee = statement.GetText(4);
        row.PayeeAccount = statement.GetText(5);
        row.Amount = statement.GetDouble(6);
        row.Class = statement.GetInt(7);
        model.push_back(row);
    }
}

void AccDataBase::getCalssification(QMap<int, float> &result)
{
    result.clear();

    classify();

    SQLiteStatement statement;

    SQLSelect select{"statements"};
    select.field("SUM(Amount) as Sum");
    select.field("Class");
    select.where("Amount < 0");
    setTimeInterval(select);
    select.groupBy("Class");

    auto data_base = SQLiteDB::getInstance();
    data_base.Prepare(statement, select);

    while (data_base.Step(statement))
    {
        int sum = statement.GetDouble(0);
        int Class = statement.GetInt(1);
        result.insert(Class, sum);
    }
}

void AccDataBase::classify()
{
    auto data_base = SQLiteDB::getInstance();
    data_base.BeginTransaction();

    for(Rule &rule : Rule::getAll(4))
    {
        SQLUpdate update("statements");
        update.set("Class", rule.category);
        update.where("Class = 0");
        update.where(QString("PayeeAccount = \"%1\"").arg(rule.value));
        data_base.Execute(update);
    }

    for(Rule &rule : Rule::getAll(1))
    {
        SQLUpdate update("statements");
        update.set("Class", rule.category);
        update.where("Class = 0");
        update.where(QString("Type = \"%1\"").arg(rule.value));
        data_base.Execute(update);
    }

    data_base.EndTransaction();
}

void AccDataBase::setTimeInterval(SQLSelect &select)
{
    if (time_interval_set)
    {
        select.where(QString("Date >= %1").arg(start_date.toInt()));
        select.where(QString("Date <= %1").arg(end_date.toInt()));
    }
}


void AccDataBase::setCategory(Statement &row, int category)
{
    SQLUpdate update("statements");
    update.set("Class", category);
    update.where(QString("ID = %1").arg(row.id));
    SQLiteDB::getInstance().Execute(update);
}

void AccDataBase::setTimeInterval(QDate start_date, QDate end_date)
{
    this->start_date = start_date;
    this->end_date = end_date;
    time_interval_set = true;
}

void AccDataBase::unsetTimeInterval()
{
    time_interval_set = false;
}


