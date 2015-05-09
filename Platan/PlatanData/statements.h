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

#include <sqlitedb.h>
#include <statementmapper.h>
#include <sqlitedate.h>

template <typename T>
class QVector;
class QString;

class Statement;
class Rule;

class Statements : public QObject, public QVector<QSharedPointer<Statement>>
{
    Q_OBJECT
public:
    Statements(SQLiteDB &db);
    static QStringList categoryList();
    static QStringList columnList();
    void SetTimeInterval(QDate start_date, QDate end_date);
    void UnsetTimeInterval();
    QMap<int, float> getCategories();
    void init();
    void GetClasses(QString lan, QMap<int, QString> &classes);
    void insertData(QVector<Statement> statements);
    void initStatementCategories();
    void setCategory(int id, int category);
    bool changed() const;
    void save();
    QVector<QSharedPointer<Statement>> statementsInDateRange();
    void categorizeUndefinedStatements(QVector<QSharedPointer<Rule> > rules);
    void categorizeUndefinedStatements(Rule &rule);
    void rollBack(Rule rule);
signals:
    void dataChanged();
private:
    SQLiteDate startDate, endDate;
    bool timeIntervalSet;
    QSet<QSharedPointer<Statement>> changes;
    QSet<QSharedPointer<Statement>> newStatements;
    QVector<QSharedPointer<Statement>> getUncategorisedStatements();
    SQLiteDB &db;
    StatementMapper statementMapper;
};

#endif // STATEMENTS_H
