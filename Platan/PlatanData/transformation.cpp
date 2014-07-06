
#include <transformation.h>


TransformationBase::TransformationBase() : column(-1)
{

}

int TransformationBase::getColumn() const
{
    return column;
}

void TransformationBase::setColumn(int value)
{
    column = value;
}

bool TransformationBase::configured() const
{
    return column != -1;
}

QVector<QString> TransformationBase::getErrorList() const
{
    return errorList;
}


void TransformationBase::clearErrorList() const
{
    errorList.clear();
}

QString TransformationBase::getData(int row, QAbstractTableModel *table) const
{
    QVariant Data = table->data(table->index(row, column), Qt::UserRole);
    QString data_string = Data.toString();

    return data_string;
}


QString TransformationBase::errorListString() const
{
    QString values;
    for (QString val : errorList)
    {
        if (!values.isEmpty())
            values += ", ";
        values += val;
    }

    return values;
}

