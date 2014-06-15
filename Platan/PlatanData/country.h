#ifndef COUNTRY_H
#define COUNTRY_H

#include <QString>

class Country
{
public:
    Country() = default;
    Country(int id, QString code) : id(id), code(code)
    {

    }

    int id;
    QString code;
};

#endif // COUNTRY_H
