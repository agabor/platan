#ifndef RULEMAPPER_H
#define RULEMAPPER_H

#include <sqlitedb.h>
#include <rule.h>



class RuleMapper
{
public:
    RuleMapper(SQLiteDB &db);
    void insert(Rule &r) const;
    QVector<Rule> getAll(int column = -1);
    static TableStructure getStructure();
private:
    SQLiteDB &data_base;
};

#endif // RULEMAPPER_H
