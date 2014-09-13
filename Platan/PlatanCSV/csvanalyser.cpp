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
#include <QSet>

class SyntaxType
{
public:
    SyntaxType()
    {
    }

    SyntaxType(int _length, QSet<QChar> _chars)
        : length(_length), chars(_chars)
    {
    }
    bool operator==(const SyntaxType& other) const
    {
        if (length != other.length)
            return false;
        return chars == other.chars;
    }
    bool operator!=(const SyntaxType& other) const
    {
        return !(*this == other);
    }
private:
    QSet<QChar> chars;
    int length;
};

CSVAnalyser::CSVAnalyser(QTextStream &_input) : input(_input)
{
    separator = '\0';
    quote = '\0';
    headers = false;
    analyse();
}

char detectSeparator(QVector<QString> lines)
{
    QString line = lines[0];
    QVector<QChar> separators{';', ',', '.', '\t', '-'};
    QMap<QChar, QPair<int, bool>> sep_map;
    for (QChar c : separators)
        sep_map.insert(c, QPair<int, bool>(line.count(c), true));

    for(int i = 1; i < lines.count(); ++i)
    {
        line = lines[i];
        for (QChar c : separators)
        {
            auto val = sep_map[c];
            if (val.second && val.first != line.count(c))
                sep_map[c] = QPair<int, bool>(0, false);
        }
    }

    int max = -1;
    QChar maxchar;
    for (QChar c : separators)
    {
        auto val = sep_map[c];
        if (val.second && max < val.first)
        {
            max = val.first;
            maxchar = c;
        }
    }

    if (max == -1)
        return '\0';

    return maxchar.toLatin1();
}
char detectQuote(QVector<QString> lines, char separator)
{
    int quote1 = 0;
    int quote2 = 0;
    for(QString line : lines)
    {
        QStringList fields = line.split(separator);
        for(QString field : fields)
        {
            if (field.length() < 2)
                continue;
            QChar quote = '\0';
            if (field.at(0) == field.at(field.size()-1))
                quote = field.at(0);
            if (quote == '\'')
                ++quote1;
            if (quote == '"')
                ++quote2;
        }
    }
    if (quote1 != 0 || quote2 != 0)
    {
        if (quote1 > quote2)
            return '\'';
        else
            return '"';
    }
    return '\0';
}

SyntaxType getSyntaxType(QString text)
{
    QSet<QChar> chars;
    for(QChar c : text)
    {
        if (!c.isDigit())
            chars.insert(c);
    }
    return SyntaxType(text.length(), chars);
}

bool detectHeadersInFirstLine(QVector<QString> lines, char separator)
{
    QStringList firstLine = lines[1].split(separator);
    QVector<QPair<SyntaxType, bool>> syntaxList;
    for (QString field : firstLine)
    {
        syntaxList.push_back(QPair<SyntaxType, bool>(getSyntaxType(field), true));
    }

    for (int i = 2; i < lines.count(); ++i)
    {
        int idx = 0;
        for (QString field : lines[i].split(separator))
        {

            QPair<SyntaxType, bool> pair = syntaxList[idx];
            if (pair.second && pair.first != getSyntaxType(field))
                syntaxList[idx] = QPair<SyntaxType, bool>(pair.first, false);

            idx++;
        }
    }
    int idx = 0;
    for (QString field : lines[0].split(separator))
    {

        QPair<SyntaxType, bool> pair = syntaxList[idx];
        if (pair.second && pair.first != getSyntaxType(field))
        {
            return true;
        }
        idx++;
    }
    return false;
}


void CSVAnalyser::analyse()
{
    QVector<QString> lines;
    for (int i= 0; i < 10 && !input.atEnd(); ++i)
        lines.push_back(input.readLine());

    separator = detectSeparator(lines);
    if (separator == '\0')
        return;

    quote = detectQuote(lines, separator);

    headers = detectHeadersInFirstLine(lines, separator);
}

char CSVAnalyser::getSeparator() const
{
    return separator;
}

char CSVAnalyser::getQuote() const
{
    return quote;
}

bool CSVAnalyser::areHeadersInFirstLine() const
{
    return headers;
}
