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
#include <rule.h>

class AccDataBase
{
public:
    AccDataBase();
    ~AccDataBase();
    void setPath(QString data_base_path);
    void create(QString data_base_path);
    void insertData(StatementTableModel &model);
    void readData(QVector<StatementRow> &model, bool only_unclassified = false);
    void getCalssification(QMap<int, float> &result);
    void getClasses(QString lan, QMap<int, QString> &classes);
    void insertRule(Rule rule);
    void setTimeInterval(QDate start_date, QDate end_date);
    void unsetTimeInterval();
    QVector<Rule> getRules(int column = -1);
    void classify();
protected:
    SQLiteDB data_base;
    SQLiteDate start_date, end_date;
    bool time_interval_set;
    void setTimeInterval(SQLSelect &select);
};

#endif // ACCDATABASE_H
