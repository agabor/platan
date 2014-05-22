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

    SetStatements();

    connect(ui->date_range, SIGNAL(dateRangeChanged(QDate,QDate)), this, SLOT(onDateRangeChanged(QDate,QDate)));
    connect(ui->date_range, SIGNAL(unsetDateRange()), this, SLOT(onUnsetDateRange()));

    connect(ui->chart, SIGNAL(sliceClicked(int)), this, SLOT(sliceClicked(int)));

    for (auto action : ui->toolBar->actions())
    {
        action->setStatusTip(action->toolTip());
    }

    connect(&statements,SIGNAL(dataChanged()), this, SLOT(refreshStatements()));
}

void MainWindow::SetStatements()
{
    auto model = new StatementTableModel(statements.GetStatements());

    ui->statements_table->setModel(model);

    auto range = model->DateRange();
    ui->date_range->setInterval(range.first, range.second);
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
    class_names.insert(0, tr("Undefined"));
    class_names.insert(1, tr("Food"));
    class_names.insert(2, tr("Clothes"));
    class_names.insert(3, tr("Housing"));
    class_names.insert(4, tr("Public transportation"));
    class_names.insert(5, tr("House costs"));
    class_names.insert(6, tr("Electronics"));
    class_names.insert(7, tr("Cash"));
    class_names.insert(8, tr("Furniture"));
    class_names.insert(9, tr("Restaurant"));
    class_names.insert(10, tr("Sport"));
    class_names.insert(11, tr("Insurance"));
    class_names.insert(12, tr("Bank"));
    class_names.insert(13, tr("Drogstore"));
    class_names.insert(14, tr("Mobil"));
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
            class_table = unclassified_table.get();
            class_table->setModel(new StatementTableModel(statements.GetStatements(), class_table));
            connect(unclassified_table.get(), SIGNAL(SetClass(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));
        } else
        {
            class_table = new QTableView();
            class_table->setModel(new StatementExtractTableModel(statements.GetStatementsForClass(class_idx), class_table));
        }

        ui->tabWidget->addTableViewTab(class_table, class_name);
    }
    ui->tabWidget->openLastTab();
}

void MainWindow::refreshStatements()
{
    SetStatements();
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
    QMap<int, QString> class_names;
    setClassNames(class_names);
    AddRuleDialog ard(class_names);
    ard.exec();

    if (!ard.ok())
        return;

    const int type = index.column() == 4 ? 0 : 1;
    //db->InsertRule(type, index.column(), unclassified_statements->data(index,Qt::DisplayRole).toString(), ard.Class(), ard.name());
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
