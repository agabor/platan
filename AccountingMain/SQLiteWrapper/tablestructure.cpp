#include "tablestructure.h"
#include <QStringList>
#include <QDebug>
#include <QRegularExpression>

using namespace std;
QString removeQuote(QString name);
QString getFieldList(QString schema);


TableStructure::TableStructure()
{
    is_valid = true;
}

TableStructure::TableStructure(QString schema)
{
    is_valid = true;

    QString field_list = getFieldList(schema);
    if (field_list.isEmpty())
        is_valid = false;
    else
        parseFieldList(field_list);
}

void TableStructure::parseFieldList(QString field_list)
{
    QStringList fields = field_list.split(",");
    for(QString field : fields)
    {
        if (!addField(field))
        {
            is_valid = false;
            return;
        }

    }
}

bool TableStructure::addField(QString field)
{
    field = field.trimmed();
    auto name_type = field.split(" ");

    if (name_type.length() < 2)
        return false;

    QString name = name_type.at(0);
    QString type = field.remove(0, name.length() + 1);
    name = removeQuote(name);
    addField(name, type);

    return true;
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

QString removeQuote(QString name)
{
    if (name.startsWith("\"") && name.endsWith("\""))
    {
        name.chop(1);
        return name.remove(0, 1);
    }
    return name;
}

QString getFieldList(QString schema)
{
    QStringList parts = schema.split("(");
    if (parts.length() != 2)
        return QString();

    QString field_list = parts.at(1);
    if (field_list.length() == 0 || field_list.at(field_list.length() - 1) != ')')
        return QString();

    field_list.chop(1);
    return field_list.replace(QRegularExpression("\\s+"), " ");
}
