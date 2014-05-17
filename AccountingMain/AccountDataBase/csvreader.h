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

#ifndef CSVREADER_H
#define CSVREADER_H
#include <QString>
#include "csvtablemodel.h"

class CSVReader
{

public:
    CSVReader(QString fileName);
    CSVTableModel* read();
    void setSeparator(char s);
    inline char separator() const;
    void setQuote(char q);
    inline char quote() const;
    void setHeadersInFirstRow(bool b);
    QStringList removeQuotes(QStringList string_list);
    QString removeQuotes(QString str);
private:
    char _separator;
    char _quote;
    bool headersInFirstRow;
    QString fileName;
};

char CSVReader::separator() const
{
    return _separator;
}

char CSVReader::quote() const
{
    return _quote;
}

#endif // CSVREADER_H
