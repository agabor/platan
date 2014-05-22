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

#ifndef STATEMENTS_H
#define STATEMENTS_H

#include "statementrow.h"
#include "statementextractrow.h"
#include "accdatabase.h"

#include <map>
#include <vector>
#include <memory>
#include <utility>
#include <QObject>

class Statements : public QObject
{
    Q_OBJECT
public:
    std::vector<StatementRow> GetStatements();
    std::vector<StatementExtractRow> GetStatementsForClass(int class_idx);
    void SetTimeInterval(QDate start_date, QDate end_date);
    void UnsetTimeInterval();
    void GetCalssification(QMap<int, float> &result);
    void Open(QString data_base_path);
    void New(QString data_base_path);
    void GetClasses(QString lan, QMap<int, QString> &classes);
    void InsertData(StatementTableModel &model);
signals:
    void dataChanged();
private:
    AccDataBase data_base;
    std::map<int, std::unique_ptr<StatementExtractTableModel>> class_statements;
    std::vector<StatementRow> statements;
};

#endif // STATEMENTS_H
