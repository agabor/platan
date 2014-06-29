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


#include <QString>
#include <QTextStream>
#include <QStringList>

#include "csvtablemodel.h"
#include "csvreader.h"

using namespace std;

CSVReader::CSVReader()
{
    _separator = ',';
    _quote ='\0';
    _headersInFirstRow = false;
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

CSVTableModel *CSVReader::read(QTextStream &input)
{
    CSVTableModel *result = new CSVTableModel();
    QString line;
    int col_num = -1;
    input.reset();
    while (!input.atEnd())
    {
        line = input.readLine();
        QStringList string_list = line.split(_separator);
        if (_quote != '\0')
        {
            string_list = removeQuotes(string_list);
        }
        if (col_num == -1)
        {
            col_num = string_list.length();
            if (headersInFirstRow())
            {
                result->setHeaders(string_list);
                continue;
            }
        }
        if (col_num < string_list.length())
        {
            col_num = string_list.length();
            result->increaseColumnCountTo(col_num);

        }
        result->addRow(string_list);
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
    _headersInFirstRow = b;
}
