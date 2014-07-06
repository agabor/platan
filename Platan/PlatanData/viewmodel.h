#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <memory>

template <class Key, class T>
class QMap;

class Statements;
class StatementTableModel;
class Rules;
class RuleTableModel;

class ViewModel
{
public:
    ViewModel(Statements &statements, Rules &rules);
    std::shared_ptr<StatementTableModel> getUncategorisedStatementsModel();
    std::shared_ptr<StatementTableModel> getAllStatements();
    std::shared_ptr<StatementTableModel> getStatementsForClass(int classIdx);
    std::shared_ptr<RuleTableModel> getRuleTable();
    void initTableModels();
    void init();
private:
    QMap<int, std::shared_ptr<StatementTableModel>> classStatements;
    std::shared_ptr<StatementTableModel> uncategorisedStatements;
    std::shared_ptr<StatementTableModel> allStatements;
    std::shared_ptr<RuleTableModel> ruleTable;
    Statements &statements;
    Rules &rules;
};

#endif // VIEWMODEL_H
