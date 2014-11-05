#include "qxlsdocument.h"

#include <QDate>

double asNumber189(const XLSCell* cell)
{
    int p30 = 1 << 30;
    int p23 = 1 << 23;
    int p29 = 1 << 29;
    bool hundred_hack = false;
    auto d = cell->d;
    if (d <= p29 && d > p23)
    {
        d *= 100;
        hundred_hack = true;
    }
    if (d > p29)
        d -= p30;
    if (hundred_hack)
        d /= 100;
    return d;
}

double asNumber515(const XLSCell* cell)
{
    int p30 = 1 << 30;
    int p29 = 1 << 29;
    auto d = cell->d;
    if (d > p29)
        d -= p30;
    return d;
}

QDate start(1899, 12, 30);
QVariant getData(XLSCell* cell)
{
    switch(cell->type)
    {
    case 638:
        if (cell->d > 200 * 365)
            return asNumber515(cell);
        return  start.addDays(cell->d);
    case 253:
    case 513:
    case 190:
        return QString(cell->str);
    case 189:
        return asNumber189(cell);
    case 515:
        return asNumber515(cell);
    default:
        return QString("(%1, %2, %3, %4)")
                .arg(cell->type)
                .arg(cell->str)
                .arg(cell->d)
                .arg(cell->l);
    }
}

QXLSDocument::QXLSDocument(QString fileName)
    : doc(fileName.toStdString().c_str())
{
    for(int i = 0; i < doc.getSheetCount(); ++i)
    {
        auto s = doc.getSheet(i);
        QXLSSheet qs(s->getName());
        for (int ir = 0; ir < s->getRowCount(); ++ir)
        {
            auto r = s->getRow(ir);
            QVector<QVariant> row;
            for (int c = 0; c < r->getCellCount(); ++c)
            {
                auto cell = r->getCell(c);
                row.append(getData(cell));
            }
        }
    }
}

const QVector<QXLSSheet> QXLSDocument::getSheets() const
{
    return sheets;
}


QXLSSheet::QXLSSheet(QString name)
    : name(name), columnCount(0)
{

}

QVariant QXLSSheet::getData(int row, int column) const
{
    if (row < 0 || row >= data.count())
        return QVariant::Invalid;
    auto r = data[row];
    if (column < 0 || column > r.count())
        return QVariant::Invalid;
    return r[column];
}

int QXLSSheet::getColumnCount() const
{
    return columnCount;
}

int QXLSSheet::getRowCount() const
{
    return data.count();
}

void QXLSSheet::addRow(QVector<QVariant> row)
{
    data.append(row);
    if (row.count() > columnCount)
        columnCount = row.count();
}
