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

#include <QVariant>

#include <statement.h>
#include <rule.h>

Statement::Statement()
{
  m_id = -1;
  m_ruleId = -1;
}

QVariant Statement::at(int idx) const
{
  return at(static_cast<Column>(idx));
}

QVariant Statement::at(Statement::Column idx) const
{
  switch (idx)
  {
  case Column::Date:
      return m_date;
  case Column::Type:
      return m_type;
  case Column::Description:
      return m_description;
  case Column::Payee:
      return m_payee;
  case Column::PayeeAccount:
      return m_payeeAccount;
  case Column::Amount:
      return m_amount;
  case Column::Category:
      return m_category;
  }
  return 0;
}
int Statement::id() const
{
    return m_id;
}

void Statement::setId(int id)
{
  m_id = id;
}

bool Statement::isIdInvalid() const
{
  return m_id == -1;
}
SQLiteDate Statement::date() const
{
    return m_date;
}

void Statement::setDate(const SQLiteDate &date)
{
    m_date = date;
}
QString Statement::type() const
{
    return m_type;
}

void Statement::setType(const QString &type)
{
    m_type = type;
}
QString Statement::description() const
{
    return m_description;
}

void Statement::setDescription(const QString &description)
{
    m_description = description;
}
QString Statement::payee() const
{
    return m_payee;
}

void Statement::setPayee(const QString &payee)
{
    m_payee = payee;
}
QString Statement::payeeAccount() const
{
    return m_payeeAccount;
}

void Statement::setPayeeAccount(const QString &payeeAccount)
{
    m_payeeAccount = payeeAccount;
}
float Statement::amount() const
{
    return m_amount;
}

void Statement::setAmount(float amount)
{
  m_amount = amount;
}

bool Statement::isAmountNaN() const
{
  return m_amount != m_amount;
}
int Statement::category() const
{
    return m_category;
}

void Statement::setCategory(int category)
{
    m_category = category;
}
int Statement::ruleId() const
{
    return m_ruleId;
}

void Statement::setRuleId(int ruleId)
{
  m_ruleId = ruleId;
}

void Statement::setToRule(const Rule &rule)
{
  m_ruleId = rule.id();
  m_category = rule.category();
}









