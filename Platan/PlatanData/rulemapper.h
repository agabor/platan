#ifndef RULEMAPPER_H
#define RULEMAPPER_H

template <typename T>
class QVector;
class SQLiteDB;
class Rule;
class TableStructure;

class RuleMapper
{
public:
    RuleMapper(SQLiteDB &db);
    void insert(Rule &r) const;
    QVector<Rule> getAll(int country = -1) const;
    static TableStructure getStructure();
    static TableStructure getStructureWithCountry();
private:
    SQLiteDB &data_base;
};

#endif // RULEMAPPER_H
