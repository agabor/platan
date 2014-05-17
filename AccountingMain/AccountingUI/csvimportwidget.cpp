#include "csvimportwidget.h"
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

CSVImportWidget::CSVImportWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    separator = AddComboBox(mainLayout, tr("separator"));
    separator->addItem(",", QChar(','));
    separator->addItem(";", QChar(';'));
    separator->addItem(".", QChar('.'));
    separator->addItem(tr("space"), QChar(' '));
    separator->addItem(tr("tab"), QChar('\t'));


    quote = AddComboBox(mainLayout, tr("quote"));
    quote->addItem(tr("none"), QChar('\0'));
    quote->addItem("\"", QChar('"'));
    quote->addItem("'", QChar('\''));

    mainLayout->addWidget(quote);
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

void CSVImportWidget::setReader(std::shared_ptr<CSVReader> r)
{
    reader = r;
    SelectData(quote, r->quote());
    SelectData(separator, r->separator());
    connect(quote, SIGNAL(currentIndexChanged(int)), this, SLOT(setQuote(int)));
    connect(separator, SIGNAL(currentIndexChanged(int)), this, SLOT(setSeparator(int)));
}

QComboBox * CSVImportWidget::AddComboBox(QVBoxLayout* mainLayout, QString name)
{
    QHBoxLayout* subLayout = new QHBoxLayout();
    QLabel *label = new QLabel(name, this);
    subLayout->addWidget(label);
    QComboBox* comboBox = new QComboBox(this);
    subLayout->addWidget(comboBox);

    mainLayout->addLayout(subLayout);

    return comboBox;
}
