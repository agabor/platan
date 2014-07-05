#ifndef VIEWMODEL_H
#define VIEWMODEL_H

#include <memory>

template <class Key, class T>
class QMap;

class Statements;
class StatementTableModel;

class ViewModel
{
public:
    ViewModel(Statements &statements);
    std::shared_ptr<StatementTableModel> getUncategorisedStatementsModel();
    std::shared_ptr<StatementTableModel> getAllStatements();
    std::shared_ptr<StatementTableModel> getStatementsForClass(int classIdx);
    void initStatementModels();
    void init();
private:
    QMap<int, std::shared_ptr<StatementTableModel>> classStatements;
    std::shared_ptr<StatementTableModel> uncategorisedStatements;
    std::shared_ptr<StatementTableModel> allStatements;
    Statements &statements;
};

#endif // VIEWMODEL_H
