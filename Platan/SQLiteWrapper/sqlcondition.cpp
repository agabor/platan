#include "sqlcondition.h"

const SimpleCondition SQLCondition::Empty("");

SimpleCondition SQLCondition::operator+(const SQLCondition &cond) const
{
    return SimpleCondition(toString() + " AND " + cond.toString());
}

bool SQLCondition::isEmpty() const
{
    return false;
}

