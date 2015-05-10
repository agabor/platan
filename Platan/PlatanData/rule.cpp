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

Rule::Rule(int _id, Statement::Column _column, QString _value, int _category, Type _type)
    :m_id(_id), m_column(_column), m_value(_value.toUpper()), m_category(_category), m_type(_type)
{
}

Rule::Rule()
{
}

bool Rule::apply(Statement &statement)
{

    switch (m_type)
    {
    case Type::Is:
        if (statement.at(m_column) == m_value)
        {
            statement.category = m_category;
            statement.ruleId = m_id;
            return true;
        }
        break;
    case Type::Contains:
        if (statement.at(m_column).toString().toUpper().contains(m_value))
        {
            statement.category = m_category;
            statement.ruleId = m_id;
            return true;
        }
        break;
    }
    return false;
}
int Rule::id() const
{
  return m_id;
}

void Rule::setId(int value)
{
  m_id = value;
}
Statement::Column Rule::column() const
{
  return m_column;
}

void Rule::setColumn(const Statement::Column &value)
{
  m_column = value;
}
QString Rule::value() const
{
  return m_value;
}

void Rule::setValue(const QString &value)
{
  m_value = value;
}
int Rule::category() const
{
  return m_category;
}

void Rule::setCategory(int category)
{
  m_category = category;
}

Rule::Type Rule::type() const
{
  return m_type;
}

void Rule::setType(const Type &type)
{
  m_type = type;
}






