#include <QDebug>

#include <cmath>

#include "qxlsdocumenttest.h"
#include "qxlsdocument.h"


void QXLSDocumentTest::readNumbers()
{
    QXLSDocument doc("E:\\platan\\sample\\numbers.xls");
    QCOMPARE(1, doc.getSheets().count());
    QXLSSheet sheet = doc.getSheets()[0];
    QCOMPARE(2000, sheet.getRowCount());
    QCOMPARE(10, sheet.getColumnCount());
    double val[10];
    val[0] = -100000000.0;
    for (int i = 1; i < 10; ++i)
        val[i] = val[i-1] + 20000000.0;
    auto cell = doc.doc.getSheet(0)->getRow(1356)->getCell(6);
    for (int r = 0; r < sheet.getRowCount(); ++r)
    {
        for (int c = 6; c < 7; ++c)
        {
            QVariant d = sheet.getData(r, c);
            QVERIFY(fabs(val[c]- d.toDouble()) < 0.001);
        }
        for (int i = 0; i < 10; ++i)
            val[i] = val[i] + 10000.0;
    }
}

void QXLSDocumentTest::readNumbers2()
{
    QXLSDocument doc("E:\\platan\\sample\\doubles.xls");
    QCOMPARE(1, doc.getSheets().count());
    QXLSSheet sheet = doc.getSheets()[0];
    QCOMPARE(2000, sheet.getRowCount());
    QCOMPARE(10, sheet.getColumnCount());
    double val[10];
    val[0] = -100000000.01;
    for (int i = 1; i < 10; ++i)
        val[i] = val[i-1] + 20000000.0;
    auto cell = doc.doc.getSheet(0)->getRow(1356)->getCell(6);
    for (int r = 0; r < sheet.getRowCount(); ++r)
    {
        for (int c = 6; c < 7; ++c)
        {
            QVariant d = sheet.getData(r, c);
            QVERIFY(fabs(val[c]- d.toDouble()) < 0.001);
        }
        for (int i = 0; i < 10; ++i)
            val[i] = val[i] + 10000.0;
    }
}
