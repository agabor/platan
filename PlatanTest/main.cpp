#include "tst_sqlite.h"

int main(int argc, char *argv[])
{
    int status = 0;
    SQLiteTest t1;
    status |= QTest::qExec(&t1, argc, argv);
    return status;
}

//#include "tst_sqlite.moc"
