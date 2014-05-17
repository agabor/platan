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

#include "csvreader.h"
#include <string>
#include <QStringList>
#include <QFile>
#include <QTextStream>

using namespace std;

CSVReader::CSVReader(QIODevice &i) : input(i)
{
    _separator = ',';
    _quote ='\0';
    headersInFirstRow = false;
}

QStringList CSVReader::removeQuotes(QStringList string_list)
{
    QStringList withoutQuotes;
    for (QString elem : string_list)
    {
        withoutQuotes.append(removeQuotes(elem));
    }

    return withoutQuotes;
}

QString CSVReader::removeQuotes(QString str)
{
    if (str.length() >= 2 && str.at(0) == _quote && str.at(str.length() -1) == _quote)
    {
        str = str.mid(1);
        str.chop(1);
    }
    return str;
}

CSVTableModel *CSVReader::read()
{
    CSVTableModel *result = new CSVTableModel();
    QTextStream in(&input);
    QString line;
    int col_num = -1;
    while (!in.atEnd())
    {
        line = in.readLine();
        QStringList string_list = line.split(_separator);
        if (col_num == -1)
        {
            col_num = string_list.length();
            if (headersInFirstRow)
            {
                result->setHeaders(string_list);
                continue;
            }
        }
        if (col_num == string_list.length())
        {
            if (_quote != '\0')
            {
                string_list = removeQuotes(string_list);
            }
            result->addRow(string_list);
        }
    }
    return result;
}

void CSVReader::setSeparator(char s)
{
    _separator = s;
}

void CSVReader::setQuote(char q)
{
    _quote = q;
}

void CSVReader::setHeadersInFirstRow(bool b)
{
    headersInFirstRow = b;
}
