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

#include "sqlquerycomponents.h"
#include <string>


using namespace std;

SQLFieldValue::SQLFieldValue()
{

}

void SQLFieldValue::addFieldValuePair(QString field, QString value)
{
    fields.push_back(field);
    values.push_back(value);
}

QString concatenate(const std::vector<QString> &strings, QString deliminator)
{
    QString result;
    bool first = true;
    for (auto val : strings)
    {
        if (!first)
            result += deliminator;
        else
            first = false;
        result += val;
    }
    return result;
}

void SQLFields::field(QString field_name)
{
    fields.push_back(field_name);
}

QString SQLFields::fieldList() const
{
    return concatenate(fields, ", ");
}

QString SQLFieldValue::valueList() const
{
    return concatenate(values, ", ");
}

QString SQLFieldValue::setFieldsList() const
{
    std::vector<QString> fieldSetList;
    for(int i = 0; i < fields.size(); ++i)
    {
        fieldSetList.push_back(QString("%1 = %2").arg(fields.at(i)).arg(values.at(i)));
    }
    return concatenate(fieldSetList, ", ");
}


void SQLFieldValue::set(QString field, QString value)
{
    auto svalue = QString("\"%1\"").arg(value);
    addFieldValuePair(field, svalue);
}

void SQLFieldValue::set(QString field, string value)
{
    set(field, QString(value.c_str()));
}

void SQLFieldValue::set(QString field, int value)
{
    auto svalue = QString(to_string(value).c_str());
    addFieldValuePair(field, svalue);
}

void SQLFieldValue::set(QString field, float value)
{
    auto svalue = QString(to_string(value).c_str());
    addFieldValuePair(field, svalue);
}


void SQLWhere::where(QString condition)
{
    conditions.push_back(condition);
}

QString SQLWhere::whereClause() const
{
    if (conditions.empty())
        return QString();
    return QString(" WHERE %1").arg(conditionList());
}

QString SQLWhere::conditionList() const
{
    return concatenate(conditions, " AND ");
}
