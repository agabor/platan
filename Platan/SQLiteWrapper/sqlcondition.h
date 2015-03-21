#ifndef SQLCONDITION_H
#define SQLCONDITION_H

#include <QString>
#include <QVector>

class SimpleCondition;

class SQLCondition
{
public:
	virtual ~SQLCondition()
	{
	}

	static const SimpleCondition Empty;
    virtual QString toString() const = 0;
    SimpleCondition operator+(const SQLCondition &cond) const;
    virtual bool isEmpty() const;
};

class SimpleCondition : public SQLCondition
{
public:
    explicit SimpleCondition(const SQLCondition &_cond)
        : cond (_cond.toString())
    {
    }

    explicit SimpleCondition(QString _cond)
        : cond(_cond)
    {
    }

    QString toString() const override
    {
        return cond;
    }

    bool isEmpty() const override
    {
        return cond.isEmpty();
    }

private:
    const QString cond;
};


#endif // SQLCONDITION_H

