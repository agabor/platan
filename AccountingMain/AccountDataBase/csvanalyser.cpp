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

#include "csvanalyser.h"
#include <QTextStream>
#include <QVector>
#include <QString>
#include <QMap>
#include <QPair>

CSVAnalyser::CSVAnalyser(QTextStream &_input) : input(_input)
{
    separator = '\0';
    analyse();
}

void CSVAnalyser::analyse()
{


    QString line = input.readLine();
    QVector<char> separators{';', ',', '.', '\t'};
    QMap<char, QPair<int, bool>> sep_map;
    for (char c : separators)
        sep_map.insert(c, QPair<int, bool>(line.count(c), true));

    for(int i = 0; i < 9 && !input.atEnd(); ++i)
    {
        line = input.readLine();
        for (char c : separators)
        {
            auto val = sep_map[c];
            if (val.second && val.first != line.count(c))
                sep_map[c] = QPair<int, bool>(0, false);
        }
    }

    int max = -1;
    char maxchar;
    for (char c : separators)
    {
        auto val = sep_map[c];
        if (val.second && max < val.first)
        {
            max = val.first;
            maxchar = c;
        }
    }
    if (max != -1)
        separator = maxchar;
}

char CSVAnalyser::getSeparator() const
{
    return separator;
}
