#ifndef RULE_H
#define RULE_H

#include <QString>

template <typename T>
class QVector;

class Rule
{
public:
    Rule(int _column, QString _value, int _category);
    Rule();

    int column;
    QString value;
    int category;

    void insert() const;
    static QVector<Rule> getAll(int column = -1);
};


#endif // RULE_H
