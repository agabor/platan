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


#include <QStringList>
#include <QObject>
#include <QMap>
#include <QSet>

#include <memory>

#include <sqlitedb.h>
#include <statementmapper.h>
#include <sqlitedate.h>

template <typename T>
class QVector;
class QString;

class Statement;
class Rule;

class Statements : public QObject, public QVector<std::shared_ptr<Statement>>
{
    Q_OBJECT
public:
    Statements(SQLiteDB &db);
    static QStringList categoryList();
    static QStringList columnList();
    void SetTimeInterval(QDate start_date, QDate end_date);
    void UnsetTimeInterval();
    QMap<int, float> getCategories();
    bool open(QString data_base_path);
    void GetClasses(QString lan, QMap<int, QString> &classes);
    void insertData(QVector<Statement> statements);
    void initStatementCategories();
    void setCategory(int id, int category);
    bool changed() const;
    void save();
    QString getOpenProjectPath() const;
    QVector<std::shared_ptr<Statement>> statementsInDateRange();
signals:
    void dataChanged();
    void modification();
private:
    void categorizeUndefinedStatements(QVector<std::shared_ptr<Rule> > rules);
    SQLiteDate startDate, endDate;
    bool timeIntervalSet;
    QSet<std::shared_ptr<Statement>> changes;
    QSet<std::shared_ptr<Statement>> newStatements;
    QVector<std::shared_ptr<Statement>> getUncategorisedStatements();
    QString openProjectPath;
    SQLiteDB &db;
    static DataBaseSchema getSchema();
    StatementMapper statementMapper;
};

#endif // STATEMENTS_H
