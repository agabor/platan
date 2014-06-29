#ifndef COUNTRY_H
#define COUNTRY_H

#include <QString>
#include <QLocale>

template <typename T>
class QVector;

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
    static QVector<QLocale::Country> getAllCountries();
};

#endif // COUNTRY_H
