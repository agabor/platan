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

#ifndef STATEMENT_H
#define STATEMENT_H

#include <QString>
#include <sqlitedate.h>

class QVariant;
class Rule;


class Statement
{
public:
  Statement();
  enum class Column
  {
    Date = 0,
    Type = 1,
    Description = 2,
    Payee = 3,
    PayeeAccount = 4,
    Amount = 5,
    Category = 6,
    RuleId = 7,
  };

  QVariant at(int idx) const;
  QVariant at(Column idx) const;

  int id() const;
  void setId(int id);
  bool isIdInvalid() const;

  SQLiteDate date() const;
  void setDate(const SQLiteDate &date);

  QString type() const;
  void setType(const QString &type);

  QString description() const;
  void setDescription(const QString &description);

  QString payee() const;
  void setPayee(const QString &payee);

  QString payeeAccount() const;
  void setPayeeAccount(const QString &payeeAccount);

  float amount() const;
  void setAmount(float amount);
  bool isAmountNaN() const;

  int category() const;
  void setCategory(int category);

  int ruleId() const;
  void setRuleId(int ruleId);

  void setToRule(const Rule &rule);

private:
  int m_id;
  SQLiteDate m_date;
  QString m_type;
  QString m_description;
  QString m_payee;
  QString m_payeeAccount;
  float m_amount;
  int m_category;
  int m_ruleId;
};

#endif // STATEMENTROW_H
