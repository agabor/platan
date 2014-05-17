#include "csvconfigwidget.h"
#include <QFile>
#include <QTextStream>
#include <csvreader.h>
#include <QTableView>
#include <csvpropertieswidget.h>
#include <QVBoxLayout>

CSVConfigWidget::CSVConfigWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    CSVReaderProperties = new CSVPropertiesWidget(this);
    mainLayout->addWidget(CSVReaderProperties);

    connect(CSVReaderProperties, SIGNAL(readerParametersChanged()), this, SLOT(readCSV()));

    tableView = new QTableView(this);
    mainLayout->addWidget(tableView);
}

void CSVConfigWidget::setReader(QString filename, CSVReader *r)
{
    reader = r;
    CSVReaderProperties->setReader(r);
    QFile inFile(filename);
    inFile.open(QIODevice::ReadOnly | QIODevice::Text);
    input = inFile.readAll();
    readCSV();
}

void CSVConfigWidget::readCSV()
{
    QTextStream input_stream(&input);
    tableView->setModel(reader->read(input_stream));
}
