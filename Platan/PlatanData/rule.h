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

#ifndef RULE_H
#define RULE_H

#include <QString>

template <typename T>
class QVector;

class Rule
{
public:
    Rule(int _column, QString _value, int _category);
    Rule();

    int column;
    QString value;
    int category;

    void insert() const;
    static QVector<Rule> getAll(int column = -1);
};


#endif // RULE_H