#ifndef TABLESTRUCTURE_H
#define TABLESTRUCTURE_H

#include <QString>
#include <vector>
#include <utility>

class TableStructure
{
public:
    TableStructure();
    TableStructure(QString schema);
    bool operator ==(const TableStructure &other) const;
    bool operator !=(const TableStructure &other) const;
    void addField(QString name, QString type);
    bool isValid() const
    {
        return is_valid;
    }
private:
    std::vector<std::pair<QString, QString>> name_type_pairs;
    int fieldCount() const;
    bool is_valid;
};

#endif // TABLESTRUCTURE_H
