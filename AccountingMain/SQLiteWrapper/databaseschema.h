#ifndef DATABASESCHEMA_H
#define DATABASESCHEMA_H

#include "tablestructure.h"
#include <vector>
#include <QString>
#include <QSqlDatabase>

class DataBaseSchema
{
public:
    DataBaseSchema();
    void addTable(TableStructure table);
    bool isConform(const QSqlDatabase &db) const;
    const TableStructure &getTable(QString name) const;
private:
    std::vector<TableStructure> tables;
};

#endif // DATABASESCHEMA_H
