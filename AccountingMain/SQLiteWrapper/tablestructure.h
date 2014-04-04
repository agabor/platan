#ifndef TABLESTRUCTURE_H
#define TABLESTRUCTURE_H

#include <QString>
#include <vector>
#include <utility>

class TableStructure
{
public:
    TableStructure();
    bool operator ==(const TableStructure &other) const;
    bool operator !=(const TableStructure &other) const;
    void addField(QString name, QString type);
private:
    std::vector<std::pair<QString, QString>> name_type_pairs;
    int fieldCount() const;
};

#endif // TABLESTRUCTURE_H
