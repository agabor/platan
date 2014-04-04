#include "tablestructure.h"
#include <QStringList>
#include <QDebug>
#include <QRegularExpression>

using namespace std;

TableStructure::TableStructure()
{
    is_valid = true;
}

QString removeQuote(QString name)
{
    if (name.startsWith("\"") && name.endsWith("\""))
    {
        name.chop(1);
        return name.remove(0, 1);
    }
    return name;
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
        field = field.trimmed();
        field = field.replace(QRegularExpression("\\s+"), " ");
        auto name_type = field.split(" ");
        if (name_type.length() < 2)
        {
            is_valid = false;
            return;
        }
        QString name = name_type.at(0);
        QString type = field.remove(0, name.length() + 1);
        name = removeQuote(name);
        addField(name, type);
    }
}

bool TableStructure::operator ==(const TableStructure &other) const
{
    if (!isValid() || !other.isValid())
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
