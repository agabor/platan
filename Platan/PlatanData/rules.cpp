#include <QStringList>

#include <rules.h>
#include <rule.h>
#include <memory>

using namespace std;

Rules::Rules(SQLiteDB &db) : ruleMapper(db), db(db), nextId(0)
{
}

void Rules::insertRuleSilent(Rule &rule)
{
    rule.id = nextId++;
    QSharedPointer<Rule> p(new Rule(rule));
    newRules.push_back(p);
    append(p);
}

void Rules::insertRule(Rule& rule)
{
    insertRuleSilent(rule);
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
    {
        if (r.id >= nextId)
            nextId = r.id + 1;
        push_back(QSharedPointer<Rule>(new Rule(r)));
    }
}

void Rules::insertRules(QVector<Rule> rules)
{
    for (Rule r : rules)
    {
        insertRuleSilent(r);
    }
    emit dataChanged();
}

void Rules::removeRuleAt(int index)
{
    auto rule = at(index);
    int i = newRules.indexOf(rule);
    if (i  != -1)
        newRules.removeAt(i);
    else
        deletedRules.push_back(rule);
    removeAt(index);
    emit dataChanged();
}

void Rules::save()
{
    db.beginTransaction();

    for(auto r : deletedRules)
        ruleMapper.remove(*r);
    deletedRules.clear();

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
    return !deletedRules.isEmpty()  ||!newRules.isEmpty();
}
