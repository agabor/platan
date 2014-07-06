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
    return 3;
}

QVariant RuleTableModel::data(const QModelIndex &index, int) const
{
    Rule r = *Rows[index.row()];
    switch (index.column()) {
    case 0:
    {
        QStringList columnList = Statements::columnList();
        if (r.column < 0 || r.column >= columnList.length())
            return QString();
        return columnList.at(r.column);
    }
    case 1:
        return r.value;
    case 2:
    {
        QStringList categoryList = Statements::categoryList();
        if (r.category < 0 || r.category >= categoryList.length())
            return QString();
        return categoryList.at(r.category);
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
            return QString("Value");
        case 2:
            return QString("Category");
        default:
            return QString("");
        }
    }

    return QString::number(section);
}

void RuleTableModel::setData(QVector<std::shared_ptr<Rule> > rows)
{
    Rows = rows;
    emit layoutChanged ();
}
