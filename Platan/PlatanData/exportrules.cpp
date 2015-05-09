#include <QFile>
#include <QTextStream>
#include <QSharedPointer>

#include <sqlinsert.h>
#include <rule.h>

#include "exportrules.h"

SQLInsert getInsertQuery(Rule &r)
{
    SQLInsert insert(db_rule::rules);
    insert.set(db_rule::column, r.column);
    insert.set(db_rule::value, r.value);
    insert.set(db_rule::category, r.category);
    insert.set(db_rule::type, static_cast<int>(r.type));

    return insert;
}


void exportRules(QString file_name, QVector<QSharedPointer<Rule> > rules)
{
    QFile out(file_name);
    out.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out_stream(&out);

    for (auto rule : rules)
        out_stream << getInsertQuery(*rule).toString() << ';' << endl;

    out.close();
}
