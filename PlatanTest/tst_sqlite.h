#ifndef TST_SQLITE_H
#define TST_SQLITE_H

#include <QtTest>

class SQLiteTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void equalsQDate();
    void equalsInt();
    void createSQLInsert1();
    void createSQLInsert2();
    void createSQLUpdate1();
    void createSQLUpdate2();
    void createSQLSelect1();
    void createSQLSelect2();
    void createSQLSelect3();
    void tableStructureEquality();
    void parseSchema();
    void parseSchema2();
};


#endif // TST_SQLITE_H
