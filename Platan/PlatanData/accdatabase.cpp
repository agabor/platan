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

inline string stringify(float x)
{
    ostringstream o;
    o << x;
    return o.str();
}

AccDataBase::AccDataBase()
{
    time_interval_set = false;
}

AccDataBase::~AccDataBase()
{
    data_base.Close();
}

void AccDataBase::SetPath(QString data_base_path)
{
    data_base.SetPath(data_base_path);
    if(data_base.isOpen())
        data_base.Close();
    data_base.Open();
}

void AccDataBase::Create(QString data_base_path)
{
    SetPath(data_base_path);
    data_base.Create();
    data_base.ExecuteScript("../rules.sql");
}

void AccDataBase::InsertData(StatementTableModel &model)
{
    data_base.BeginTransaction();

    const int row_count = model.rowCount();
    for(int i = 0; i < row_count; ++i)
    {
        StatementRow row = model.row(i);
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


void AccDataBase::ReadData(std::vector<StatementRow> &model, bool only_unclassified)
{
    SQLiteStatement statement;

    SQLSelect select{"statements"};
    select.field("Date");
    select.field("Type");
    select.field("Description");
    select.field("Payee");
    select.field("PayeeAccount");
    select.field("Amount");
    select.field("Class");
    select.where("Amount < 0");
    if (only_unclassified)
        select.where("Class = 0");
    SetTimeInterval(select);

    data_base.Prepare(statement, select);

    while (data_base.Step(statement))
    {
        StatementRow row;
        row.Date = statement.GetDate(0);
        row.Type = statement.GetText(1);
        row.Description = statement.GetText(2);
        row.Payee = statement.GetText(3);
        row.PayeeAccount = statement.GetText(4);
        row.Amount = statement.GetDouble(5);
        row.Class = statement.GetInt(6);
        model.push_back(row);
    }

    data_base.Finalize(statement);
}

void AccDataBase::GetCalssification(QMap<int, float> &result)
{
    result.clear();

    Classify();

    SQLiteStatement statement;

    SQLSelect select{"statements"};
    select.field("SUM(Amount) as Sum");
    select.field("Class");
    select.where("Amount < 0");
    SetTimeInterval(select);
    select.groupBy("Class");

    data_base.Prepare(statement, select);

    while (data_base.Step(statement))
    {
        int sum = statement.GetDouble(0);
        int Class = statement.GetInt(1);
        result.insert(Class, sum);
    }

    data_base.Finalize(statement);
}

void AccDataBase::Classify()
{
    SQLiteStatement statement;

    data_base.BeginTransaction();

    SQLSelect select{"rules"};
    select.field("Value");
    select.field("Class");
    select.where("Type = 0");

    data_base.Prepare(statement, select);

    while (data_base.Step(statement))
    {
        SQLUpdate update("statements");
        update.set("Class", statement.GetInt(1));
        update.where("Class = 0");
        update.where(QString("PayeeAccount = \"%1\"").arg(statement.GetText(0)));
        data_base.Execute(update);
    }

    select = SQLSelect {"rules"};
    select.field("Value");
    select.field("Class");
    select.where("Type = 1");

    data_base.Prepare(statement, select);

    while (data_base.Step(statement))
    {
        SQLUpdate update("statements");
        update.set("Class", statement.GetInt(1));
        update.where("Class = 0");
        update.where(QString("Type = \"%2\"").arg(statement.GetText(0)));
        data_base.Execute(update);
    }

    data_base.Finalize(statement);
    data_base.EndTransaction();
}

void AccDataBase::SetTimeInterval(SQLSelect &select)
{
    if (time_interval_set)
    {
        select.where(QString("Date >= %1").arg(start_date.toInt()));
        select.where(QString("Date <= %1").arg(end_date.toInt()));
    }
}


void AccDataBase::InsertRule(int type, int column, QString value, int Class, QString name)
{
    data_base.BeginTransaction();

    SQLInsert insert("rules");
    insert.set("Type", type);
    insert.set("Column", column);
    insert.set("Value", value);
    insert.set("Class", Class);
    insert.set("Name", name);

    data_base.Execute(insert);

    data_base.EndTransaction();
}

void AccDataBase::SetTimeInterval(QDate start_date, QDate end_date)
{
    this->start_date = start_date;
    this->end_date = end_date;
    time_interval_set = true;
}

void AccDataBase::UnsetTimeInterval()
{
    time_interval_set = false;
}
