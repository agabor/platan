#ifndef RULES_H
#define RULES_H

#include <QVector>
#include <QObject>

#include <QSharedPointer>

#include <rulemapper.h>
#include <sqlitedb.h>

class Rule;
class QStringList;

class Rules : public QObject, public QVector<QSharedPointer<Rule>>
{
    Q_OBJECT
public:
	explicit Rules(SQLiteDB &db);
    void insertRule(Rule &rule);
    QVector<Rule> getRules();
    void init();
    void insertRules(QVector<Rule> rules);
    void removeRuleAt(int index);
    void save();
    static QStringList typeList();
    bool changed() const;
signals:
    void dataChanged();
private:
    void insertRuleSilent(Rule &rule);
    int nextId;
    RuleMapper ruleMapper;
    SQLiteDB &db;
    QVector<QSharedPointer<Rule>> deletedRules;
    QVector<QSharedPointer<Rule>> newRules;
};

#endif // RULES_H
