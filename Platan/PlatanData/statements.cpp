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
    QVector<StatementRow> result;
    for(const StatementRow &row : *this)
    {
        if (row.Class == 0)
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

    for(const StatementRow &row : *this)
    {
        if (row.Class == classIdx)
        {
            StatementExtractRow extract_row;
            extract_row.Ammount = row.Amount;
            extract_row.Date = row.Date;
            extract_row.Payee = row.Payee;
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

void Statements::setCategory(StatementRow &row, int category)
{
    data_base.setCategory(row, category);
    data_base.readData(*this);
    refreshTableModels();
}

void Statements::SetTimeInterval(QDate start_date, QDate end_date)
{
    data_base.setTimeInterval(start_date, end_date);
    data_base.readData(*this);
    refreshTableModels();
}


void Statements::UnsetTimeInterval()
{
    data_base.unsetTimeInterval();
    data_base.readData(*this);
    refreshTableModels();
}


void Statements::GetCalssification(QMap<int, float> &result)
{
    data_base.getCalssification(result);
}


void Statements::Open(QString data_base_path)
{
    data_base.setPath(data_base_path);
    data_base.readData(*this);
}

void Statements::New(QString data_base_path)
{
    data_base.create(data_base_path);
}


void Statements::InsertData(StatementTableModel &model)
{
    data_base.insertData(model);
    data_base.classify();
    data_base.readData(*this);
    refreshTableModels();
    emit dataChanged();
}

void Statements::InsertRule(Rule rule)
{
    data_base.insertRule(rule);
    data_base.classify();
    data_base.readData(*this);
    refreshTableModels();
    emit dataChanged();
}

QVector<Rule> Statements::getRules()
{
    return data_base.getRules();
}
