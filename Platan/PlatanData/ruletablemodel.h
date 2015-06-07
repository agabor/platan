#ifndef RULETABLEMODEL_H
#define RULETABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QSharedPointer>

class Rule;

class RuleTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RuleTableModel(QObject *parent = 0);    
    // QAbstractItemModel interface
    int rowCount(const QModelIndex &) const override;
    int columnCount(const QModelIndex &) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void setData(QVector<QSharedPointer<Rule> > rows);
protected:
    QVector<QSharedPointer<Rule>> Rows;
};

#endif // RULETABLEMODEL_H
