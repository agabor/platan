#ifndef RULES_H
#define RULES_H

#include <QVector>

#include <memory>

#include <rulemapper.h>


class Rule;

class Rules : public QVector<std::shared_ptr<Rule> >
{
public:
    Rules(SQLiteDB &db);
    void insertRule(Rule rule);
    QVector<Rule> getRules();
    void init();
    void insertRules(QVector<Rule> rules);
private:
    RuleMapper ruleMapper;
};

#endif // RULES_H
