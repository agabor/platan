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

CSVReader::CSVReader(QString filename)
{
    separator = ',';
    quote ='\0';
    headersInFirstRow = false;
    fileName = filename;
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
    if (str.at(0) == quote && str.at(str.length() -1) == quote)
    {
        str = str.mid(1);
        str.chop(1);
    }
    return str;
}

CSVTableModel *CSVReader::read()
{
    CSVTableModel *result = new CSVTableModel();
    QFile csv_file(fileName);
    csv_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&csv_file);
    QString line;
    int col_num = -1;
    while (!in.atEnd())
    {
        line = in.readLine();
        QStringList string_list = line.split(separator);
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
            if (quote != '\0')
            {
                string_list = removeQuotes(string_list);
            }
            result->addRow(string_list);
        }
    }
    csv_file.close();
    return result;
}

void CSVReader::setSeparator(char s)
{
    separator = s;
}

void CSVReader::setQuote(char q)
{
    quote = q;
}

void CSVReader::setHeadersInFirstRow(bool b)
{
    headersInFirstRow = b;
}
