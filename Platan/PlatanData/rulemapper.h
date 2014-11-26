#ifndef RULEMAPPER_H
#define RULEMAPPER_H

#include <rule.h>

template <typename T>
class QVector;
class SQLiteDB;
class TableStructure;

class RuleMapper
{
public:
    RuleMapper(SQLiteDB &db);
    void insert(Rule &r) const;
    void remove(Rule &r);
    void update(Rule &r);
    QVector<Rule> getAll(int _country = -1) const;
    static TableStructure getStructure();
    static TableStructure getStructureWithCountry();
private:
    SQLiteDB &data_base;
};

#endif // RULEMAPPER_H
