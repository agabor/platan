#include <QString>
#include <QtTest>
#include <../AccountingMain/SQLiteWrapper/sqlitedate.h>
#include <../AccountingMain/SQLiteWrapper/sqlinsert.h>
#include <../AccountingMain/SQLiteWrapper/sqlupdate.h>
#include <../AccountingMain/SQLiteWrapper/sqlselect.h>

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
};

void SQLiteTest::equalsQDate()
{
    QDate qDate(2014, 2, 23);
    SQLiteDate sqlDate(qDate);
    QCOMPARE((QDate)sqlDate, qDate);
}

void SQLiteTest::equalsInt()
{
    for (int i = 0; i < 100; ++i)
    {
        int day = i * 101;
        SQLiteDate sqlDate(day);
        QCOMPARE(sqlDate.toInt(), day);
    }
}

void SQLiteTest::createSQLInsert1()
{
    SQLInsert insert("Cars");
    insert.set("color", "blue");
    insert.set("brand", "volkswagen");
    QCOMPARE(insert.toString(), QString("INSERT INTO Cars (color, brand) VALUES (\"blue\", \"volkswagen\")"));
}

void SQLiteTest::createSQLInsert2()
{
    SQLInsert insert("Cars");
    insert.set("varrianty", 2);
    insert.set("brand", "volkswagen");
    QCOMPARE(insert.toString(), QString("INSERT INTO Cars (varrianty, brand) VALUES (2, \"volkswagen\")"));
}

void SQLiteTest::createSQLUpdate1()
{
    SQLUpdate update("Cars");
    update.set("color", "red");
    update.set("brand", "opel");

    QCOMPARE(update.toString(), QString("UPDATE Cars SET color = \"red\", brand = \"opel\""));
}


void SQLiteTest::createSQLUpdate2()
{
    SQLUpdate update("Cars");
    update.set("color", "red");
    update.set("brand", "opel");
    update.where("color = \"blue\"");
    update.where("brand = \"volkswagen\"");

    QCOMPARE(update.toString(), QString("UPDATE Cars SET color = \"red\", brand = \"opel\" WHERE color = \"blue\" AND brand = \"volkswagen\""));
}

void SQLiteTest::createSQLSelect1()
{
    SQLSelect select{"Cars"};
    select.field("color");
    select.field("brand");

    QCOMPARE(select.toString(), QString("SELECT color, brand FROM Cars"));
}

void SQLiteTest::createSQLSelect2()
{
    SQLSelect select{"Cars"};
    select.field("color");
    select.field("brand");
    select.where("color = \"blue\"");

    QCOMPARE(select.toString(), QString("SELECT color, brand FROM Cars WHERE color = \"blue\""));

}


void SQLiteTest::createSQLSelect3()
{
    SQLSelect select{"Cars"};
    select.field("color");
    select.field("brand");
    select.where("color = \"blue\"");
    select.groupBy("color");

    QCOMPARE(select.toString(), QString("SELECT color, brand FROM Cars WHERE color = \"blue\" GROUP BY color"));

}

QTEST_APPLESS_MAIN(SQLiteTest)

#include "tst_sqlite.moc"
