#include "countrymapper.h"
#include "country.h"

QVector<Country> CountryMapper::getAll()
{
    SQLSelect select{"countries"};
    select.field("ID");
    select.field("Code");
    SQLiteStatement statement;
    data_base.prepare(statement, select);

    QVector<Country> result;

    while (data_base.step(statement))
    {
        result.push_back(Country(statement.GetInt(0), statement.GetText(1)));
    }

    return result;
}
