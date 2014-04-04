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

#include "sqlitedb.h"
#include <iostream>
#include <string>
#include <QSqlError>
#include <QStringList>
#include <QDebug>
#include <set>
#include <assert.h>
#include "tablestructure.h"

using namespace std;

void SQLiteDB::Prepare(SQLiteStatement &statement, SQLSelect &select)
{
    string s = select.toString();
    const char *select_query = s.c_str();
    statement.statement = QSqlQuery(select_query);
}

bool isSQLiteDB(const QSqlDatabase &db)
{
    QStringList system_tables = db.tables(QSql::SystemTables);

    if(system_tables.length() != 1)
        return false;

    if(system_tables.at(0) != "sqlite_master")
        return false;

    return true;
}

bool SQLiteDB::isDatabaseValid() const
{
    if(!isSQLiteDB(db))
        return false;

    return schema.isConform(db);
}

void SQLiteDB::initSchema()
{
    TableStructure classes{"classes"};
    classes.addField("En", "TEXT");
    classes.addField("De", "TEXT");
    classes.addField("Hu", "TEXT");
    classes.addField("ID", "INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL");
    schema.addTable(classes);

    TableStructure payee{"payee"};
    payee.addField("ID", "INTEGER PRIMARY KEY");
    payee.addField("Name", "TEXT");
    payee.addField("Account", "TEXT");
    payee.addField("Type", "NUMERIC");
    schema.addTable(payee);

    TableStructure rules{"rules"};
    rules.addField("Payee", "INTEGER");
    rules.addField("Type", "INTEGER");
    rules.addField("Column", "INTEGER");
    rules.addField("Value", "TEXT");
    rules.addField("Class", "INTEGER");
    schema.addTable(rules);

    TableStructure statements{"statements"};
    statements.addField("ID", "INTEGER PRIMARY KEY");
    statements.addField("Date", "INTEGER");
    statements.addField("Type", "TEXT");
    statements.addField("Description", "TEXT");
    statements.addField("Payee", "TEXT");
    statements.addField("PayeeAccount", "TEXT");
    statements.addField("Amount", "REAL");
    statements.addField("Class", "INTEGER");
    schema.addTable(statements);
}

bool SQLiteDB::Step(SQLiteStatement &statement)
{
    return statement.statement.next();
}

void SQLiteDB::Finalize(SQLiteStatement &statement)
{
    //sqlite3_finalize(statement.statement);
}

SQLiteDB::SQLiteDB()
  : db(QSqlDatabase::addDatabase("QSQLITE")),
    is_open(false)
{
    initSchema();
}

void SQLiteDB::SetPath(string data_base_path)
{
    this->data_base_path = data_base_path;
}

void SQLiteDB::Open()
{
    assert(!is_open);
    db.setDatabaseName(data_base_path.c_str());
    if (!db.open() || db.isOpenError())
    {
        QString error_msg{db.lastError().text()};
        cerr << "Can't open database. " << error_msg.toStdString() << endl;
        Close();
        throw db_exception();
    }

    is_open = isDatabaseValid();
    if (!is_open)
        Close();
}

void SQLiteDB::Close()
{
    db.close();
}

void SQLiteDB::Execute(SQLQuery &query)
{
    QSqlQuery qry = db.exec(query.toString().c_str());

    if( !qry.exec() )
    {
        QString error_msg{db.lastError().text()};
        cerr << error_msg.toStdString() << endl;
    }
}

void SQLiteDB::BeginTransaction()
{
    db.transaction();
}

void SQLiteDB::EndTransaction()
{
    db.commit();
}

