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


void Statements::initUncategorisedStatements()
{
    QVector<Statement> result;
    for(const Statement &row : *this)
    {
        if (row.category == 0)
        {
            result.push_back(row);
        }
    }
    if (uncategorisedStatements.get() == nullptr)
        uncategorisedStatements.reset(new StatementTableModel(result));
    else
        uncategorisedStatements->setData(result);
}

void Statements::initAllStatements()
{
    if (allStatements.get() == nullptr)
        allStatements.reset(new StatementTableModel(*this));
    else
        allStatements->setData(*this);
}

std::shared_ptr<StatementTableModel> Statements::getUncategorisedStatements()
{
    initUncategorisedStatements();
    return uncategorisedStatements;
}

std::shared_ptr<StatementTableModel> Statements::getAllStatements()
{
    initAllStatements();
    return allStatements;
}

void Statements::initClassStatements(int classIdx)
{
    QVector<StatementExtractRow> result;

    for(const Statement &row : *this)
    {
        if (row.category == classIdx)
        {
            StatementExtractRow extract_row;
            extract_row.Ammount = row.amount;
            extract_row.Date = row.date;
            extract_row.Payee = row.payee;
            result.push_back(extract_row);
        }
    }
    if (classStatements.find(classIdx) == classStatements.end())
        classStatements[classIdx] = std::shared_ptr<StatementExtractTableModel>(new StatementExtractTableModel(result));
    else
        classStatements[classIdx]->setData(result);
}

std::shared_ptr<StatementExtractTableModel> Statements::getStatementsForClass(int classIdx)
{
    initClassStatements(classIdx);
    return classStatements[classIdx];
}


void Statements::refreshTableModels()
{
    initUncategorisedStatements();
    initAllStatements();
    for(int classIdx : classStatements.keys())
        initClassStatements(classIdx);
}

void Statements::setCategory(Statement &row, int category)
{
    row.category = category;
    row.update();
    clear();
    *this << Statement::getAll();
    refreshTableModels();
}

void Statements::SetTimeInterval(QDate start_date, QDate end_date)
{
    data_base.setTimeInterval(start_date, end_date);
    clear();
    *this << Statement::getAll();
    refreshTableModels();
}


void Statements::UnsetTimeInterval()
{
    data_base.unsetTimeInterval();
    clear();
    *this << Statement::getAll();
    refreshTableModels();
}


void Statements::GetCalssification(QMap<int, float> &result)
{
    data_base.getCalssification(result);
}


bool Statements::Open(QString data_base_path)
{
    bool result = data_base.setPath(data_base_path);
    clear();
    *this << Statement::getAll();
    return result;
}

void Statements::New(QString data_base_path)
{
    data_base.create(data_base_path);
}


void Statements::InsertData(StatementTableModel &model)
{
    data_base.insertData(model);
    data_base.classify();
    clear();
    *this << Statement::getAll();
    refreshTableModels();
    emit dataChanged();
}

void Statements::InsertRule(Rule rule)
{
    rule.insert();
    data_base.classify();
    clear();
    *this << Statement::getAll();
    refreshTableModels();
    emit dataChanged();
}

