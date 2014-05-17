#include "csvimportwidget.h"
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>

CSVImportWidget::CSVImportWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    separator = AddLabeledWidget<QComboBox>(mainLayout, tr("separator"));
    separator->addItem(",", QChar(','));
    separator->addItem(";", QChar(';'));
    separator->addItem(".", QChar('.'));
    separator->addItem(tr("space"), QChar(' '));
    separator->addItem(tr("tab"), QChar('\t'));


    quote = AddLabeledWidget<QComboBox>(mainLayout, tr("quote"));
    quote->addItem(tr("none"), QChar('\0'));
    quote->addItem("\"", QChar('"'));
    quote->addItem("'", QChar('\''));

    headers = AddLabeledWidget<QCheckBox>(mainLayout, tr("Headers are in the first row."));
}

void CSVImportWidget::SelectData(QComboBox* comboBox, char c)
{
    for(int i= 0; i < comboBox->count(); ++i)
    {
        if (comboBox->itemData(i).toChar() == c)
            comboBox->setCurrentIndex(i);
    }
}

void CSVImportWidget::setSeparator(int index)
{
    reader->setSeparator(separator->itemData(index).toChar().toLatin1());
    emit readerParametersChanged();
}

void CSVImportWidget::setQuote(int index)
{
    reader->setQuote(quote->itemData(index).toChar().toLatin1());
    emit readerParametersChanged();
}

void CSVImportWidget::setHeaders(bool b)
{
    reader->setHeadersInFirstRow(b);
    emit readerParametersChanged();
}

void CSVImportWidget::setReader(CSVReader* r)
{
    reader = r;
    SelectData(quote, r->quote());
    SelectData(separator, r->separator());
    connect(quote, SIGNAL(currentIndexChanged(int)), this, SLOT(setQuote(int)));
    connect(separator, SIGNAL(currentIndexChanged(int)), this, SLOT(setSeparator(int)));
    connect(headers, SIGNAL(toggled(bool)), this, SLOT(setHeaders(bool)));
}



