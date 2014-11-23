#include <QStringList>

#include <rules.h>
#include <rule.h>


using namespace std;

Rules::Rules(SQLiteDB &db) : ruleMapper(db)
{
}

void Rules::insertRule(Rule rule)
{
    ruleMapper.insert(rule);
}

QVector<Rule> Rules::getRules()
{
    return ruleMapper.getAll();
}

void Rules::init()
{
    clear();
    for (Rule &r : ruleMapper.getAll())
        push_back(shared_ptr<Rule>(new Rule(r)));
}

void Rules::insertRules(QVector<Rule> rules)
{
    for (Rule r : rules)
        ruleMapper.insert(r);
}

void Rules::removeRuleAt(int index)
{
    deletedRules.push_back(at(index));
    removeAt(index);
    emit dataChanged();
}

void Rules::save()
{
    for(auto r : deletedRules)
        ruleMapper.deleteRule(*r);
}

QStringList Rules::typeList()
{
    QStringList result;
    result.push_back(QObject::tr("is"));
    result.push_back(QObject::tr("contains"));
    return result;
}
