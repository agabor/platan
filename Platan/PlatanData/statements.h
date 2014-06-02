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

#include "statement.h"
#include "statementextractrow.h"

#include <map>
#include <vector>
#include <memory>
#include <utility>
#include <QObject>
#include <QString>
#include <QMap>
#include <QSet>
#include <statementtablemodel.h>
#include <statementextracttablemodel.h>
#include <rule.h>

class CategoryList : public QStringList
{
public:
    CategoryList()
    {
        push_back(QObject::tr("Undefined"));
        push_back(QObject::tr("Food"));
        push_back(QObject::tr("Clothes"));
        push_back(QObject::tr("Housing"));
        push_back(QObject::tr("Public transportation"));
        push_back(QObject::tr("House costs"));
        push_back(QObject::tr("Electronics"));
        push_back(QObject::tr("Cash"));
        push_back(QObject::tr("Furniture"));
        push_back(QObject::tr("Restaurant"));
        push_back(QObject::tr("Sport"));
        push_back(QObject::tr("Insurance"));
        push_back(QObject::tr("Bank"));
        push_back(QObject::tr("Drogstore"));
        push_back(QObject::tr("Mobil"));
    }
};


class ColumnList : public QStringList
{
public:
    ColumnList()
    {
        push_back(QObject::tr("Date"));
        push_back(QObject::tr("Type"));
        push_back(QObject::tr("Description"));
        push_back(QObject::tr("Payee"));
        push_back(QObject::tr("Payee account"));
        push_back(QObject::tr("Amount"));
    }
};


class Statements : public QObject, public QVector<Statement>
{
    Q_OBJECT
public:
    Statements();
    static const CategoryList categoryList;
    static const ColumnList columnList;
    std::shared_ptr<StatementTableModel> getUncategorisedStatements();
    std::shared_ptr<StatementTableModel> getAllStatements();
    std::shared_ptr<StatementExtractTableModel> getStatementsForClass(int classIdx);
    void SetTimeInterval(QDate start_date, QDate end_date);
    void UnsetTimeInterval();
    QMap<int, float> getCategories();
    bool open(QString data_base_path);
    void create(QString data_base_path);
    void GetClasses(QString lan, QMap<int, QString> &classes);
    void insertData(QVector<Statement> importedStatements);
    void insertRule(Rule rule);
    void initStatementCategories();
    void refreshTableModels();
    void setCategory(int id, int category);
    bool changed() const;
    void save();
signals:
    void dataChanged();
    void changeSetModified(bool changes);
private:
    void categorizeUndefinedStatements();
    bool apply(Statement &statement, Rule rule);
    QMap<int, std::shared_ptr<StatementExtractTableModel>> classStatements;
    std::shared_ptr<StatementTableModel> uncategorisedStatements;
    std::shared_ptr<StatementTableModel> allStatements;
    SQLiteDate startDate, endDate;
    bool timeIntervalSet;
    QSet<int> changes;
    QVector<const Statement *> statementsInDateRange();
};

#endif // STATEMENTS_H
