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

#ifndef DATERANGEWIDGET_H
#define DATERANGEWIDGET_H

#include <QWidget>
#include "qdatechooser.h"
#include <QComboBox>
#include <QDate>
#include <vector>
#include <QCheckBox>

class DateRangeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DateRangeWidget(QWidget * parent=0);
    void setInterval(QDate start, QDate end);
    void setRange(QDate start, QDate end);
signals:
    void dateRangeChanged(QDate start, QDate end);
    void unsetDateRange();
private:
    void addDateChooser(QDateChooser *&date_chooser, QLayout *layout);
    void enableDateSet(bool value);
    void registerDateSetWidget(QWidget *widget);
    QDateChooser *start_date;
    QDateChooser *end_date;
    QComboBox *monthes_cbx;
    std::vector<QDate> monthes;
    QCheckBox *all_range;
    std::vector<QWidget*> date_set_widgets;
private slots:
    void onDateRangeChanged();
    void monthChanged(int index);
    void onAllCheckChanged(int state);

};

#endif // DATERANGEWIDGET_H
