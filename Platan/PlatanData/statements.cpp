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

#include "statements.h"

using namespace std;

const CategoryList Statements::categoryList;
const ColumnList Statements::columnList;

Statements::Statements()
{
    for(int i = 1; i < categoryList.count(); ++i)
        classStatements[i].reset(new StatementExtractTableModel(QVector<StatementExtractRow>()));
    uncategorisedStatements.reset(new StatementTableModel(QVector<Statement>()));
    allStatements.reset(new StatementTableModel(QVector<Statement>()));
    timeIntervalSet = false;
}

std::shared_ptr<StatementTableModel> Statements::getUncategorisedStatements()
{
    return uncategorisedStatements;
}

std::shared_ptr<StatementTableModel> Statements::getAllStatements()
{
    return allStatements;
}

void Statements::initStatementCategories()
{
    QVector<Statement> uncategorised;
    QMap<int, QVector<StatementExtractRow>> categories;
    for(int i : classStatements.keys())
        categories.insert(i, QVector<StatementExtractRow>());

    for(const Statement &row : *this)
    {
        const int category = row.category;
        if (category == 0)
            uncategorised.push_back(row);
        else
            if (categories.keys().contains(category))
            {
                StatementExtractRow extract_row;
                extract_row.Ammount = row.amount;
                extract_row.Date = row.date;
                extract_row.Payee = row.payee;
                categories[category].push_back(extract_row);
            }
    }

    for (int k : categories.keys())
        classStatements[k]->setData(categories[k]);
    uncategorisedStatements->setData(uncategorised);
}

std::shared_ptr<StatementExtractTableModel> Statements::getStatementsForClass(int classIdx)
{
    return classStatements[classIdx];
}

void Statements::refreshTableModels()
{
    clear();
    if (timeIntervalSet)
        *this << Statement::getAll(startDate.greater("Date") + endDate.less("Date"));
    else
        *this << Statement::getAll(SQLCondition::Empty);
    allStatements->setData(*this);
    initStatementCategories();
}

void Statements::setCategory(Statement &row, int category)
{
    row.category = category;
    row.update();
    refreshTableModels();
}

void Statements::categorizeUndefinedStatements()
{
    QVector<Rule> rules = Rule::getAll();
    for (Statement &s : *this)
    {
        if (s.category == 0)
        {
            for(Rule r : rules)
            {
                if (apply(s, r))
                    break;
            }
        }
    }
}

bool Statements::apply(Statement &statement, Rule rule)
{

    if (statement.at(rule.column) == rule.value)
    {
        statement.category = rule.category;
        return true;
    }
    return false;
}

void Statements::SetTimeInterval(QDate start_date, QDate end_date)
{
    timeIntervalSet = true;
    startDate = start_date;
    endDate = end_date;
    refreshTableModels();
}


void Statements::UnsetTimeInterval()
{
    timeIntervalSet = false;
    refreshTableModels();
}


QMap<int, float> Statements::getCategories()
{
    QMap<int, float> result;
    for(const Statement &row : *this)
    {
        const int category = row.category;
        if (result.keys().contains(category))
            result[category] += row.amount;
        else
            result.insert(category, row.amount);
    }
    return result;
}


bool Statements::open(QString data_base_path)
{
    auto data_base = SQLiteDB::getInstance();
    if(data_base.isOpen())
        data_base.close();
    data_base.SetPath(data_base_path);
    if(!data_base.open())
        return false;
    refreshTableModels();
    return true;
}

void Statements::create(QString data_base_path)
{
    auto data_base = SQLiteDB::getInstance();
    data_base.SetPath(data_base_path);
    data_base.create();
    data_base.ExecuteScript("../rules.sql");
}


void Statements::insertData(QVector<Statement> importedStatements)
{
    auto database = SQLiteDB::getInstance();
    database.BeginTransaction();
    for(Statement& s : importedStatements)
        s.insert();
    database.EndTransaction();
    categorizeUndefinedStatements();
    refreshTableModels();
    emit dataChanged();
}

void Statements::insertRule(Rule rule)
{
    rule.insert();
    categorizeUndefinedStatements();
    refreshTableModels();
    emit dataChanged();
}

