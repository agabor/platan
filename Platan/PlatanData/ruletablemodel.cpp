#include "ruletablemodel.h"
#include <rule.h>
#include <statements.h>

RuleTableModel::RuleTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

int RuleTableModel::rowCount(const QModelIndex &) const
{
    return Rows.size();
}

int RuleTableModel::columnCount(const QModelIndex &) const
{
    return 4;
}

QVariant RuleTableModel::data(const QModelIndex &index, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant::Invalid;
    const Rule r = *Rows[index.row()];
    switch (index.column()) {
    case 0:
    {
        QStringList columnList = Statements::columnList();
        int column_idx = static_cast<int>(r.column());
        if (column_idx < 0 || column_idx >= columnList.length())
            return QString();
        return columnList.at(column_idx);
    }
    case 1:
        switch(r.type())
        {
        case Rule::Type::Is:
            return QString{"is"};
        case Rule::Type::Contains:
            return QString{"contains"};
        }
        return QVariant::Invalid;

    case 2:
        return r.value();
    case 3:
    {
        QStringList categoryList = Statements::categoryList();
        if (r.category() < 0 || r.category() >= categoryList.length())
            return QString();
        return categoryList.at(r.category());
    }
    default:
        return QVariant::Invalid;
    }
}

QVariant RuleTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant::Invalid;

    if(orientation == Qt::Horizontal)
    {
        switch(section)
        {
        case 0:
            return QString("Column");
        case 1:
            return QString("Type");
        case 2:
            return QString("Value");
        case 3:
            return QString("Category");
        default:
            return QString("");
        }
    }

    return QString::number(section);
}

void RuleTableModel::setData(QVector<QSharedPointer<Rule> > rows)
{
    Rows = rows;
    emit layoutChanged ();
}
