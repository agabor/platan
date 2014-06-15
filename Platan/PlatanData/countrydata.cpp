#include "countrydata.h"
#include "countrycodes.h"

QVector<QLocale::Country> CountryData::getAllCountries()
{
    QVector<QLocale::Country> result;
    for (int i = 1; i <= QLocale::LastCountry; ++i)
    {
        QLocale::Country c = (QLocale::Country)i;
        if (c != QLocale::Antarctica)
            result.push_back(c);
    }
    return result;
}

CountryData::CountryData()
{

}

CountryData::CountryData(QLocale::Country c)
{
    name = QLocale::countryToString(c);
    for (int i = 1; i < name.length(); ++ i)
        if (name.at(i).isUpper())
            name.insert(i++, " ");
    code = getCountryCode(c).toLower();
}


bool CountryData::operator==(const CountryData &other) const
{
    return name == other.name;
}

bool CountryData::operator!=(const CountryData &other) const
{
    return name == other.name;
}

bool CountryData::operator<(const CountryData &other) const
{
    return name < other.name;
}

bool CountryData::operator>(const CountryData &other) const
{
    return name > other.name;
}

bool CountryData::operator<=(const CountryData &other) const
{
    return name <= other.name;
}

bool CountryData::operator>=(const CountryData &other) const
{
    return name >= other.name;
}
