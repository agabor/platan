#include "applicationdb.h"

ApplicationDB::ApplicationDB()
    : dataBase(getDBSchema(), "app"),
      countryMapper(dataBase),
      ruleMapper(dataBase)
{
}

QVector<Country> ApplicationDB::getCountries()
{
    return countryMapper.getAll();
}

void ApplicationDB::init()
{
    dataBase.setPath("platandata");
    dataBase.open();
}

QVector<Rule> ApplicationDB::getRulesForCountry(int country)
{
    return ruleMapper.getAll(country);
}

void ApplicationDB::close()
{
    dataBase.close();
}

DataBaseSchema ApplicationDB::getDBSchema()
{
    DataBaseSchema schema;
    schema.addTable(RuleMapper::getStructureWithCountry());

    TableStructure countries{"countries"};
    countries.addField("ID", SQLType::Integer().PK());
    countries.addField("Code", SQLType::Text());
    schema.addTable(countries);
    return schema;
}
