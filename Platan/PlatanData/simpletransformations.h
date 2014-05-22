// This file is part of Platan.
// Copyright (C) 2014 Gábor Angyal
//
// Platan is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Platan is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Platan.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SIMPLETRANSFORMATIONS_H
#define SIMPLETRANSFORMATIONS_H

#include "transformation.h"
#include <QString>
#include <QStringList>

class IdentityTransFormation : public Transformation<QString>
{
public:
    IdentityTransFormation()
    {

    }

    QString convert(QString Data) const
    {
        return Data;
    }

    QString getErrorMessage() const
    {
        return QString();
    }
};

class IntTransformation : public Transformation<int>
{
public:
    IntTransformation()
    {

    }

    int convert(QString data) const
    {
        bool ok;
        int result = data.toInt(&ok);
        if (!ok)
            errorList.push_back(data);
        return result;
    }

    QString getErrorMessage() const
    {
        QString result= QObject::tr("The following values do not look like integers: ");
        return result + errorListString();
    }
};

class FloatTransformation : public Transformation<float>
{
public:
    FloatTransformation()
    {

    }

    float convert(QString data) const
    {
        QStringList string_list = data.split(decimal);
        const int length = string_list.length();

        bool error_found = false;
        if (length == 0)
        {
            errorList.push_back(data);
            error_found = true;
            return 0;
        }

        bool ok;
        const int w = string_list.at(0).toInt(&ok);

        if (!ok && !error_found)
        {
            errorList.push_back(data);
            error_found = true;
        }

        if (length == 1)
            return (float)w;

        const QString frac_string = string_list.at(1);
        const int frac = frac_string.toInt(&ok);

        if (!ok && !error_found)
        {
            errorList.push_back(data);
            error_found = true;
        }

        const int decimal_places = frac_string.length();

        float frac_mul = 1;
        for (int i = 0; i < decimal_places; ++i)
            frac_mul /= 10;

        return w + (w < 0 ? -1 : 1) * frac * frac_mul;
    }

    char getDecimal() const
    {
        return decimal;
    }
    void setDecimal(char value)
    {
        decimal = value;
    }

    QString getErrorMessage() const
    {
        QString result = QObject::tr("The following values do not look like real numbers: ");
        return result + errorListString();
    }

protected:
    char decimal;
};


#endif // SIMPLETRANSFORMATIONS_H

