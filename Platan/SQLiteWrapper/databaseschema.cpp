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

#include <QStringList>
#include <QSqlQuery>
#include <QVariant>
#include <QString>
#include <QSqlDatabase>

#include <set>

#include "databaseschema.h"
#include "sqlselect.h"

using namespace std;

TableStructure DataBaseSchema::invalid = TableStructure::Invalid();

DataBaseSchema::DataBaseSchema()
{
}

void DataBaseSchema::addTable(TableStructure table)
{
    tables.push_back(table);
}

bool DataBaseSchema::isConform(const QSqlDatabase &db) const
{
    set<QString> application_tables;
    for(auto &t : tables)
        application_tables.insert(t.getName());

    QStringList table_names = db.tables();

    for(auto & table_name : table_names)
    {
        if (application_tables.find(table_name) == application_tables.end())
            continue;

        SQLSelect s{"sqlite_master"};
        s.field("sql");
        s.where(QString("name = \"%1\"").arg(table_name));
        QSqlQuery q(s.toString(), db);
        while(q.next())
        {
            TableStructure ts = TableStructure::fromSchema(q.value(0).toString());
            const TableStructure &table = getTable(table_name);
            if (table != ts)
                return false;
        }

        application_tables.erase(table_name);
    }

    return application_tables.empty();
}

const TableStructure &DataBaseSchema::getTable(QString name) const
{
    for (auto & table : tables)
    {
        if (table.getName() == name)
            return table;
    }
    return invalid;
}

void DataBaseSchema::createTables(const QSqlDatabase &db) const
{
    for (auto & table : tables)
        db.exec(table.sqlCommand());
}
