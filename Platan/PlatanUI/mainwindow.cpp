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

#include <QFileDialog>
#include <QTableView>
#include <QDebug>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDate>
#include <QMessageBox>
#include <QCloseEvent>

#include <mainwindow.h>
#include <importdialog.h>
#include <qpiechart.h>
#include <addruledialog.h>
#include <ui_mainwindow.h>
#include <pythonide.h>
#include <rulewidget.h>
#include <setcategorydialog.h>
#include <welcomewidget.h>
#include <qstatemenview.h>
#include <statementtablemodel.h>
#include <ruletablemodel.h>
#include <rules.h>
#include <viewmodel.h>
#include <PythonAPI/pythonapi.h>
#include <statements.h>

MainWindow::MainWindow(Statements &statements, Rules &rules, ViewModel &viewModel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    statements(statements),
    rules(rules),
    viewModel(viewModel),
    unclassifiedTable(new QStatemenView(this)),
    welcomeWidget(nullptr)
{
    ui->setupUi(this);
    ui->tabWidget->removeCloseButtons();

    classes = statements.getCategories();

    connect(ui->date_range, SIGNAL(dateRangeChanged(QDate,QDate)), this, SLOT(onDateRangeChanged(QDate,QDate)));
    connect(ui->date_range, SIGNAL(unsetDateRange()), this, SLOT(onUnsetDateRange()));

    connect(ui->chart, SIGNAL(sliceClicked(int)), this, SLOT(sliceClicked(int)));

    for (auto action : ui->toolBar->actions())
    {
        action->setStatusTip(action->toolTip());
    }

    uncategorisedTableModel = nullptr;
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));
    unclassifiedTable->addAction(ui->actionAdd_rule);
    unclassifiedTable->addAction(ui->actionSet_category);
    ui->statements_table->addAction(ui->actionAdd_rule);
    ui->statements_table->addAction(ui->actionSet_category);
    ui->actionPythonConsole->setEnabled(false);

    connect(&rules, SIGNAL(dataChanged()), this, SLOT(onDataChanged()));
    connect(&statements, SIGNAL(dataChanged()), this, SLOT(onDataChanged()));
}


void MainWindow::setDateInterval()
{
    auto range = viewModel.getAllStatements()->DateRange();
    ui->date_range->setInterval(range.first, range.second);
}

void MainWindow::init()
{
    statements.init();
    rules.init();
    viewModel.init();
    setDateInterval();

    ui->rulesView->setModel(viewModel.getRuleTable().get());
    ui->rulesView->addAction(ui->actionChangeRule);
    ui->rulesView->addAction(ui->actionDeleteRule);

    ui->statements_table->setModel(viewModel.getAllStatements().get());

    if (statements.isEmpty())
    {
        ui->tabWidget->setHidden(true);
        welcomeWidget = new WelcomeWidget(this);
        connect(welcomeWidget, SIGNAL(clicked()), this, SLOT(on_actionImport_Bank_Statements_triggered()));
        this->centralWidget()->layout()->addWidget(welcomeWidget);
    } else
    {
        statements.categorizeUndefinedStatements(rules);
        refreshStatements();
    }
}

void MainWindow::setPythonIDE(std::shared_ptr<PythonIDE> pythonIDE)
{
    this->pythonIDE = pythonIDE;
}

void MainWindow::initChart(QVector<QPair<QColor, float>> values)
{
    ui->chart->init(values);
}

