#ifndef PROJECTDB_H
#define PROJECTDB_H

class QString;
template <typename T>
class QVector;

class Rules;
class SQLiteDB;
class Rule;

class ProjectDB
{
public:
    ProjectDB(Rules &rules, SQLiteDB &db);
    void create(QString data_base_path);
    void insertRules(QVector<Rule> rules);
    bool open(QString project_path);
    void close();
private:
    Rules &rules;
    SQLiteDB &db;
};

#endif // PROJECTDB_H
