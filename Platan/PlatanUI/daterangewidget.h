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
#include <QComboBox>
#include <QDate>
#include <vector>
#include <QCheckBox>

class QLabel;
class QTableWidget;
class QToolButton;

class DateRangeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DateRangeWidget(QWidget * parent=0);
    void setInterval(QDate start, QDate end);
    void setRange(QDate start, QDate end);
    void setYearLabelText();
    void enableDateNavigation();
signals:
    void dateRangeChanged(QDate start, QDate end);
    void unsetDateRange();
private:
    QLabel *yearLabel;
    QTableWidget *monthTable;
    int year;
    int selectedMonth() const;
    QDate startDate;
    QDate endDate;
    QToolButton *previousYear;
    QToolButton *nextYear;
private slots:
    void onDateRangeChanged();
    void onMonthChanged();
    void increaseYear();
    void decreaseYear();
};

#endif // DATERANGEWIDGET_H
