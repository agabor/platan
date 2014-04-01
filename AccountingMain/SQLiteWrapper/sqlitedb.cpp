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
#include "sqlite3.h"
#include <string>

using namespace std;

void SQLiteDB::Prepare(SQLiteStatement &statement, SQLSelect &select)
{
    string s = select.toString();
    const char *select_query = s.c_str();
    int retval = sqlite3_prepare_v2(db, select_query, -1, &statement.statement, 0);

    if(retval)
    {
        cerr << select_query << endl;
        cerr << "Selecting data from DB Failed (err_code="<< retval <<")" << endl;
        return;
    }
}

bool SQLiteDB::Step(SQLiteStatement &statement)
{
    return sqlite3_step(statement.statement) == SQLITE_ROW;
}

void SQLiteDB::Finalize(SQLiteStatement &statement)
{
    sqlite3_finalize(statement.statement);
}

void SQLiteDB::SetPath(string data_base_path)
{
    this->data_base_path = data_base_path;
}

void SQLiteDB::Open()
{
    int rc = sqlite3_open(data_base_path.c_str(), &db);
    if (rc)
    {
        cerr << "Can't open database: %s\n" << sqlite3_errmsg(db);
        sqlite3_close(db);
        throw db_exception();
    }
}

void SQLiteDB::Close()
{
    sqlite3_close(db);
}

void SQLiteDB::Execute(SQLQuery &query)
{
    char *zErrMsg = 0;
    string s = query.toString();
    const char *query_string = s.c_str();
    int rc = sqlite3_exec(db, query_string, 0, 0, &zErrMsg);
    if(rc != SQLITE_OK)
    {
        cerr << query_string << "\nSQL error: %s" << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
}

void SQLiteDB::BeginTransaction()
{
    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
}

void SQLiteDB::EndTransaction()
{
    sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
}

