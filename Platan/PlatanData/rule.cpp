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

#include <rule.h>
#include <statement.h>
#include <QVariant>

Rule::Rule(int _id, int _column, QString _value, int _category, Type _type)
    :id(_id), column(_column), value(_value.toUpper()), category(_category), type(_type)
{
}

Rule::Rule()
{
}

bool Rule::apply(Statement &statement)
{

    switch (type)
    {
    case Type::Is:
        if (statement.at(column) == value)
        {
            statement.category = category;
            statement.ruleId = id;
            return true;
        }
        break;
    case Type::Contains:
        if (statement.at(column).toString().toUpper().contains(value))
        {
            statement.category = category;
            statement.ruleId = id;
            return true;
        }
        break;
    }
    return false;
}

