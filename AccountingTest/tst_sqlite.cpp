#include <QString>
#include <QtTest>
#include <../AccountingMain/SQLiteWrapper/sqlitedate.h>
#include <../AccountingMain/SQLiteWrapper/sqlinsert.h>
#include <../AccountingMain/SQLiteWrapper/sqlupdate.h>
#include <../AccountingMain/SQLiteWrapper/sqlselect.h>
#include <../AccountingMain/SQLiteWrapper/tablestructure.h>
#include <QSqlQuery>
#include <string>

using namespace std;

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
    insert.set("color", QString("blue"));
    insert.set("brand", QString("volkswagen"));
    QCOMPARE(insert.toString(), string("INSERT INTO Cars (color, brand) VALUES (\"blue\", \"volkswagen\")"));
}

void SQLiteTest::createSQLInsert2()
{
    SQLInsert insert("Cars");
    insert.set("varrianty", 2);
    insert.set("brand", QString("volkswagen"));
    QCOMPARE(insert.toString(), string("INSERT INTO Cars (varrianty, brand) VALUES (2, \"volkswagen\")"));
}

void SQLiteTest::createSQLUpdate1()
{
    SQLUpdate update("Cars");
    update.set("color", QString("red"));
    update.set("brand", QString("opel"));

    QCOMPARE(update.toString(), string("UPDATE Cars SET color = \"red\", brand = \"opel\""));
}


void SQLiteTest::createSQLUpdate2()
{
    SQLUpdate update("Cars");
    update.set("color", QString("red"));
    update.set("brand", QString("opel"));
    update.where("color = \"blue\"");
    update.where("brand = \"volkswagen\"");

    QCOMPARE(update.toString(), string("UPDATE Cars SET color = \"red\", brand = \"opel\" WHERE color = \"blue\" AND brand = \"volkswagen\""));
}

void SQLiteTest::createSQLSelect1()
{
    SQLSelect select{"Cars"};
    select.field("color");
    select.field("brand");

    QCOMPARE(select.toString(), string("SELECT color, brand FROM Cars"));
    //QSqlQuery q(select.toString().c_str());
    //q.exec();
    //QVERIFY(q.isValid());
    //QVERIFY(q.isSelect());
}

void SQLiteTest::createSQLSelect2()
{
    SQLSelect select{"Cars"};
    select.field("color");
    select.field("brand");
    select.where("color = \"blue\"");

    QCOMPARE(select.toString(), string("SELECT color, brand FROM Cars WHERE color = \"blue\""));

}


void SQLiteTest::createSQLSelect3()
{
    SQLSelect select{"Cars"};
    select.field("color");
    select.field("brand");
    select.where("color = \"blue\"");
    select.groupBy("color");

    QCOMPARE(select.toString(), string("SELECT color, brand FROM Cars WHERE color = \"blue\" GROUP BY color"));

}

void SQLiteTest::tableStructureEquality()
{
    TableStructure ts1;
    ts1.addField("name1", "type1");
    ts1.addField("name2", "type2");
    TableStructure ts2;
    ts2.addField("name1", "type1");
    ts2.addField("name2", "type2");
    QCOMPARE(ts1, ts2);
    ts2.addField("name3", "type3");
    QVERIFY(ts1 != ts2);
}

void SQLiteTest::parseSchema()
{
    TableStructure ts1("CREATE TABLE rules (Payee INTEGER, Type INTEGER, Column INTEGER, Value TEXT, Class INTEGER)");
    QVERIFY(ts1.isValid());
    TableStructure ts2;
    ts2.addField("Payee", "INTEGER");
    ts2.addField("Type", "INTEGER");
    ts2.addField("Column", "INTEGER");
    ts2.addField("Value", "TEXT");
    ts2.addField("Class", "INTEGER");
    QCOMPARE(ts1, ts2);
}

QTEST_APPLESS_MAIN(SQLiteTest)

#include "tst_sqlite.moc"
