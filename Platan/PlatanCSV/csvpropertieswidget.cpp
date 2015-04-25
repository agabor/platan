// This file is part of Platan.
// Copyright (C) 2014 Gábor Angyal
//
// Platan is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Platan is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Platan.  If not, see <http://www.gnu.org/licenses/>.

#include "csvpropertieswidget.h"
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>

#include <csvreader.h>

template<class T>
QPair<T*, QLabel*> AddLabeledWidget(QBoxLayout* mainLayout, QString name)
{
    QHBoxLayout* subLayout = new QHBoxLayout();
    QLabel *label = new QLabel(name, mainLayout->parentWidget());
    subLayout->addWidget(label);
    T* comboBox = new T(mainLayout->parentWidget());
    subLayout->addWidget(comboBox);

    mainLayout->addLayout(subLayout);

    return 	qMakePair(comboBox, label);
}


template<class T>
T *AddNamedWidget(QBoxLayout* mainLayout, QString name)
{
    return AddLabeledWidget<T>(mainLayout, name).first;
}

CSVPropertiesWidget::CSVPropertiesWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    separator = AddNamedWidget<QComboBox>(mainLayout, tr("separator"));
    separator->addItem(",", QChar(','));
    separator->addItem(";", QChar(';'));
    separator->addItem(".", QChar('.'));
    separator->addItem(tr("space"), QChar(' '));
    separator->addItem(tr("tab"), QChar('\t'));


    quote = AddNamedWidget<QComboBox>(mainLayout, tr("quote"));
    quote->addItem(tr("none"), QChar('\0'));
    quote->addItem("\"", QChar('"'));
    quote->addItem("'", QChar('\''));

    headers = AddNamedWidget<QCheckBox>(mainLayout, tr("Headers are in the first row."));
}

void CSVPropertiesWidget::SelectData(QComboBox* comboBox, char c)
{
    for(int i= 0; i < comboBox->count(); ++i)
    {
        if (comboBox->itemData(i).toChar() == c)
            comboBox->setCurrentIndex(i);
    }
}

void CSVPropertiesWidget::setSeparator(int index)
{
    reader->setSeparator(separator->itemData(index).toChar().toLatin1());
    emit readerParametersChanged();
}

void CSVPropertiesWidget::setQuote(int index)
{
    reader->setQuote(quote->itemData(index).toChar().toLatin1());
    emit readerParametersChanged();
}

void CSVPropertiesWidget::setHeaders(bool b)
{
    reader->setHeadersInFirstRow(b);
    emit readerParametersChanged();
}

void CSVPropertiesWidget::setReader(CSVReader* r)
{
    reader = r;
    SelectData(quote, r->quote());
    SelectData(separator, r->separator());
    headers->setChecked(r->headersInFirstRow());
    connect(quote, SIGNAL(currentIndexChanged(int)), this, SLOT(setQuote(int)));
    connect(separator, SIGNAL(currentIndexChanged(int)), this, SLOT(setSeparator(int)));
    connect(headers, SIGNAL(toggled(bool)), this, SLOT(setHeaders(bool)));
}



