#include "tablestructure.h"
#include <QStringList>
#include <QDebug>
#include <QRegularExpression>

using namespace std;

TableStructure::TableStructure()
{
    is_valid = true;
}

TableStructure::TableStructure(QString schema)
{
    is_valid = true;
    QStringList parts = schema.split("(");
    if (parts.length() != 2)
    {
        is_valid = false;
        return;
    }
    QString field_list = parts.at(1);
    if (field_list.length() == 0 || field_list.at(field_list.length() - 1) != ')')
    {
        is_valid = false;
        return;
    }


    field_list.chop(1);
    QStringList fields = field_list.split(",");
    for(QString field : fields)
    {
        field = field.trimmed().replace(QRegularExpression("\\s+"), " ");
        auto name_type = field.split(" ");
        if (name_type.length() != 2)
        {
            is_valid = false;
            return;
        }
        addField(name_type.at(0), name_type.at(1));
    }
}

bool TableStructure::operator ==(const TableStructure &other) const
{
    if (!is_valid || !other.is_valid)
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
