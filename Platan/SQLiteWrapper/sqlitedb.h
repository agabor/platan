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

#ifndef SQLITEDB_H
#define SQLITEDB_H

#include <string>
#include <QString>
#include <exception>
#include <QDate>
#include "sqlitestatement.h"
#include "sqlquery.h"
#include "sqlselect.h"
#include <QSqlDatabase>
#include "databaseschema.h"
#include <memory>

class db_exception : public std::exception
{
public:
    db_exception(){}
};


class SQLiteDB
{
public:
    static SQLiteDB &getInstance();
    void SetPath(QString data_base_path);
    void Open();
    void Close();
    void Create();
    void BeginTransaction();
    void EndTransaction();
    void Execute(SQLQuery &query);
    void ExecuteScript(QString filename);
    bool Step(SQLiteStatement &statement);
    void Prepare(SQLiteStatement &statement, SQLSelect &select);
    bool isOpen() const;
private:
    SQLiteDB();
    void Execute(QString query_str);
    void connect();
    QSqlDatabase db;
    QString data_base_path;
    bool is_open;
    bool isDatabaseValid() const;
    DataBaseSchema schema;
    void initSchema();
    static SQLiteDB instance;
};

#endif // SQLITEDB_H
