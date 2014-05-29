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

#ifndef SQLTYPE_H
#define SQLTYPE_H

#include <QString>

class SQLType
{
public:
    static SQLType Invalid();
    static SQLType Integer();
    static SQLType Real();
    static SQLType Text();
    static SQLType Numeric();
    static SQLType DefaultPK()
    {
        return Integer().PK().AutoIncrement().NotNull();
    }

    SQLType PK() const;
    SQLType AutoIncrement() const;
    SQLType NotNull() const;
    bool operator ==(const SQLType& other) const;
    bool operator !=(const SQLType& other) const;
    QString toString() const;
    static SQLType parse(QString sqlstring);
private:
    SQLType(char val);
    SQLType(SQLType other, bool isPK, bool isAutoIncrement, bool isNotNull);
    char val;
    bool isPK;
    bool isAutoIncrement;
    bool isNotNull;
};

#endif // SQLTYPE_H
