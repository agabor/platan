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
    IdentityTransFormation(int column, bool remove_quot)
        : Transformation(column, remove_quot)
    {

    }

    QString convert(QString Data) const
    {
        return Data;
    }
};

class IntTransformation : public Transformation<int>
{
public:
    IntTransformation(int column, bool remove_quot)
        : Transformation(column, remove_quot)
    {

    }

    int convert(QString Data) const
    {
        return Data.toInt();
    }
};

class FloatTransformation : public Transformation<float>
{
public:
    FloatTransformation(int column, char decimal, bool remove_quot)
        : Transformation(column, remove_quot), decimal(decimal)
    {

    }

    float convert(QString Data) const
    {
        QStringList string_list = Data.split(decimal);
        const int length = string_list.length();

        if (length == 0)
            return 0;

        const int w = string_list.at(0).toInt();
        if (length == 1)
            return (float)w;

        const QString frac_string = string_list.at(1);
        const int frac = frac_string.toInt();

        const int decimal_places = frac_string.length();

        float frac_mul = 1;
        for (int i = 0; i < decimal_places; ++i)
            frac_mul /= 10;

        return w + (w < 0 ? -1 : 1) * frac * frac_mul;
    }

protected:
    char decimal;
};

#endif // SIMPLETRANSFORMATIONS_H
