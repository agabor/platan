#include "tabletransformer.h"
#include <statementtablemodel.h>
#include <csvtablemodel.h>
#include <vector>
#include <utility>
#include <QVector>

using namespace std;

TableTransformer::TableTransformer()
{
    transformations.push_back(&Amount);
    transformations.push_back(&Type);
    transformations.push_back(&Date);
    transformations.push_back(&Payee);
    transformations.push_back(&PayeeAccount);
    transformations.push_back(&Description);
}

QVector<Statement> TableTransformer::transform(QAbstractTableModel *model) const
{
    for (TransformationBase *tr : transformations)
        tr->clearErrorList();

    QVector<Statement> rows;
    for(int r = 0; r < model->rowCount(); ++r)
    {
        Statement row;
        if (Amount.configured())
            row.amount = Amount.apply(model, r);
        if (Type.configured())
            row.type = Type.apply(model, r);
        if (Date.configured())
            row.date = Date.apply(model, r);
        if (Payee.configured())
            row.payee = Payee.apply(model, r);
        if (PayeeAccount.configured())
            row.payeeAccount = PayeeAccount.apply(model, r);
        if (Description.configured())
            row.description = Description.apply(model, r);
        row.category = 0;
        rows.push_back(row);
    }
    return rows;
}

void TableTransformer::removeColumnType(int column)
{
    for (TransformationBase *tr : transformations)
    {
        if (tr->getColumn() == column)
        {
            tr->setColumn(-1);
            break;
        }
    }
}

ColumnType TableTransformer::getColumnType(int column) const
{
    int idx = 0;
    for (TransformationBase *tr : transformations)
    {
        if (tr->getColumn() == column)
            return (ColumnType)idx;
        ++idx;
    }
    return ColumnType::None;
}

QVector<ColumnType> TableTransformer::unsetMandatoryFields() const
{
    QVector<ColumnType> result;

    if (!Amount.configured())
        result.push_back(ColumnType::Amount);
    if (!Date.configured())
        result.push_back(ColumnType::Date);
    if (!Payee.configured())
        result.push_back(ColumnType::Payee);
    if (!PayeeAccount.configured())
        result.push_back(ColumnType::PayeeAccount);

    return result;
}

QVector<ColumnType> TableTransformer::unsetNotMandatoryFields() const
{
    QVector<ColumnType> result;

    if (!Type.configured())
        result.push_back(ColumnType::Type);
    if (!Description.configured())
        result.push_back(ColumnType::Description);

    return result;
}

bool TableTransformer::errorInImport() const
{
    for (TransformationBase *tr : transformations)
        if (!tr->getErrorList().isEmpty())
            return true;
    return false;
}

QString TableTransformer::getErrorMessage() const
{
    QString result;
    for (TransformationBase *tr : transformations)
        if (!tr->getErrorList().isEmpty())
        {
            if (!result.isEmpty())
                result += "\n";
            result += tr->getErrorMessage();
        }
    return result;
}


void TableTransformer::setColumnType(int column, ColumnType type)
{
    removeColumnType(column);
    if (type != ColumnType::None)
        transformations[(int)type]->setColumn(column);
}


QString toString(ColumnType type)
{
    switch (type)
    {
     case ColumnType::None:
        return QObject::tr("None");
     case ColumnType::Amount:
        return QObject::tr("Amount");
     case ColumnType::Type:
        return QObject::tr("Type");
     case ColumnType::Date:
        return QObject::tr("Date");
     case ColumnType::Payee:
        return QObject::tr("Payee");
     case ColumnType::PayeeAccount:
        return QObject::tr("Payee account");
     case ColumnType::Description:
        return QObject::tr("Description");
    default:
        return QString();
    }
}
