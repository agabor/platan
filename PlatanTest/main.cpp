#include "tst_sqlite.h"
#include "csvtest.h"
#include "qxlsdocumenttest.h"

int main(int argc, char *argv[])
{
    int status = 0;
    SQLiteTest t1;
    status |= QTest::qExec(&t1, argc, argv);
    CSVTest t2;
    status |= QTest::qExec(&t2, argc, argv);
    QXLSDocumentTest t3;
    status |= QTest::qExec(&t3, argc, argv);
    return status;
}

//#include "tst_sqlite.moc"
