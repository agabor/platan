#include <QString>
#include <QVector>

#include <projectdb.h>
#include <rules.h>
#include <sqlitedb.h>
#include <rule.h>

ProjectDB::ProjectDB(Rules &rules, SQLiteDB &db)
    : rules(rules),
      db(db)
{
}

void ProjectDB::create(QString data_base_path)
{
    db.setPath(data_base_path);
    db.create();
}

void ProjectDB::insertRules(QVector<Rule> rulesToInsert)
{
    db.beginTransaction();
    rules.insertRules(rulesToInsert);
    db.endTransaction();
}

bool ProjectDB::open(QString project_path)
{
    if(db.isOpen())
        db.close();
    db.setPath(project_path);
    if(!db.open())
        return false;
    return true;
}
