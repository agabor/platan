#ifndef COUNTRYMAPPER_H
#define COUNTRYMAPPER_H

#include <sqlitedb.h>
#include <QVector>

class Country;

class CountryMapper
{
public:
    CountryMapper(SQLiteDB &db) : data_base(db)
    {

    }

    QVector<Country> getAll();

private:
    SQLiteDB &data_base;
};

#endif // COUNTRYMAPPER_H
