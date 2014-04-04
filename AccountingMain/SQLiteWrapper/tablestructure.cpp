#include "tablestructure.h"
#include <QStringList>
#include <QRegularExpression>

using namespace std;


TableStructure::TableStructure(QString name)
{
    this->name = name;
    is_valid = true;
}


bool TableStructure::operator ==(const TableStructure &other) const
{
    if (!isValid() || !other.isValid())
        return false;

    if (name != other.name)
        return false;

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

bool TableStructure::operator !=(const TableStructure &other) const
{
    return !(*this == other);
}

void TableStructure::addField(QString name, QString type)
{
    name_type_pairs.push_back(pair<QString, QString>(name, type));
}

int TableStructure::fieldCount() const
{
    return name_type_pairs.size();
}


QString TableStructure::sqlCommand() const
{
    QString sql{"CREATE TABLE %1 (%2)"};
    QString field_list;
    for(auto &field : name_type_pairs)
    {
        if (!field_list.isEmpty())
            field_list +=", ";
        field_list += QString("%1 %2").arg(field.first, field.second);
    }
    return sql.arg(name, field_list);
}
