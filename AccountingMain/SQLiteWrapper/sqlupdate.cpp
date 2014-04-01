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

#include "sqlupdate.h"

SQLUpdate::SQLUpdate(QString table_name)
{
    this->table_name = table_name;
}

std::string SQLUpdate::toString() const
{
    QString result("UPDATE %1 SET %2%3");
    result = result.arg(table_name)
             .arg(setFieldsList())
             .arg(whereClause());
    return result.toStdString();
}
