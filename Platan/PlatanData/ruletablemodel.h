#ifndef RULETABLEMODEL_H
#define RULETABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>

#include <memory>

class Rule;

class RuleTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit RuleTableModel(QObject *parent = 0);    
    // QAbstractItemModel interface
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setData(QVector<std::shared_ptr<Rule> > rows);
protected:
    QVector<std::shared_ptr<Rule>> Rows;
};

#endif // RULETABLEMODEL_H
