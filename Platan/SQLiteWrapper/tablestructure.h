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

#ifndef TABLESTRUCTURE_H
#define TABLESTRUCTURE_H

#include <QString>
#include <vector>
#include <utility>
#include "sqltype.h"

class TableStructure
{
public:
    TableStructure(QString name);
    static TableStructure fromSchema(QString schema);
    inline static TableStructure Invalid();

    bool operator ==(const TableStructure &other) const;
    bool operator !=(const TableStructure &other) const;
    void addField(QString name, SQLType type);
    inline bool isValid() const;
    inline QString getName() const;
    QString sqlCommand() const;
private:
    std::vector<std::pair<QString, SQLType>> name_type_pairs;
    int fieldCount() const;
    bool is_valid;
    QString name;
};

//Inline functions

inline TableStructure TableStructure::Invalid()
{
    TableStructure iv{QString{}};
    iv.is_valid = false;
    return iv;
}

inline bool TableStructure::isValid() const
{
    return is_valid;
}

inline QString TableStructure::getName() const
{
    return name;
}

#endif // TABLESTRUCTURE_H
