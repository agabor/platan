#ifndef TABLETRANSFORMER_H
#define TABLETRANSFORMER_H

#include <simpletransformations.h>
#include <datetransformation.h>

class StatementTableModel;
class CSVTableModel;

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
    StatementTableModel *transform(CSVTableModel *model) const;
};

#endif // TABLETRANSFORMER_H
