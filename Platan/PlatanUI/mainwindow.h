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

#include <memory>

#include "colorpalette.h"

namespace Ui{
class MainWindow;
}

class MainApplication;
class WelcomeWidget;
class Statements;
class Rules;
class QStatemenView;
class QPieChart;
class StatementTableModel;
class ViewModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Statements &statements, Rules &rules, ViewModel &viewModel, QWidget *parent = 0);

    ~MainWindow();

    void initChart(QVector<QPair<QColor, float> > values);

    void InitLegend(ColorPalette *palette, QMap<int, QString> classNames);

    QTabWidget *tabWidget() const;


    const QPieChart *pieChart() const;

    void InitChart();
    void InitLegend();
    void setDateRange(QDate start, QDate end);

    void setClassNames(QMap<int, QString> &classNames);

    void setDateInterval();

    void init();

    void setSaveButtonEnabled();

    bool hasChanges();
protected:
     void closeEvent(QCloseEvent *event) override;

public slots:
    void onDateRangeChanged(QDate start, QDate end);

private slots:

    void onDataChanged();

    void on_actionClose_triggered();

    void on_actionHome_triggered();

    void on_actionStatements_triggered();

    void sliceClicked(int idx);


    void onUnsetDateRange();


    void refreshStatements();

    void refreshChart();

    void on_actionImport_Bank_Statements_triggered();

    void on_actionAdd_rule_triggered();

    void onTabChanged(int idx);

    void on_actionSet_category_triggered();

    void on_actionSave_triggered();

    void on_actionDeleteRule_triggered();

    void statementsTableIndexChanged(const QModelIndex &current, const QModelIndex &);
    void on_actionExport_Commands_triggered();

    void on_actionAbout_triggered();

    void on_actionPlugins_triggered();

private:

    Ui::MainWindow *ui;
    QVector<QDate> months;
    Statements &statements;
    Rules &rules;
    ViewModel &viewModel;
    QMap<int, float> classes;
    QMap<int, QString> classNames;
    ColorPalette palette;
    std::unique_ptr<QStatemenView> unclassifiedTable;
    std::shared_ptr<StatementTableModel> uncategorisedTableModel;
    WelcomeWidget *welcomeWidget;
};

#endif // MAINWINDOW_H
