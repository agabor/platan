#include "tablestructure.h"

using namespace std;

TableStructure::TableStructure()
{
}

bool TableStructure::operator ==(const TableStructure &other) const
{
    const int field_count = fieldCount();
    if (other.fieldCount() != field_count)
        return false;
    for (int i = 0; i < field_count; ++i)
    {
        auto &field = name_type_pairs[i];
        auto &field_other = other.name_type_pairs[i];

        if(field.first != field_other.first)
            return false;

        if(field.second != field_other.second)
            return false;
    }

    return true;

}

int TableStructure::fieldCount() const
{
    return name_type_pairs.size();
}
