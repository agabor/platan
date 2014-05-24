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

#ifndef ACCDATABASE_H
#define ACCDATABASE_H

#include "statementtablemodel.h"
#include "statementextracttablemodel.h"
#include <string>
#include <vector>
#include <QMap>
#include <QDate>
#include "sqlitedb.h"
#include <sqlitedate.h>

class AccDataBase
{
public:
    AccDataBase();
    ~AccDataBase();
    void SetPath(QString data_base_path);
    void Create(QString data_base_path);
    void InsertData(StatementTableModel &model);
    void ReadData(std::vector<StatementRow> &model, bool only_unclassified = false);
    void GetCalssification(QMap<int, float> &result);
    void GetClasses(QString lan, QMap<int, QString> &classes);
    void InsertRule(int type, int column, QString value, int Class, QString name);
    void SetTimeInterval(QDate start_date, QDate end_date);
    void UnsetTimeInterval();
    void Classify();
protected:
    SQLiteDB data_base;
    SQLiteDate start_date, end_date;
    bool time_interval_set;
    void SetTimeInterval(SQLSelect &select);
};

#endif // ACCDATABASE_H
