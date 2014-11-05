#ifndef QXLSDOCUMENT_H
#define QXLSDOCUMENT_H

#include <QVariant>
#include <QString>
#include <QVector>

#include "xlsdocument.h"

class QXLSSheet
{
public:
    QXLSSheet(QString name);
    QVariant getData(int row, int column) const;
    QString getName() const;
    int getColumnCount() const;
    int getRowCount() const;
    void addRow(QVector<QVariant> row);
private:
    QVector<QVector<QVariant>> data;
    int columnCount;
    QString name;
};

class QXLSDocument
{
public:
    QXLSDocument(QString fileName);
    const QVector<QXLSSheet> getSheets() const;
private:
    QVector<QXLSSheet> sheets;
    XLSDocument doc;
};

#endif // QXLSDOCUMENT_H
