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

#ifndef STATEMENTEXTRACTROW_H
#define STATEMENTEXTRACTROW_H

#include <QDateTime>
#include <QString>
#include <string>
#include <QVariant>

struct StatementExtractRow
{
    QDate Date;
    std::string Payee;
    float Ammount;

    static int size()
    {
        return 3;
    }
    QVariant at(int idx) const
    {
        switch (idx)
        {
        case 0:
            return Date;
        case 1:
            return QString(Payee.c_str());
        case 2:
            return Ammount;
        }
        return 0;
    }

};

#endif // STATEMENTEXTRACTROW_H
