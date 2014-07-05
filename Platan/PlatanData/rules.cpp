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
