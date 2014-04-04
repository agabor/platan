
#include "tablestructure.h"
#include <QStringList>
#include <QString>
#include <QRegularExpression>

QString removeQuote(QString name);
QString getFieldList(QString schema);
QString getTableName(QString schema);
bool parseFieldList(TableStructure &ts, QString field_list);
bool addField(TableStructure &ts, QString field);

TableStructure TableStructure::fromSchema(QString schema)
{
    schema = schema.replace(QRegularExpression("\\s+"), " ");

    QString table_name = getTableName(schema);

    if (table_name.isEmpty())
        return TableStructure::Invalid();

    TableStructure ts(table_name);

    QString field_list = getFieldList(schema);
    if (field_list.isEmpty())
        return TableStructure::Invalid();
    else
        if(!parseFieldList(ts, field_list))
            return TableStructure::Invalid();
    return ts;
}

QString getTableName(QString schema)
{
    QString prefix{"CREATE TABLE "};
    if (!schema.startsWith(prefix))
        return QString{};
    schema = schema.remove(0, prefix.length());
    QStringList parts = schema.split(" ");
    if (parts.length() == 0)
        return QString{};
    return parts.at(0);
}

bool parseFieldList(TableStructure &ts, QString field_list)
{
    QStringList fields = field_list.split(",");
    for(QString field : fields)
    {
        if (!addField(ts, field))
        {
            return false;
        }
    }
    return true;
}

bool addField(TableStructure &ts, QString field)
{
    field = field.trimmed();
    auto name_type = field.split(" ");

    if (name_type.length() < 2)
        return false;

    QString name = name_type.at(0);
    QString type = field.remove(0, name.length() + 1);
    name = removeQuote(name);
    ts.addField(name, type);

    return true;
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
        return QString{};

    QString field_list = parts.at(1);
    if (field_list.length() == 0 || field_list.at(field_list.length() - 1) != ')')
        return QString{};

    field_list.chop(1);
    return field_list;
}
