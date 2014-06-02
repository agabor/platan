#include "sqlcondition.h"



SimpleCondition SQLCondition::operator+(const SQLCondition &cond) const
{
    return SimpleCondition(toString() + " AND " + cond.toString());
}

