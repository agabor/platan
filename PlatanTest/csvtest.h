#ifndef CSVTEST_H
#define CSVTEST_H

#include <QtTest>
#include <QVector>
#include <QString>

class CSVTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void readCSV1();
private:
    void testCSV(char separator, QVector<QVector<QString>> data, char quote);
};

#endif // CSVTEST_H
