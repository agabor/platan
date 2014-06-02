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

#ifndef SQLQUERYCOMPONENTS_H
#define SQLQUERYCOMPONENTS_H

#include <QString>
#include <string>
#include <sqlcondition.h>

QString concatenate(const std::vector<QString> &strings, QString deliminator);

class SQLFields
{
public:
    void field(QString field_name);
protected:
    SQLFields(){}
    QString fieldList() const;
    std::vector<QString> fields;
};

class SQLFieldValue : public SQLFields
{
public:
    void set(QString field, QString value);
    void set(QString field, int value);
    void set(QString field, float value);
protected:
    SQLFieldValue();
    void addFieldValuePair(QString field, QString value);
    QString valueList() const;
    QString setFieldsList() const;
private:
    std::vector<QString> values;
};

class SQLWhere
{
public:
    void where(QString condition);
    void where(const SQLCondition &condition);
protected:
    QString whereClause() const;
private:
    std::vector<QString> conditions;
    QString conditionList() const;
};

#endif // SQLQUERYCOMPONENTS_H
