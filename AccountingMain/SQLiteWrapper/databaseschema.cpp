#include "databaseschema.h"
#include <set>
#include <QStringList>
#include "sqlselect.h"
#include <QSqlQuery>
#include <QVariant>

using namespace std;

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
        string query{s.toString()};
        QSqlQuery q(query.c_str());
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
    return TableStructure::Invalid();
}
