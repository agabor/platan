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

#include "sqlselect.h"
#include <cstdarg>

SQLSelect::SQLSelect(std::initializer_list<QString> tables)
{
    for(auto &table : tables)
        this->tables.push_back(table);
}

QString SQLSelect::groupByClause() const
{
    if (group_by_field == QString())
        return QString();
    return QString(" GROUP BY %1").arg(group_by_field);
}

QString SQLSelect::toString() const
{
    QString result("SELECT %1 FROM %2%3%4");
    result = result.arg(fieldList())
            .arg(concatenate(tables, ", "))
            .arg(whereClause())
            .arg(groupByClause());
    return result;
}
