#include <QFile>
#include <QByteArray>
#include <QTextStream>
#include <QString>

#include "csvanalyzertest.h"

#include "csvanalyser.h"


void CSVAnalyzerTest::postBankTest()
{
    QString srcDir(SRCDIR);
    QFile inFile(srcDir + "TestData\\postbank_sample.csv");
    inFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray input = inFile.readAll();
    QTextStream input_stream(&input);
    CSVAnalyser analyser(input_stream);
    QCOMPARE('\t', analyser.getSeparator());
    QCOMPARE('\0', analyser.getQuote());
    QCOMPARE(8, analyser.getLinesToSkip());
    QCOMPARE(true, analyser.areHeadersInFirstLine());
}

void CSVAnalyzerTest::sparkasseTest()
{
    QString srcDir(SRCDIR);
    QFile inFile(srcDir + "TestData\\sample_MT940.csv");
    inFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray input = inFile.readAll();
    QTextStream input_stream(&input);
    CSVAnalyser analyser(input_stream);
    QCOMPARE(';', analyser.getSeparator());
    QCOMPARE('\"', analyser.getQuote());
    QCOMPARE(0, analyser.getLinesToSkip());
    QCOMPARE(true, analyser.areHeadersInFirstLine());
}
