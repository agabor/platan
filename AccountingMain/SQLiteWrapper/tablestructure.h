#ifndef TABLESTRUCTURE_H
#define TABLESTRUCTURE_H

#include <QString>
#include <vector>
#include <utility>

class TableStructure
{
public:
    TableStructure(QString name);
    static TableStructure fromSchema(QString schema);
    static TableStructure Invalid()
    {
        TableStructure iv{""};
        iv.is_valid = false;
        return iv;
    }

    bool operator ==(const TableStructure &other) const;
    bool operator !=(const TableStructure &other) const;
    void addField(QString name, QString type);
    bool isValid() const
    {
        return is_valid;
    }
    QString getName() const
    {
        return name;
    }
private:
    std::vector<std::pair<QString, QString>> name_type_pairs;
    int fieldCount() const;
    bool is_valid;
    QString name;
};

#endif // TABLESTRUCTURE_H
