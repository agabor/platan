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
#include "statement.h"


template <typename T>
class QVector;

class Rule
{
public:
    enum class Type
    {
        Is,
        Contains
    };

    Rule(int id, Statement::Column column, QString value, int category, Type type);
    Rule();

    bool apply(Statement &statement);

    int id() const;
    void setId(int m_value);

    Statement::Column column() const;
    void setColumn(const Statement::Column &m_value);

    QString value() const;
    void setValue(const QString &m_value);

    int category() const;
    void setCategory(int category);

    Type type() const;
    void setType(const Type &type);

private:
    int m_id;
    Statement::Column m_column;
    QString m_value;
    QString m_upperValue;
    bool m_hasUmlaut;
    QString m_withoutUmlautValue;
    QString m_convertedUmlautValue;
    int m_category;
    Type m_type;
};


#endif // RULE_H
