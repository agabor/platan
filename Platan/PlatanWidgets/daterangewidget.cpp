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

#include "daterangewidget.h"

#include <QPushButton>
#include <QToolButton>
#include <QLayout>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>
#include <QCheckBox>

static QSize myGetQTableWidgetSize(QTableWidget *t) {
   int w = 0;
   for (int i = 0; i < t->columnCount(); i++)
      w += t->columnWidth(i) + 3;
   int h = 0;
   for (int i = 0; i < t->rowCount(); i++)
      h += t->rowHeight(i) + 3;
   return QSize(w, h);
}

DateRangeWidget::DateRangeWidget(QWidget * parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *h_layout = new QHBoxLayout();


    layout->addLayout(h_layout);

    previousYear = new QToolButton(this);
    previousYear->setIcon(QIcon(":/icons/icons/go-previous.png"));
    h_layout->addWidget(previousYear);
    connect(previousYear, SIGNAL(clicked()), this, SLOT(decreaseYear()));

    yearLabel = new QLabel(this);
    yearLabel->setAlignment(Qt::AlignCenter);
    h_layout->addWidget(yearLabel);

    nextYear = new QToolButton(this);
    nextYear->setIcon(QIcon(":/icons/icons/go-next.png"));
    h_layout->addWidget(nextYear);
    connect(nextYear, SIGNAL(clicked()), this, SLOT(increaseYear()));

    monthTable = new QTableWidget(this);
    monthTable->setRowCount(3);
    monthTable->setColumnCount(4);
    monthTable->verticalHeader()->hide();
    monthTable->horizontalHeader()->hide();
    for (int c = 0 ; c < 4; ++c)
      for (int r = 0 ; r < 3; ++r)
      {
          QTableWidgetItem *item = new QTableWidgetItem(QDate::longMonthName(r * 4 + c + 1));
          item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
          monthTable->setItem(r,c,item);
      }
    monthTable->setShowGrid(false);
    monthTable->setMaximumSize(myGetQTableWidgetSize(monthTable));
    monthTable->setMinimumSize(monthTable->maximumSize());
    monthTable->setSelectionMode(QAbstractItemView::SingleSelection);
    layout->addWidget(monthTable);

    connect(monthTable, SIGNAL(currentCellChanged(int,int,int,int)), this, SLOT(onMonthChanged()));


    setFixedWidth(monthTable->maximumWidth()+10);

    showAllCHB = new QCheckBox(tr("Show all"), this);
    layout->addWidget(showAllCHB);
    connect(showAllCHB, SIGNAL(toggled(bool)), this, SLOT(showAll(bool)));
    showAllCHB->setChecked(true);
}


void DateRangeWidget::setYearLabelText()
{
    yearLabel->setText(QString::number(year));
}

void DateRangeWidget::setInterval(QDate start, QDate end)
{
    year = start.year();
    setYearLabelText();
    startDate = start;
    endDate = end;
    enableDateNavigation();
}

int DateRangeWidget::selectedMonth() const
{
    int r = monthTable->currentRow();
    int c = monthTable->currentColumn();
    return r * 4 + c + 1;
}

int DateRangeWidget::firstEnabledMonth()
{
    for (int r = 0 ; r < 3; ++r)
    {
        for (int c = 0 ; c < 4; ++c)
        {
            QTableWidgetItem *item = monthTable->item(r, c);
            if (item->flags() & Qt::ItemIsEnabled)
            {
                int month = r * 4 + c + 1;
                return month;
            }
        }
    }
    return 13;
}

int DateRangeWidget::lastEnabledMonth()
{
    for (int r = 2 ; r >= 0; --r)
    {
        for (int c = 3 ; c >= 0; --c)
        {
            QTableWidgetItem *item = monthTable->item(r, c);
            if (item->flags() & Qt::ItemIsEnabled)
            {
                int month = r * 4 + c + 1;
                return month;
            }
        }
    }
    return 0;
}

void DateRangeWidget::onDateRangeChanged()
{
    emit dateRangeChanged(QDate(), QDate());
}

void DateRangeWidget::onMonthChanged()
{
    QDate monthStart(year, selectedMonth(), 1);
    emit dateRangeChanged(monthStart, QDate(year, monthStart.month(), monthStart.daysInMonth()));
}

void DateRangeWidget::enableDateNavigation()
{
    previousYear->setEnabled(!showAllCHB->isChecked() && year > startDate.year());
    nextYear->setEnabled(!showAllCHB->isChecked() && year < endDate.year());
    for (int r = 0 ; r < 3; ++r)
        for (int c = 0 ; c < 4; ++c)
        {
            int month = r * 4 + c + 1;
            QTableWidgetItem *item = monthTable->item(r, c);
            QDate monthStart(year, month, 1);
            QDate monthEnd(year, month, monthStart.daysInMonth());
            if (monthEnd >= startDate && monthStart <= endDate)
                item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
            else
                item->setFlags(Qt::NoItemFlags);
        }
}

void DateRangeWidget::clampMonth(int month)
{
    int m = firstEnabledMonth();
    if (month < m)
        monthTable->setCurrentCell((m-1) / 4, (m-1) % 4);
    m = lastEnabledMonth();
    if (month > m)
        monthTable->setCurrentCell((m-1) / 4, (m-1) % 4);
}

void DateRangeWidget::increaseYear()
{
    int month = selectedMonth();
    ++year;
    setYearLabelText();
    enableDateNavigation();
    clampMonth(month);
    onMonthChanged();
}

void DateRangeWidget::decreaseYear()
{
    int month = selectedMonth();
    --year;
    setYearLabelText();
    enableDateNavigation();
    clampMonth(month);
    onMonthChanged();
}

void DateRangeWidget::showAll(bool value)
{
    previousYear->setEnabled(!value);
    nextYear->setEnabled(!value);
    monthTable->setEnabled(!value);
    if (value)
        emit unsetDateRange();
    else
    {
        clampMonth(selectedMonth());
        onMonthChanged();
    }
}


inline QDate monthStart(QDate month)
{
    return QDate(month.year(), month.month(), 1);
}

inline QDate monthEnd(QDate month)
{
    return QDate(month.year(), month.month(), month.daysInMonth());
}

