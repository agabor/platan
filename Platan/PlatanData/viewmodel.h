#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <memory>

#include <QObject>
#include <QMap>

#include "statementtablemodel.h"
#include "ruletablemodel.h"

class Statements;
class Rules;

class ViewModel : QObject
{
    Q_OBJECT
public:
    ViewModel(Statements &statements, Rules &rules);
    std::shared_ptr<StatementTableModel> getUncategorisedStatementsModel();
    std::shared_ptr<StatementTableModel> getAllStatements();
    std::shared_ptr<StatementTableModel> getStatementsForClass(int classIdx);
    std::shared_ptr<RuleTableModel> getRuleTable();
    void initTableModels();
    void init();
private slots:
    void initRules();
    void initStatements();
private:
    void setEventListener(StatementTableModel &model);
    QMap<int, std::shared_ptr<StatementTableModel>> classStatements;
    std::shared_ptr<StatementTableModel> uncategorisedStatements;
    std::shared_ptr<StatementTableModel> allStatements;
    std::shared_ptr<RuleTableModel> ruleTable;
    Statements &statements;
    Rules &rules;
    std::shared_ptr<Statement> activeStatement;
};

#endif // VIEWMODEL_H
