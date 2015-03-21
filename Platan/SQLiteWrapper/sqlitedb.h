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

#include "sqlitestatement.h"
#include "sqlquery.h"
#include "sqlselect.h"
#include <QSqlDatabase>
#include "databaseschema.h"

class SQLiteDB
{
public:
    SQLiteDB(DataBaseSchema schema, const QString &connectionName);
    void setPath(QString data_base_path);
    bool open();
    void close();
    void create();
    void beginTransaction();
    void endTransaction();
    void execute(SQLQuery &query);
    void executeScript(QString filename);
    bool step(SQLiteStatement &statement);
    void prepare(SQLiteStatement &statement, SQLSelect &select);
    bool isOpen() const;
private:

    void execute(QString query_str);
    void connect();
    QSqlDatabase db;
    QString data_base_path;
    bool is_open;
    bool isDatabaseValid() const;
    DataBaseSchema schema;
};

#endif // SQLITEDB_H
