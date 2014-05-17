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

CSVReader::CSVReader()
{
}

CSVTableModel* CSVReader::ReadCSV(QString filename)
{
    CSVTableModel *result = new CSVTableModel();
    QFile csv_file(filename);
    csv_file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&csv_file);
    //in.setCodec("UTF-8");
    QString line;
    int col_num = -1;
    while (!in.atEnd())
    {
        line = in.readLine();
        QStringList string_list = line.split(";");
        if (col_num == -1)
        {
            col_num = string_list.length();
            result->setHeaders(string_list);
            continue;
        }
        if (col_num == string_list.length())
            result->addRow(string_list);
    }
    csv_file.close();
    return result;
}
