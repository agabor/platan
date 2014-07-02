#include <rules.h>
#include <rule.h>

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
