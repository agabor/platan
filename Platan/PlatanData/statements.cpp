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
#include <sqlitedb.h>

using namespace std;

const CategoryList Statements::categoryList;
const ColumnList Statements::columnList;


Statements::Statements()
{
    for(int i = 1; i < categoryList.count(); ++i)
        classStatements[i].reset(new StatementTableModel);
    uncategorisedStatements.reset(new StatementTableModel);
    allStatements.reset(new StatementTableModel);
    timeIntervalSet = false;
}

shared_ptr<StatementTableModel> Statements::getUncategorisedStatements()
{
    return uncategorisedStatements;
}

shared_ptr<StatementTableModel> Statements::getAllStatements()
{
    return allStatements;
}

void Statements::initStatementCategories()
{
    QVector<shared_ptr<Statement>> uncategorised;
    QMap<int, QVector<shared_ptr<Statement>>> categories;
    for(int i : classStatements.keys())
        categories.insert(i, QVector<shared_ptr<Statement>>());

    for(auto row : statementsInDateRange())
    {
        const int category = row->category;
        if (category == 0)
            uncategorised.push_back(row);
        else
            if (categories.keys().contains(category))
                categories[category].push_back(row);
    }

    for (int k : categories.keys())
        classStatements[k]->setData(categories[k]);
    uncategorisedStatements->setData(uncategorised);
}

shared_ptr<StatementTableModel> Statements::getStatementsForClass(int classIdx)
{
    return classStatements[classIdx];
}

void Statements::refreshTableModels()
{
    allStatements->setData(*this);
    categorizeUndefinedStatements();
    initStatementCategories();
}

void Statements::setCategory(int id, int category)
{
    std::shared_ptr<Statement> row;
    int idx = -1;
    for (auto s : *this)
    {
        ++idx;
        if(s->id == id)
        {
            row = s;
            break;
        }
    }

    if (row.get() == nullptr)
        return;

   row->category = category;

    if (!changes.contains(row))
    {
        changes.insert(row);
        emit modification();
    }
    refreshTableModels();
}

bool Statements::changed() const
{
    return !changes.isEmpty() || !newStatements.isEmpty();
}

void Statements::save()
{
    SQLiteDB::getInstance().beginTransaction();
    for (auto s : changes)
    {
        s->update();
    }
    changes.clear();

    for (auto s : newStatements)
    {
        s->insert();
    }
    newStatements.clear();
    SQLiteDB::getInstance().endTransaction();
    emit modification();
}

void Statements::categorizeUndefinedStatements()
{
    QVector<Rule> rules = Rule::getAll();
    bool changed = false;
    for (auto s : *this)
    {
        if (s->category == 0)
        {
            for(Rule r : rules)
            {
                if (apply(s, r))
                {
                    changes.insert(s);
                    changed = true;
                    break;
                }
            }
        }
    }
    if (changed)
        emit modification();
}

bool Statements::apply(shared_ptr<Statement> statement, Rule rule)
{

    if (statement->at(rule.column) == rule.value)
    {
        statement->category = rule.category;
        return true;
    }
    return false;
}

QVector<shared_ptr<Statement>> Statements::statementsInDateRange()
{
    QVector<shared_ptr<Statement>> result;
    for(shared_ptr<Statement> row : *this)
    {
        if (timeIntervalSet && (row->date < startDate || row->date > endDate))
            continue;
        result.append(row);
    }
    return result;
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
    for(auto row : statementsInDateRange())
    {
        const int category = row->category;
        if (result.keys().contains(category))
            result[category] += row->amount;
        else
            result.insert(category, row->amount);
    }
    return result;
}


bool Statements::open(QString data_base_path)
{
    auto data_base = SQLiteDB::getInstance();
    if(data_base.isOpen())
        data_base.close();
    data_base.setPath(data_base_path);
    if(!data_base.open())
        return false;
    clear();
    for(Statement &s : Statement::getAll(SQLCondition::Empty))
        append(shared_ptr<Statement>(new Statement(s)));
    refreshTableModels();
    return true;
}

void Statements::create(QString data_base_path)
{
    auto data_base = SQLiteDB::getInstance();
    data_base.setPath(data_base_path);
    data_base.create();
    data_base.executeScript("../rules.sql");
}


void Statements::insertData(QVector<Statement> statements)
{
    for(Statement &s : statements)
    {
        shared_ptr<Statement> p(new Statement(s));
        newStatements.insert(p);
        append(p);
    }
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

