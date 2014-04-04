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
    inline static TableStructure Invalid();

    bool operator ==(const TableStructure &other) const;
    bool operator !=(const TableStructure &other) const;
    void addField(QString name, QString type);
    inline bool isValid() const;
    inline QString getName() const;
    QString sqlCommand() const;
private:
    std::vector<std::pair<QString, QString>> name_type_pairs;
    int fieldCount() const;
    bool is_valid;
    QString name;
};

//Inline functions

inline TableStructure TableStructure::Invalid()
{
    TableStructure iv{QString{}};
    iv.is_valid = false;
    return iv;
}

inline bool TableStructure::isValid() const
{
    return is_valid;
}

inline QString TableStructure::getName() const
{
    return name;
}

#endif // TABLESTRUCTURE_H
