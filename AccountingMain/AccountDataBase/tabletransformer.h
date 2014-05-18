#ifndef TABLETRANSFORMER_H
#define TABLETRANSFORMER_H

#include <simpletransformations.h>
#include <datetransformation.h>

#include <vector>

class StatementTableModel;
class CSVTableModel;

enum class ColumnType
{
    Amount,
    Type,
    Date,
    Payee,
    PayeeAccount,
    Description,
    None
};

class TableTransformer
{
public:
    TableTransformer();
    FloatTransformation Amount;
    IdentityTransFormation Type;
    DateTransformation Date;
    IdentityTransFormation Payee;
    IdentityTransFormation PayeeAccount;
    IdentityTransFormation Description;
    StatementTableModel *transform(QAbstractTableModel *model) const;
    void setColumnType(int column, ColumnType type);
    void removeColumnType(int column);
    ColumnType getColumnType(int column);
private:
    std::vector<TransformationBase*> transformations;
};

#endif // TABLETRANSFORMER_H
