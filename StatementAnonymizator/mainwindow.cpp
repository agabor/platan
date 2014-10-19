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

#include <memory>

#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QDebug>
#include <QVector>
#include <QSet>
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QFileDialog>

#include <csvtablemodel.h>
#include <tablehelpers.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "validation.h"
#include "substitutetablemodel.h"

using namespace std;

MainWindow::MainWindow() :
    QMainWindow(nullptr),
    ui(new Ui::MainWindow),
    reader{new CSVReader}
{
    reader->setMaxium(100);
    ui->setupUi(this);
    ui->tableView->setVisible(false);
    ui->tableView->setWordWrap(true);
    ui->lbLines->setVisible(false);
    ui->sbLines->setVisible(false);
    ui->btDelete->setVisible(false);
    ui->progressBar->setVisible(false);
    step = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
        on_btDelete_clicked();
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, QObject::tr("Select file for anonyimization"),
                                                    "",
                                                    QObject::tr("CSV (*.csv *.txt)"));
    if (fileName.isEmpty())
    {
        close();
        return;
    }

    ui->csvConfig->setReader(fileName, reader.get());
}

bool MainWindow::checkColumn(int c, Ereaser *ereaser)
{
    for (int r= 0; r < model->rowCount(); ++r)
    {
        QString data = model->data(model->index(r,c)).toString();
        if (!data.isEmpty() && !ereaser->exactMatch(data))
        {
            return false;
        }
    }
    return true;
}

bool MainWindow::ereaseColumn(int c, Ereaser *ereaser)
{
    for (int r= 0; r < model->rowCount(); ++r)
    {
        QString data = model->data(model->index(r,c)).toString();
        if (!data.isEmpty())
            model->setData(model->index(r,c), ereaser->getTag(data));
    }
}

bool MainWindow::tryToMatchColumn(int c, QVector<shared_ptr<Ereaser>> ereasers)
{
    for (auto ereaser : ereasers)
    {
        if (checkColumn(c, ereaser.get()))
        {
            ereaseColumn(c, ereaser.get());
            return true;
        }
    }
    return false;
}

QVector<shared_ptr<Ereaser>> getEreasers()
{
    QVector<shared_ptr<Ereaser>> ereasers {
        make_shared<IBANEreaser>(),
        make_shared<BICEreaser>()};
    for(QString year : {"YYYY", "YY"})
    {
        for (QChar sep : {'.', '-'})
        {
            ereasers.push_back(make_shared<DateEreaser>(year, "MM", "DD", sep));
            ereasers.push_back(make_shared<DateEreaser>(year, "DD", "MM", sep));
            ereasers.push_back(make_shared<DateEreaser>("MM", "DD", year, sep));
            ereasers.push_back(make_shared<DateEreaser>("DD", "MM", year, sep));
        }
    }
    ereasers.push_back(make_shared<NumberEreaser>());
    ereasers.push_back(make_shared<AmountEreaser>(','));
    ereasers.push_back(make_shared<AmountEreaser>('.'));

    return ereasers;
}

void MainWindow::applyEreasers()
{
    ui->progressBar->setVisible(true);
    auto ereasers = getEreasers();

    QSet<int> matchedColumns;
    int columnCount = model->columnCount();
    int rowCount = model->rowCount();
    for (int c = 0; c < columnCount; ++c)
    {
        if (tryToMatchColumn(c, ereasers))
            matchedColumns.insert(c);
        int p = ((c+1.0f)/(columnCount * 2.0f)) * 100;
        qDebug() << p;
        ui->progressBar->setValue(p);
        QCoreApplication::processEvents();
    }

    for (int i = 0; i < 2; ++i)
        ereasers.removeLast();

    for (int c= 0; c < columnCount; ++c)
    {
        if (!matchedColumns.contains(c))
        {
            for (int r= 0; r < rowCount; ++r)
            {

                for (auto ereaser : ereasers)
                    model->ReplaceAll(*ereaser, r, c);
            }
        }
        int p = 50 + (((c+1.0f))/(columnCount)) * 50;
        qDebug() << p;
        ui->progressBar->setValue(p);
        QCoreApplication::processEvents();
    }
    ui->progressBar->setVisible(false);
    model->layoutChanged();
    resizeToContents(ui->tableView);
}

void MainWindow::showAnonymizedTable()
{
    model = new SubstituteTableModel(ui->csvConfig->getTableModel());
    model->setHeaders(ui->csvConfig->getTableModel()->getHeaders());
    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    applyEreasers();
    ui->lbLines->setVisible(true);
    ui->sbLines->setVisible(true);
    ui->btDelete->setVisible(true);
    ui->csvConfig->setVisible(false);
    ui->tableView->setVisible(true);
}

void MainWindow::printHeaders(QTextStream &out)
{
    QStringList headers = model->getHeaders();
    char q = reader->quote();
    if (!headers.isEmpty())
    {
        for(int i=0; i<headers.size()-1;++i)
        {
            if (q != 0)
                out << q;
            out << headers[i];
            if (q != 0)
                out << q;
            out << reader->separator();
        }
        if (q != 0)
            out << q;
        out << headers[headers.size()-1];
        if (q != 0)
            out << q;
        out << endl;
    }
}

void MainWindow::printData(QTextStream &out)
{
    char q = reader->quote();
    for (int r = 0; r < model->getStartSection(); ++r)
    {
        for (int c = 0; c < model->columnCount()-1; ++c)
        {
            if (q != 0)
                out << q;
            QString data = model->data(model->index(r,c)).toString();
            out << data;
            if (q != 0)
                out << q;
            out << reader->separator();
        }
        if (q != 0)
            out << q;
        QString data = model->data(model->index(r,model->columnCount()-1)).toString();
        out << data;
        if (q != 0)
            out << q;
        out << endl;
    }
}

bool MainWindow::saveOutput()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Set output file"),"",
                                                    QObject::tr("CSV (*.csv *.txt)"));
    if (fileName.isEmpty())
        return false;

    if (!fileName.toLower().endsWith(".csv") && !fileName.toLower().endsWith(".txt"))
        fileName += ".csv";

    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
        return false;

    QTextStream out(&file);
    printHeaders(out);
    printData(out);
    file.close();

    return true;
}

void MainWindow::on_stepButton_clicked()
{
    switch (step) {
    case 0:
        showAnonymizedTable();
        break;
    case 1:
        if (saveOutput())
            close();
        else
            return;

        break;
    }
    ++step;

}

void MainWindow::on_sbLines_valueChanged(int arg1)
{
    model->setStartSection(ui->sbLines->value());
}

void MainWindow::on_sbLines_editingFinished()
{

}

void MainWindow::on_btDelete_clicked()
{
    QModelIndex idx = ui->tableView->selectionModel()->currentIndex();
    if (idx.isValid())
        model->deleteRow(idx.row());
}
