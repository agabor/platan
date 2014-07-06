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

std::shared_ptr<RuleTableModel> ViewModel::getRuleTable()
{
    return ruleTable;
}


void ViewModel::initTableModels()
{
    ruleTable->setData(rules);
    allStatements->setData(statements);
    QVector<shared_ptr<Statement>> uncategorised;
    QMap<int, QVector<shared_ptr<Statement>>> categories;
    for(int i : classStatements.keys())
        categories.insert(i, QVector<shared_ptr<Statement>>());

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
