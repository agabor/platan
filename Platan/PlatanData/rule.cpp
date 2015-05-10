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

Rule::Rule(int id, Statement::Column column, QString value, int category, Type type)
    :m_id(id), m_column(column), m_category(category), m_type(type)
{
  setValue(value);
}

Rule::Rule()
{
}

bool Rule::apply(Statement &statement)
{
  QString stm_value{statement.at(m_column).toString()};
    switch (m_type)
    {
    case Type::Is:
        if (stm_value == m_value)
        {
            statement.category = m_category;
            statement.ruleId = m_id;
            return true;
        }
        break;
    case Type::Contains:
        QString stm_upper_value = stm_value.toUpper();
        if (stm_upper_value.contains(m_upperValue))
        {
            statement.category = m_category;
            statement.ruleId = m_id;
            return true;
        }
        if (!m_hasUmlaut)
          return false;

        if (stm_upper_value.contains(m_convertedUmlautValue))
        {
            statement.category = m_category;
            statement.ruleId = m_id;
            return true;
        }

        if (stm_upper_value.contains(m_withoutUmlautValue))
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

const QString auml = QString::fromUtf8("Ä");
const QString auml_c{"AE"};
const QString auml_p{"A"};

const QString ouml = QString::fromUtf8("Ö");
const QString ouml_c{"OE"};
const QString ouml_p{"O"};

const QString uuml = QString::fromUtf8("Ü");
const QString uuml_c{"UE"};
const QString uuml_p{"U"};

bool containsUmlaut(const QString &value)
{
  return value.contains(auml) || value.contains(ouml) || value.contains(uuml);
}

void removeUmlaut(QString &value)
{
  value.replace(auml, auml_p);
  value.replace(ouml, ouml_p);
  value.replace(uuml, uuml_p);
}

void convertUmlaut(QString &value)
{
  value.replace(auml, auml_c);
  value.replace(ouml, ouml_c);
  value.replace(uuml, uuml_c);
}

void Rule::setValue(const QString &value)
{
  m_value = value;
  m_upperValue = value.toUpper();
  m_hasUmlaut = containsUmlaut(m_upperValue);
  if (m_hasUmlaut)
  {
    m_convertedUmlautValue = m_upperValue;
    convertUmlaut(m_convertedUmlautValue);

    m_withoutUmlautValue = m_upperValue;
    removeUmlaut(m_withoutUmlautValue);
  }
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






