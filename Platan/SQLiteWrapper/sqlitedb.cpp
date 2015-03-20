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
#include <QFile>
#include <set>
#include <assert.h>
#include "tablestructure.h"

using namespace std;


void SQLiteDB::prepare(SQLiteStatement &statement, SQLSelect &select)
{
    statement.SetStatement(select.toString(), db);
}

bool SQLiteDB::isOpen() const
{
    return is_open;
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

bool SQLiteDB::step(SQLiteStatement &statement)
{
    return statement.statement.next();
}

SQLiteDB::SQLiteDB(DataBaseSchema schema, const QString & connectionName)
  : db(QSqlDatabase::addDatabase("QSQLITE", connectionName)),
    is_open(false),
    schema(schema)
{
}

void SQLiteDB::setPath(QString data_base_path)
{
    this->data_base_path = data_base_path;
}

bool SQLiteDB::open()
{
    connect();
    is_open = isDatabaseValid();
    if (!is_open)
        close();
    return is_open;
}

void SQLiteDB::close()
{
    db.close();
}

void SQLiteDB::create()
{
    connect();
    schema.createTables(db);
}

void SQLiteDB::connect()
{
    assert(!is_open);
    db.setDatabaseName(data_base_path);
    bool o = db.open();
    if (!o || db.isOpenError())
    {
        QString error_msg{db.lastError().text()};
        cerr << "Can't open database. " << error_msg.toStdString() << endl;
        close();
        throw db_exception();
    }
}

void SQLiteDB::execute(QString query_str)
{
    QSqlQuery qry(db);

    if( !qry.exec(query_str) )
    {
        QString error_msg{db.lastError().text()};
        cerr << error_msg.toStdString() << endl;
        assert(false);
    }
}

void SQLiteDB::execute(SQLQuery &query)
{
    execute(query.toString());
}

void SQLiteDB::executeScript(QString filename)
{
    QFile scriptFile(filename);
    if (scriptFile.open(QIODevice::ReadOnly))
    {
        QStringList scriptQueries = QTextStream(&scriptFile).readAll().split(';');
        beginTransaction();
        for (QString insert : scriptQueries)
        {
            string query = insert.toStdString();
            execute(query.c_str());
        }
        endTransaction();
    }
}

void SQLiteDB::beginTransaction()
{
    db.transaction();
}

void SQLiteDB::endTransaction()
{
    db.commit();
}

