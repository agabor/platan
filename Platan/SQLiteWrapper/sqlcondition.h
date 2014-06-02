#ifndef SQLCONDITION_H
#define SQLCONDITION_H

#include <QString>
#include <QVector>

class SimpleCondition;

class SQLCondition
{
public:
    virtual QString toString() const = 0;
    SimpleCondition operator+(const SQLCondition &cond) const;
};

class SimpleCondition : public SQLCondition
{
public:
    SimpleCondition(QString _cond) :cond(_cond)
    {
    }

    QString toString() const
    {
        return cond;
    }

private:
    const QString cond;
};


#endif // SQLCONDITION_H

