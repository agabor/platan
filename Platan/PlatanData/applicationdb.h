#ifndef APPLICATIONDB_H
#define APPLICATIONDB_H

#include <countrymapper.h>
#include <rulemapper.h>
#include <sqlitedb.h>

template <typename T>
class QVector;

class ApplicationDB
{
public:
    ApplicationDB();
    QVector<Country> getCountries();
    void init();
    QVector<Rule> getRulesForCountry(int country);
    void close();
private:
    static DataBaseSchema getDBSchema();
    SQLiteDB dataBase;
    CountryMapper countryMapper;
    RuleMapper ruleMapper;
};

#endif // APPLICATIONDB_H
