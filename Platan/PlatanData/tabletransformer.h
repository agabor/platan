#ifndef TABLETRANSFORMER_H
#define TABLETRANSFORMER_H

#include <simpletransformations.h>
#include <datetransformation.h>

#include <QVector>

class Statement;
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

QString toString(ColumnType type);

class TableTransformer
{
public:
    TableTransformer();
    QVector<Statement> transform(CSVTableModel *model) const;
    void setColumnType(int column, ColumnType type);
    void removeColumnType(int column);
    ColumnType getColumnType(int column) const;
    QVector<ColumnType> unsetMandatoryFields() const;
    QVector<ColumnType> unsetNotMandatoryFields() const;
    bool errorInImport() const;
    QString getErrorMessage() const;

    FloatTransformation Amount;
    IdentityTransFormation Type;
    DateTransformation Date;
    IdentityTransFormation Payee;
    IdentityTransFormation PayeeAccount;
    IdentityTransFormation Description;
private:
    QVector<TransformationBase*> transformations;
};

#endif // TABLETRANSFORMER_H
