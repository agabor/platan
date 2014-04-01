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

#include "sqlitestatement.h"

#include <sstream>
#include "sqlitedate.h"

using namespace std;

SQLiteStatement::SQLiteStatement()
{
    statement = 0;
}

SQLiteStatement::~SQLiteStatement()
{
    //delete statement;
}

int SQLiteStatement::GetInt(int idx)
{
    return sqlite3_column_int(statement, idx);
}

double SQLiteStatement::GetDouble(int idx)
{
    return sqlite3_column_double(statement, idx);
}

inline std::string stringify(const unsigned char *c)
{
    ostringstream o;
    o << c;
    return string(o.str());
}

std::string SQLiteStatement::GetText(int idx)
{
    return stringify(sqlite3_column_text(statement, idx));;
}

QDate SQLiteStatement::GetDate(int idx)
{
   return SQLiteDate(GetInt(idx));
}
