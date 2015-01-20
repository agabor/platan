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
void exportRules(QString file_name, QVector<std::shared_ptr<Rule> > rules);

#endif // RULEEXPORT_H
