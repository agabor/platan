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

#include <QString>
#include <QVector>

#include <statements.h>
#include <statement.h>
#include <rule.h>

using namespace std;


uint qHash(shared_ptr<Statement> key, uint seed) Q_DECL_NOTHROW
{
    return ulong(key.get()) ^ seed;
}


Statements::Statements(SQLiteDB &db) : db(db), statementMapper(db)
{
    timeIntervalSet = false;
}

QStringList Statements::categoryList()
{
    QStringList result;
    result.push_back(QObject::tr("Other"));
    result.push_back(QObject::tr("Supermarket"));
    result.push_back(QObject::tr("Clothing store"));
    result.push_back(QObject::tr("Rent"));
    result.push_back(QObject::tr("Public transportation"));
    result.push_back(QObject::tr("Bills"));
    result.push_back(QObject::tr("Electronics store"));
    result.push_back(QObject::tr("Cash"));
    result.push_back(QObject::tr("Furniture store"));
    result.push_back(QObject::tr("Hardware store"));
    result.push_back(QObject::tr("Department store"));
    result.push_back(QObject::tr("Online marketplace"));
    result.push_back(QObject::tr("Restaurant"));
    result.push_back(QObject::tr("Sport"));
    result.push_back(QObject::tr("Insurance"));
    result.push_back(QObject::tr("Bank"));
    result.push_back(QObject::tr("Drogstore"));
    result.push_back(QObject::tr("Mobil"));
    result.push_back(QObject::tr("Savings"));
    return result;
}

QStringList Statements::columnList()
{
    QStringList result;
    result.push_back(QObject::tr("Date"));
    result.push_back(QObject::tr("Type"));
    result.push_back(QObject::tr("Description"));
    result.push_back(QObject::tr("Payee"));
    result.push_back(QObject::tr("Payee account"));
    result.push_back(QObject::tr("Amount"));
    return result;
}


void Statements::setCategory(int id, int category)
{
    shared_ptr<Statement> row;
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
        emit dataChanged();
    }
}

bool Statements::changed() const
{
    return !changes.isEmpty() || !newStatements.isEmpty();
}

void Statements::save()
{
    db.beginTransaction();
    for (auto s : changes)
    {
        statementMapper.update(*s);
    }
    changes.clear();

    for (auto s : newStatements)
    {
        statementMapper.insert(*s);
    }
    newStatements.clear();
    db.endTransaction();
}

void Statements::categorizeUndefinedStatements(QVector<shared_ptr<Rule>> rules)
{
    bool changed = false;
    for (auto s : getUncategorisedStatements())
    {
        for(auto r : rules)
        {
            if (r->apply(*s.get()))
            {
                changes.insert(s);
                changed = true;
                break;
            }
        }
    }
    if (changed)
        emit dataChanged();
}

void Statements::categorizeUndefinedStatements(Rule &rule)
{
    bool changed = false;
    for (auto s : getUncategorisedStatements())
    {
        if (rule.apply(*s.get()))
        {
            changes.insert(s);
            changed = true;
        }
    }
    if (changed)
        emit dataChanged();
}

void Statements::rollBack(Rule rule)
{
    bool changed = false;
    for(auto s : *this)
    {
        if (s->ruleId == rule.id)
        {
            s->ruleId = -1;
            s->category = 0;
            changes.insert(s);
            changed = true;
        }
    }
    if (changed)
        emit dataChanged();
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

QVector<shared_ptr<Statement> > Statements::getUncategorisedStatements()
{
    QVector<shared_ptr<Statement> > result;
    for (auto s : *this)
    {
        if (s->category == 0)
        {
            result.push_back(s);
        }
    }
    return result;
}


void Statements::SetTimeInterval(QDate start_date, QDate end_date)
{
    timeIntervalSet = true;
    startDate = start_date;
    endDate = end_date;
}


void Statements::UnsetTimeInterval()
{
    timeIntervalSet = false;
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


void Statements::init()
{
    clear();
    for(Statement &s : statementMapper.getAll(SQLCondition::Empty))
        push_back(shared_ptr<Statement>(new Statement(s)));
}


void Statements::insertData(QVector<Statement> statements)
{
    for(Statement &s : statements)
    {
        shared_ptr<Statement> p(new Statement(s));
        newStatements.insert(p);
        if (p->amount < 0)
            append(p);
    }
    emit dataChanged();
}

