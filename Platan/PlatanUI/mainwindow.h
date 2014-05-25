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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <set>
#include <QMap>
#include "accdatabase.h"
#include "colorpalette.h"
#include <QTabWidget>
#include "qpiechart.h"
#include "statements.h"
#include "multicolorpalette.h"
#include "qstatemenview.h"

namespace Ui{
class MainWindow;
}

class MainApplication;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(MainApplication * const application, Statements &statements, QWidget *parent = 0);

    ~MainWindow();

    void initChart(QVector<float> values, ColorPalette *palette);

    void InitLegend(ColorPalette *palette, QMap<int, QString> classNames);

    QTabWidget *tabWidget() const;


    const QPieChart *pieChart() const;

    void InitChart();
    void InitLegend();
    void setDateRange(QDate start, QDate end);

    void setClassNames(QMap<int, QString> &classNames);

public slots:
    void onDateRangeChanged(QDate start, QDate end);

private slots:

    void on_actionClose_triggered();

    void on_actionHome_triggered();

    void on_actionStatements_triggered();

    void sliceClicked(int idx);


    void onUnsetDateRange();
    void doubleClicked(QModelIndex index);

    void on_actionPythonConsole_triggered();

    void refreshStatements();

    void refreshChart();

    void on_actionImport_Bank_Statements_triggered();

private:

    Ui::MainWindow *ui;
    QVector<QDate> months;
    Statements &statements;
    QMap<int, float> classes;
    QMap<int, QString> classNames;
    MultiColorPalette palette;
    std::unique_ptr<QStatemenView> unclassified_table;
    MainApplication const * application;
    std::shared_ptr<StatementTableModel> uncategorisedTableModel;
};

#endif // MAINWINDOW_H
