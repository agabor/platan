#include <QStringList>

#include <rules.h>
#include <rule.h>
#include <memory>

using namespace std;

Rules::Rules(SQLiteDB &db) : ruleMapper(db), db(db)
{
}

void Rules::insertRule(Rule rule)
{
    auto p = make_shared<Rule>(rule);
    newRules.push_back(p);
    append(p);
    emit dataChanged();
}

QVector<Rule> Rules::getRules()
{
    return ruleMapper.getAll();
}

void Rules::init()
{
    clear();
    for (Rule &r : getRules())
        push_back(shared_ptr<Rule>(new Rule(r)));
}

void Rules::insertRules(QVector<Rule> rules)
{
    for (Rule r : rules)
    {
        auto p = make_shared<Rule>(r);
        newRules.push_back(p);
        append(p);
    }
    emit dataChanged();
}

void Rules::removeRuleAt(int index)
{
    deletedRules.push_back(at(index));
    removeAt(index);
    emit dataChanged();
}

void Rules::save()
{
    db.beginTransaction();

    for(auto r : deletedRules)
        ruleMapper.remove(*r);
    deletedRules.clear();

    for(auto r : changedRules)
        ruleMapper.update(*r);
    changedRules.clear();

    for(auto r : newRules)
        ruleMapper.insert(*r);
    newRules.clear();

    db.endTransaction();
}

QStringList Rules::typeList()
{
    QStringList result;
    result.push_back(QObject::tr("is"));
    result.push_back(QObject::tr("contains"));
    return result;
}

bool Rules::changed() const
{
    return !deletedRules.isEmpty() ||!changedRules.isEmpty() ||!newRules.isEmpty();
}
