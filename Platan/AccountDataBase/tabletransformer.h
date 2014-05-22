#ifndef TABLETRANSFORMER_H
#define TABLETRANSFORMER_H

#include <simpletransformations.h>
#include <datetransformation.h>

#include <QVector>

#include <memory>
#include <statementtablemodel.h>

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
    std::shared_ptr<StatementTableModel> transform(QAbstractTableModel *model) const;
    void setColumnType(int column, ColumnType type);
    void removeColumnType(int column);
    ColumnType getColumnType(int column);
    QVector<ColumnType> unsetMandatoryFields() const;
    QVector<ColumnType> unsetNotMandatoryFields() const;
    bool errorInImport() const;
    QString getErrorMessage() const;
private:
    QVector<TransformationBase*> transformations;
};

#endif // TABLETRANSFORMER_H
