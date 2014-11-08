#ifndef QXLSDOCUMENTTEST_H
#define QXLSDOCUMENTTEST_H

#include <QtTest>

class QXLSDocumentTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void readNumbers();
    void readNumbers2();
};

#endif // QXLSDOCUMENTTEST_H
