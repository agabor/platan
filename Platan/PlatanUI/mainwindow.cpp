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

    connect(&statements,SIGNAL(dataChanged()), this, SLOT(refreshStatements()));
    connect(&statements,SIGNAL(dataChanged()), this, SLOT(onModification()));
    connect(&statements,SIGNAL(modification()), this, SLOT(onModification()));

    QGroupBox *groupBox = new QGroupBox(ui->scrollArea);
    ui->scrollArea->setWidget(groupBox);
    auto layout = new QVBoxLayout(groupBox);
    for(Rule rule : rules.getRules())
    {
        layout->addWidget(new RuleWidget(rule,ui->scrollArea));
    }

    uncategorisedTableModel = nullptr;
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));
    unclassifiedTable->addAction(ui->actionAdd_rule);
    unclassifiedTable->addAction(ui->actionSet_category);
}


void MainWindow::setDateInterval()
{
    auto model = viewModel.getAllStatements();

    ui->statements_table->setModel(model.get());

    auto range = model->DateRange();
    ui->date_range->setInterval(range.first, range.second);
}

void MainWindow::init()
{
    statements.init();
    rules.init();
    viewModel.init();
    setDateInterval();
    setWindowTitle("Platan - " + statements.getOpenProjectPath());

    if (statements.isEmpty())
    {
        ui->tabWidget->setHidden(true);
        welcomeWidget = new WelcomeWidget(this);
        connect(welcomeWidget, SIGNAL(clicked()), this, SLOT(on_actionImport_Bank_Statements_triggered()));
        this->centralWidget()->layout()->addWidget(welcomeWidget);
    } else
        refreshStatements();
}

void MainWindow::setPythonIDE(std::shared_ptr<PythonIDE> pythonIDE)
{
    this->pythonIDE = pythonIDE;
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
    if (!ui->tabWidget->isOpen(class_name))
    {
        QTableView *class_table;
        if (class_idx == 0)
        {
            uncategorisedTableModel = viewModel.getUncategorisedStatementsModel();
            unclassifiedTable->setModel(uncategorisedTableModel.get());
            class_table = unclassifiedTable.get();
        } else
        {
            class_table = new QTableView();
            class_table->setModel(viewModel.getStatementsForClass(class_idx).get());
        }

        ui->tabWidget->addTableViewTab(class_table, class_name);
    }
    ui->tabWidget->openLastTab();
}

void MainWindow::refreshStatements()
{
    statements.categorizeUndefinedStatements(rules);
    viewModel.initStatementModels();
    refreshChart();
    setDateInterval();

    if (welcomeWidget && !statements.isEmpty())
    {
        welcomeWidget->setHidden(true);
        ui->tabWidget->setHidden(false);
    }
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
    viewModel.initStatementModels();
    refreshChart();
}

void MainWindow::onUnsetDateRange()
{
    statements.UnsetTimeInterval();
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
        statements.insertData(id.getImportedStatements());
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

    rules.insertRule(ard.getRule());
}

void MainWindow::onTabChanged(int idx)
{
    int uc_idx = ui->tabWidget->getIndex(Statements::categoryList().at(0));
    if (uc_idx == -1)
    {
        ui->actionAdd_rule->setEnabled(false);
        ui->actionSet_category->setEnabled(false);
        return;
    }
    ui->actionAdd_rule->setEnabled(uc_idx == idx);
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

void MainWindow::onModification()
{
    ui->actionSave->setEnabled(statements.changed());
}

void MainWindow::on_actionSave_triggered()
{
    statements.save();
}
