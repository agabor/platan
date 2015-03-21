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

#ifndef SQLSELECT_H
#define SQLSELECT_H

#include <vector>
#include <initializer_list>

#include "sqlquery.h"
#include "sqlquerycomponents.h"

class SQLSelect : public SQLQuery, public SQLWhere, public SQLFields
{
public:
    SQLSelect(std::initializer_list<QString> tables);
    inline void groupBy(QString field);
    // SQLQuery interface
    QString toString() const override;
private:
    std::vector<QString> tables;
    QString groupByClause() const;

    QString group_by_field;
};

void SQLSelect::groupBy(QString field)
{
    group_by_field = field;
}


#endif // SQLSELECT_H
