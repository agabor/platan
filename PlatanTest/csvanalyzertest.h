#ifndef CSVANALYZERTEST_H
#define CSVANALYZERTEST_H

#include <QObject>
#include <QtTest>


class CSVAnalyzerTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void postBankTest();
    void sparkasseTest();

};

#endif // CSVANALYZERTEST_H
