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
#include "ui_mainwindow.h"
#include <csvreader.h>
#include <csvtablemodel.h>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QDebug>
#include <tablehelpers.h>
#include <QVector>
#include <QSet>
#include "validation.h"
#include "substitutetablemodel.h"

MainWindow::MainWindow(QString fileName) :
    QMainWindow(nullptr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->csvConfig->setReader(fileName, new CSVReader());
    ui->tableView->setVisible(false);
    ui->tableView->setModel(new SubstituteTableModel(ui->csvConfig->getTableModel()));
    ui->tableView->setWordWrap(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_stepButton_clicked()
{
    ui->csvConfig->setVisible(false);
    ui->tableView->setVisible(true);
    auto model = ui->tableView->model();
    AmountEreaser commaAmmount(',');
    AmountEreaser dotAmmount('.');
    NumberEreaser numberEreaser;
    QVector<Ereaser*> ereasers{new IBANEreaser, new BICEreaser,
                new DateEreaser("YY","MM","DD",'.'),
                new DateEreaser("YY","DD","MM",'.'),
                new DateEreaser("DD","MM","YY",'.'),
                new DateEreaser("MM","DD","YY",'.'),
                &numberEreaser,
                &commaAmmount,
                &dotAmmount,
                              };
    QSet<int> matchedColumns;
    for (int c= 0; c < model->columnCount(); ++c)
    {
        for (auto ereaser : ereasers)
        {
            bool works = true;
            for (int r= 0; r < model->rowCount(); ++r)
            {
                QString data = model->data(model->index(r,c)).toString();
                if (!data.isEmpty() && !ereaser->exactMatch(data))
                {
                    works = false;
                    break;
                }
            }
            if (works)
            {
                for (int r= 0; r < model->rowCount(); ++r)
                {
                    QString data = model->data(model->index(r,c)).toString();
                    model->setData(model->index(r,c), ereaser->getTag(data) +"!!");
                }
                matchedColumns.insert(c);
                break;
            }
        }
    }

    ereasers.removeLast();
    ereasers.removeLast();
    ereasers.removeLast();

    for (int r= 0; r < model->rowCount(); ++r)
    {
        for (int c= 0; c < model->columnCount(); ++c)
        {
            if (matchedColumns.contains(c))
                continue;
            QString data = model->data(model->index(r,c)).toString();
            for (auto ereaser : ereasers)
                ereaser->ReplaceAll(data);
            model->setData(model->index(r,c), data);
        }
    }
    model->layoutChanged();
    resizeToContents(ui->tableView);
    for (auto ereaser : ereasers)
        delete ereaser;

}
