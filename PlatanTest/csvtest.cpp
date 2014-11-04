#include <QFile>
#include <QTextStream>

#include <csvreader.h>
#include <csvtablemodel.h>

#include "csvtest.h"

void CSVTest::testCSV(char separator, QVector<QVector<QString>> data, char quote)
{
    const char* filename = "test.csv";
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    for(auto line : data)
    {
        QString aggregate;
        for (auto elem : line)
        {
            if (!aggregate.isEmpty())
                aggregate += separator;
            if (quote != '\0')
                aggregate += quote + elem + quote;
            else
                aggregate += elem;
        }
        aggregate += '\n';
        file.write(aggregate.toStdString().c_str());
    }
    file.close();

    QFile input(filename);
    input.open(QIODevice::ReadOnly | QIODevice::Text);
    CSVReader r;
    r.setHeadersInFirstRow(false);
    r.setSeparator(separator);
    r.setQuote(quote);
    QTextStream stream(&input);
    CSVTableModel *model = r.read(stream);
    int row = 0;
    for(auto line : data)
    {
        int column = 0;
        for (auto elem : line)
        {
            QString d = model->data(model->index(row,column), Qt::DisplayRole).toString();
            QCOMPARE(elem, d);
            ++column;
        }
        ++row;
    }
    input.close();
    QVERIFY(file.remove());
}

void CSVTest::readCSV1()
{
    QVector<QVector<QString>> data{
    {"a","b","c","d"},
    {"e","f","g","h"},
    {"i","j","k","l"},
    {"m","n","o","p"}};
    testCSV(',', data, '"');
    testCSV(';', data, '\0');
    testCSV(';', data, '\'');
}
