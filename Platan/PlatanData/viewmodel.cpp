#include <QMap>

#include <viewmodel.h>
#include <statementtablemodel.h>
#include <statements.h>
#include <rules.h>
#include <ruletablemodel.h>

using namespace std;

ViewModel::ViewModel(Statements &statements, Rules &rules)
    : statements(statements),
      rules(rules)
{
}

void ViewModel::init()
{
    auto cl = statements.categoryList();
    for(int i = 1; i < cl.count(); ++i)
        classStatements[i].reset(new StatementTableModel);
    uncategorisedStatements.reset(new StatementTableModel);
    allStatements.reset(new StatementTableModel);
    ruleTable.reset(new RuleTableModel);
    connect(&rules, SIGNAL(dataChanged()), this, SLOT(initRules()));
    connect(&statements, SIGNAL(dataChanged()), this, SLOT(initStatements()));
}

shared_ptr<StatementTableModel> ViewModel::getUncategorisedStatementsModel()
{
    return uncategorisedStatements;
}

shared_ptr<StatementTableModel> ViewModel::getAllStatements()
{
    return allStatements;
}

shared_ptr<StatementTableModel> ViewModel::getStatementsForClass(int classIdx)
{
    return classStatements[classIdx];
}

shared_ptr<RuleTableModel> ViewModel::getRuleTable()
{
    return ruleTable;
}


void ViewModel::initRules()
{
    ruleTable->setData(rules);
}

void ViewModel::initStatements()
{
    allStatements->setData(statements);
    QVector<QSharedPointer<Statement>> uncategorised;
    QMap<int, QVector<QSharedPointer<Statement>>> categories;
    for(int i : classStatements.keys())
        categories.insert(i, QVector<QSharedPointer<Statement>>());

    for(auto row : statements.statementsInDateRange())
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


void ViewModel::setEventListener(StatementTableModel &model)
{
    connect(&model, SIGNAL(currentChanged(QModelIndex,QModelIndex)), this, SLOT(tableIndexChanged(QModelIndex,QModelIndex)));
}

void ViewModel::initTableModels()
{
    initRules();
    initStatements();
}
