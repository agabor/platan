
#include "tablestructure.h"
#include <QStringList>
#include <QString>
#include <QRegularExpression>

QString removeRedundantWhitespaces(QString str);
QString removeQuote(QString str);
QString getFieldList(QString schema);
QString getTableName(QString schema);
bool parseFieldList(TableStructure &ts, QString field_list);
bool addField(TableStructure &ts, QString field);

TableStructure TableStructure::fromSchema(QString schema)
{
    schema = removeRedundantWhitespaces(schema);

    QString table_name = getTableName(schema);

    if (table_name.isEmpty())
        return Invalid();

    TableStructure ts(table_name);

    if(!parseFieldList(ts, getFieldList(schema)))
        return Invalid();

    return ts;
}

QString removeRedundantWhitespaces(QString str)
{
    return str.replace(QRegularExpression("\\s+"), " ");
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
    if (field_list.isEmpty())
        return false;

    QStringList fields = field_list.split(",");
    for(QString field : fields)
    {
        if (!addField(ts, field))
            return false;
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
    ts.addField(name, SQLType::parse(type));

    return true;
}


QString removeQuote(QString str)
{
    if (str.startsWith("\"") && str.endsWith("\""))
    {
        str.chop(1);
        return str.remove(0, 1);
    }
    return str;
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
