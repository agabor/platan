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

Rule::Rule(int _column, QString _value, int _category)
    :column(_column), value(_value), category(_category)
{
}

Rule::Rule()
{
}

bool Rule::apply(Statement &statement)
{

    if (statement.at(column) == value)
    {
        statement.category = category;
        return true;
    }
    return false;
}

