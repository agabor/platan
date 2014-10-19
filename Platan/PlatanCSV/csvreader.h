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

class QString;
class QStringList;
class CSVTableModel;
class QTextStream;

class CSVReader
{

public:
    CSVReader();
    CSVTableModel* read(QTextStream &input);
    void setSeparator(char s);
    inline char separator() const;
    void setQuote(char q);
    inline char quote() const;
    void setHeadersInFirstRow(bool b);
    inline bool headersInFirstRow() const;
    QStringList removeQuotes(QStringList string_list);
    QString removeQuotes(QString str);
    void setMaxium(int max)
    {
        maximum = max;
        maximumIsSet = true;
    }

private:
    char _separator;
    char _quote;
    bool _headersInFirstRow;
    bool maximumIsSet = false;
    int maximum;
};

char CSVReader::separator() const
{
    return _separator;
}

char CSVReader::quote() const
{
    return _quote;
}

bool CSVReader::headersInFirstRow() const
{
    return _headersInFirstRow;
}

#endif // CSVREADER_H