void MainWindow::InitLegend(ColorPalette *palette, QMap<int, QString> class_names)
{
    ui->legend->clear();
    for (auto i : class_names.keys())
    {
        ui->legend->addItem(palette->getColor(i), class_names[i]);
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
    QVector<QPair<QColor, float>> values;
    for (int i : classes.keys())
    {
        float sum = -1 * classes[i];
        if (sum < 0)
            sum *= -1;
        values.append(QPair<QColor, float>(palette.getColor(i), sum));
    }

    initChart(values);
}

void MainWindow::setClassNames(QMap<int, QString> &class_names)
{
    int i = 0;
    for(QString category : Statements::categoryList())
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
    if (ui->tabWidget->isOpen(class_name))
    {
        ui->tabWidget->activateTab(class_name);
        return;
    }
    QTableView *class_table;
    if (class_idx == 0)
    {
        uncategorisedTableModel = viewModel.getUncategorisedStatementsModel();
        unclassifiedTable->setModel(uncategorisedTableModel.get());
        class_table = unclassifiedTable.get();
    } else
    {
        class_table = new QStatemenView();
        class_table->setModel(viewModel.getStatementsForClass(class_idx).get());
        class_table->addAction(ui->actionSet_category);
    }

    ui->tabWidget->addTableViewTab(class_table, class_name);
    ui->tabWidget->activateLastTab();
}

bool MainWindow::hasChanges()
{
    return statements.changed() || rules.changed();
}

void MainWindow::setSaveButtonEnabled()
{
    ui->actionSave->setEnabled(hasChanges());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (hasChanges())
    {
        auto reply = QMessageBox::question(this, tr("Unsaved changes"),
                                           tr("You have unsaved changes! Would you like to save them before closing?"),
                                      QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        switch(reply)
        {
        case QMessageBox::Yes:
            on_actionSave_triggered();
            break;
        case QMessageBox::No:
            break;
        case QMessageBox::Cancel:
            event->ignore();
            break;
        }
    }
}

void MainWindow::refreshStatements()
{
    viewModel.initTableModels();
    ui->rulesView->resizeColumnsToContents();
    refreshChart();
    setDateInterval();

    if (welcomeWidget && !statements.isEmpty())
    {
        welcomeWidget->setHidden(true);
        ui->tabWidget->setHidden(false);
    }

    setSaveButtonEnabled();
}

void MainWindow::refreshChart()
{
    classes = statements.getCategories();
    InitChart();
    InitLegend();
}

void MainWindow::onDateRangeChanged(QDate start, QDate end)
{
    statements.SetTimeInterval(start, end);
    viewModel.initTableModels();
    refreshChart();
}

void MainWindow::onDataChanged()
{
    setSaveButtonEnabled();
    refreshChart();
}

void MainWindow::onUnsetDateRange()
{
    statements.UnsetTimeInterval();
    viewModel.initTableModels();
    refreshChart();
}


void MainWindow::on_actionPythonConsole_triggered()
{
    pythonIDE->show();
}


void MainWindow::on_actionImport_Bank_Statements_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "",
                                                    tr("Files (*.csv)"));

    if (fileName.isEmpty())
        return;
    ImportDialog id(this, fileName);
    if (id.exec() == QDialog::Accepted)
    {
        statements.insertData(id.getImportedStatements());
        refreshStatements();
    }
}

void MainWindow::on_actionAdd_rule_triggered()
{
    QModelIndex index = unclassifiedTable->currentIndex();
    if (!index.isValid())
        return;
    AddRuleDialog ard;
    ard.setRow(uncategorisedTableModel->row(index.row()));
    ard.setColumn(index.column());

    if (QDialog::Accepted != ard.exec())
        return;

    Rule rule = ard.getRule();
    rules.insertRule(rule);
    statements.categorizeUndefinedStatements(rule);
}

void MainWindow::onTabChanged(int idx)
{
    ui->actionChangeRule->setEnabled(false);
    ui->actionDeleteRule->setEnabled(1 == idx);

    int uc_idx = ui->tabWidget->getIndex(Statements::categoryList().at(0));
    if (uc_idx == -1)
    {
        ui->actionAdd_rule->setEnabled(false);
        ui->actionSet_category->setEnabled(false);
        return;
    }
    if (uc_idx == idx)
    ui->actionAdd_rule->setEnabled(true);
    ui->actionSet_category->setEnabled(uc_idx == idx);
}

void MainWindow::on_actionSet_category_triggered()
{
    QModelIndex index = unclassifiedTable->currentIndex();
    if (!index.isValid())
        return;
    SetCategoryDialog d;

    if (QDialog::Accepted != d.exec())
        return;

    statements.setCategory(uncategorisedTableModel->row(index.row()).id, d.category());
}

void MainWindow::on_actionSave_triggered()
{
    rules.save();
    statements.save();
    setSaveButtonEnabled();
}

void MainWindow::on_actionDeleteRule_triggered()
{
    QModelIndex index = ui->rulesView->currentIndex();
    if (!index.isValid())
        return;
    int r = index.row();
    statements.rollBack(*rules.at(r));
    rules.removeRuleAt(r);
}

void MainWindow::statementsTableIndexChanged(const QModelIndex &current, const QModelIndex &previous)
{
    ui->actionAdd_rule->setEnabled(statements[current.row()]->category == 0);
}
