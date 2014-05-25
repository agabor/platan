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

#include "mainwindow.h"
#include <QFileDialog>
#include <csvreader.h>
#include "importdialog.h"
#include <sstream>
#include "qpiechart.h"
#include "analogouscolorpalette.h"
#include "multicolorpalette.h"
#include <QDebug>
#include "qlegend.h"
#include <QLayout>
#include <QTableView>
#include "addruledialog.h"
#include "PythonAPI/pythonapi.h"
#include "ui_mainwindow.h"
#include "mainapplication.h"
#include "pythonide.h"
#include <rulewidget.h>
#include <QVBoxLayout>
#include <QGroupBox>

MainWindow::MainWindow(MainApplication * const application, Statements &statements, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    statements(statements),
    unclassified_table(new QStatemenView(this)),
    application(application)
{
    ui->setupUi(this);
    ui->tabWidget->removeCloseButtons();

    this->statements.GetCalssification(classes);

    palette.push_back(AnalogousColorPalette(0xE67F90, 55));
    palette.push_back(AnalogousColorPalette(0x89E67E, 55));
    palette.push_back(AnalogousColorPalette(0x7FBCE6, 55));
    palette.init(classes.size());

    auto model = statements.getAllStatements();

    ui->statements_table->setModel(model.get());

    auto range = model->DateRange();
    ui->date_range->setInterval(range.first, range.second);

    connect(ui->date_range, SIGNAL(dateRangeChanged(QDate,QDate)), this, SLOT(onDateRangeChanged(QDate,QDate)));
    connect(ui->date_range, SIGNAL(unsetDateRange()), this, SLOT(onUnsetDateRange()));

    connect(ui->chart, SIGNAL(sliceClicked(int)), this, SLOT(sliceClicked(int)));

    for (auto action : ui->toolBar->actions())
    {
        action->setStatusTip(action->toolTip());
    }

    connect(&statements,SIGNAL(dataChanged()), this, SLOT(refreshStatements()));

    QGroupBox *groupBox = new QGroupBox(ui->scrollArea);
    ui->scrollArea->setWidget(groupBox);
    auto layout = new QVBoxLayout(groupBox);
    for(Rule rule : statements.getRules())
    {
        layout->addWidget(new RuleWidget(rule,ui->scrollArea));
    }

    uncategorisedTableModel = nullptr;
}


void MainWindow::initChart(QVector<float> values, ColorPalette *palette)
{
    ui->chart->init(values, palette);
}

void MainWindow::InitLegend(ColorPalette *palette, QMap<int, QString> class_names)
{
    ui->legend->clear();
    for (int i = 0; i < class_names.size(); ++ i)
    {
        ui->legend->addItem(palette->getColor(i), class_names.values().at(i));
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionClose_triggered()
{
    close();
}

void MainWindow::on_actionHome_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_actionStatements_triggered()
{
    ui->tabWidget->setCurrentIndex(1);
}


QTabWidget *MainWindow::tabWidget() const
{
    return ui->tabWidget;
}


const QPieChart *MainWindow::pieChart() const
{
    return ui->chart;
}

void MainWindow::InitChart()
{
    const int size = classes.size();
    QVector<float> values;
    for (int i = 0; i < size; ++ i)
    {
        float sum = -1 * classes.values().at(i);
        if (sum < 0)
            sum *= -1;
        values.push_back(sum);
    }

    initChart(values, &palette);
}

void MainWindow::setClassNames(QMap<int, QString> &class_names)
{
    int i = 0;
    for(QString category : Statements::categoryList)
        class_names.insert(i++, category);
}

void MainWindow::InitLegend()
{
    setClassNames(classNames);

    //remove empty classes
    for(int i = 0; i < classNames.size(); ++i)
    {
        const int key = classNames.keys().at(i);
        if (!classes.contains(key))
        {
            classNames.remove(key);
            --i;
        }
    }

    InitLegend(&palette, classNames);
}

void MainWindow::setDateRange(QDate start, QDate end)
{
    ui->date_range->setRange(start, end);
}

void MainWindow::sliceClicked(int idx)
{
    const int class_idx = classes.keys().at(idx);
    const QString &class_name = classNames[class_idx];
    if (!ui->tabWidget->isOpen(class_name))
    {
        QTableView *class_table;
        if (class_idx == 0)
        {
            uncategorisedTableModel = statements.getUncategorisedStatements();
            unclassified_table->setModel(uncategorisedTableModel.get());
            connect(unclassified_table.get(), SIGNAL(SetClass(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));
            class_table = unclassified_table.get();
        } else
        {
            class_table = new QTableView();
            class_table->setModel(statements.getStatementsForClass(class_idx).get());
        }

        ui->tabWidget->addTableViewTab(class_table, class_name);
    }
    ui->tabWidget->openLastTab();
}

void MainWindow::refreshStatements()
{
    refreshChart();
}

void MainWindow::refreshChart()
{
    statements.GetCalssification(classes);
    palette.init(classes.size());
    InitChart();
    InitLegend();
}

void MainWindow::onDateRangeChanged(QDate start, QDate end)
{
    statements.SetTimeInterval(start, end);
    refreshChart();
}

void MainWindow::onUnsetDateRange()
{
    statements.UnsetTimeInterval();
    refreshChart();
}



void MainWindow::doubleClicked( QModelIndex  index)
{
    if (index.column() != 4 && index.column() != 1)
        return;

    AddRuleDialog ard;
    ard.setRow(uncategorisedTableModel->row(index.row()));

    if (QDialog::Accepted != ard.exec())
        return;

    statements.InsertRule(ard.getRule());
}

void MainWindow::on_actionPythonConsole_triggered()
{
    application->getPythonConsole()->show();
}


void MainWindow::on_actionImport_Bank_Statements_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Files (*.csv)"));

    ImportDialog id(this, fileName);
    if (id.exec() == QDialog::Accepted)
        statements.InsertData(*id.getModel().get());
}
