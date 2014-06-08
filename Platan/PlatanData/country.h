#ifndef COUNTRY_H
#define COUNTRY_H

#include <QString>
#include <QLocale>
#include <QVector>

class CountryData
{
public:
    CountryData();
    CountryData(QLocale::Country c);

    QString name;
    QString code;

    bool operator==(const CountryData &other) const;
    bool operator!=(const CountryData &other) const;
    bool operator<(const CountryData &other) const;
    bool operator>(const CountryData &other) const;
    bool operator<=(const CountryData &other) const;
    bool operator>=(const CountryData &other) const;
};

QVector<QLocale::Country> getAllCountries();

#endif // COUNTRY_H
