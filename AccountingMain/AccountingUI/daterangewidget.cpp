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
#include <QVBoxLayout>
#include <QHBoxLayout>

std::vector<QDate> MonthesBetween(QDate min_date, QDate max_date);
QString MonthToString(QDate month);

DateRangeWidget::DateRangeWidget(QWidget * parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *h_layout = new QHBoxLayout();

    addDateChooser(start_date, h_layout);
    addDateChooser(end_date, h_layout);

    QPushButton *refresh = new QPushButton("Refresh", this);
    registerDateSetWidget(refresh);
    connect(refresh, SIGNAL(clicked()), this, SLOT(onDateRangeChanged()));
    h_layout->addWidget(refresh);

    layout->addLayout(h_layout);

    QHBoxLayout *h_layout2 = new QHBoxLayout();

    monthes_cbx = new QComboBox(this);
    registerDateSetWidget(monthes_cbx);
    h_layout2->addWidget(monthes_cbx);
    connect(monthes_cbx, SIGNAL(currentIndexChanged(int)), this, SLOT(monthChanged(int)));

    all_range = new QCheckBox(this);
    all_range->setText("all");
    h_layout2->addWidget(all_range);
    connect(all_range, SIGNAL(stateChanged(int)), this, SLOT(onAllCheckChanged(int)));

    layout->addLayout(h_layout2);
}


void DateRangeWidget::setInterval(QDate start, QDate end)
{
    start_date->setDate(start);
    end_date->setDate(end);
    monthes = MonthesBetween(start, end);
    for(const QDate& month : monthes)
    {
        monthes_cbx->addItem(MonthToString(month), month);
    }
}

void DateRangeWidget::setRange(QDate start, QDate end)
{
    start_date->setDate(start);
    end_date->setDate(end);
    onDateRangeChanged();
}

void DateRangeWidget::onDateRangeChanged()
{
    emit dateRangeChanged(start_date->date(), end_date->date());
}


inline QDate monthStart(QDate month)
{
    return QDate(month.year(), month.month(), 1);
}

inline QDate monthEnd(QDate month)
{
    return QDate(month.year(), month.month(), month.daysInMonth());
}

void DateRangeWidget::monthChanged(int index)
{
    QDate month = monthes_cbx->itemData(index).toDate();
    start_date->setDate(monthStart(month));
    end_date->setDate(monthEnd(month));
    onDateRangeChanged();
}

void DateRangeWidget::onAllCheckChanged(int state)
{
    Qt::CheckState check_state = (Qt::CheckState)state;
    if (check_state == Qt::Checked)
    {
        enableDateSet(false);
        emit unsetDateRange();
        return;
    }
    enableDateSet(true);
    onDateRangeChanged();
}


std::vector<QDate> MonthesBetween(QDate min_date, QDate max_date)
{
    std::vector<QDate> result;

    if (!min_date.isValid() || max_date.isValid())
        return result;

    QDate i(min_date.year(), min_date.month(), 1);

    while (i <= max_date)
    {
        result.push_back(i);
        i = i.addMonths(1);
    }

    return result;
}

QString MonthToString(QDate month)
{
    QString month_name(QDate::longMonthName(month.month()));
    return QString("%1 %2").arg(month.year()).arg(month_name);
}

void DateRangeWidget::addDateChooser(QDateChooser *&date_chooser, QLayout *layout)
{
    date_chooser = new QDateChooser(this);
    registerDateSetWidget(date_chooser);
    QToolButton *choose_date_btn = new QToolButton(this);
    choose_date_btn->setText("...");
    connect(choose_date_btn, SIGNAL(clicked()), date_chooser, SLOT(chooseDate()));
    layout->addWidget(date_chooser);
    layout->addWidget(choose_date_btn);
}

void DateRangeWidget::enableDateSet(bool value)
{
    for(auto widget : date_set_widgets)
        widget->setEnabled(value);
}

void DateRangeWidget::registerDateSetWidget(QWidget *widget)
{
    date_set_widgets.push_back(widget);
}
