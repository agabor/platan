#include "sqlitedate.h"


enum class RelOp
{
    G, GE, E, LE, L
};

class DateCondition : public SQLCondition
{
public:
    DateCondition(SQLiteDate _date, QString _field, RelOp _op)
    {
        date = _date;
        field = _field;
        op = _op;
    }

    QString toString() const
    {
        QString opStr;
        switch(op)
        {
        case RelOp::G:
            opStr = ">";
            break;
        case RelOp::GE:
            opStr = ">=";
            break;
        case RelOp::E:
            opStr = "=";
            break;
        case RelOp::LE:
            opStr = "<=";
            break;
        case RelOp::L:
            opStr = "<";
            break;
        }

        return QString("%1 %2 %3").arg(field).arg(opStr).arg(date.toInt());
    }

private:
    SQLiteDate date;
    QString field;
    RelOp op;
};

DateCondition SQLiteDate::greater(QString field) const
{
    return DateCondition(*this, field, RelOp::GE);
}

DateCondition SQLiteDate::less(QString field) const
{
    return DateCondition(*this, field, RelOp::LE);
}

