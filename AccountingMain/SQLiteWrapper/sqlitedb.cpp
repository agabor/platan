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

using namespace std;

void SQLiteDB::Prepare(SQLiteStatement &statement, SQLSelect &select)
{
    string s = select.toString();
    const char *select_query = s.c_str();
    statement.statement = QSqlQuery(select_query);
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
  : db(QSqlDatabase::addDatabase("QSQLITE"))
{

}

void SQLiteDB::SetPath(string data_base_path)
{
    this->data_base_path = data_base_path;
}

void SQLiteDB::Open()
{
    db.setDatabaseName(data_base_path.c_str());
    if (!db.open())
    {
        QString error_msg{db.lastError().text()};
        cerr << "Can't open database. " << error_msg.toStdString() << endl;
        Close();
        throw db_exception();
    }
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

