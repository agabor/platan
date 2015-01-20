#ifndef RULEEXPORT_H
#define RULEEXPORT_H

#include <QString>

class SQLInsert;
class Rule;

namespace db_rule
{
const QString rules = "Rules";
const QString column = "Column";
const QString id = "Id";
const QString value = "Value";
const QString category = "Category";
const QString type = "Type";
const QString country = "Country";
}

SQLInsert getInsertQuery(Rule &r);

#endif // RULEEXPORT_H
