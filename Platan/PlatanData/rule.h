#ifndef RULE_H
#define RULE_H

#include <QString>

class Rule
{
public:
    Rule(int _column, QString _value, int _category)
        :column(_column), value(_value), category(_category)
    {

    }

    Rule()
    {

    }

    int column;
    QString value;
    int category;
};


#endif // RULE_H
