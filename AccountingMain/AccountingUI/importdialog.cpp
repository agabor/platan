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

#include "importdialog.h"
#include "ui_importdialog.h"
#include <simpletransformations.h>
#include <datetransformation.h>
#include <accdatabase.h>

ImportDialog::ImportDialog(QWidget *parent, QString filename) :
    QDialog(parent),
    ui(new Ui::ImportDialog)
{
    ui->setupUi(this);
    ui->csvConfigWidget->setReader(filename, &reader);
    ui->csvImportWidget->setVisible(false);
    ui->finish->setVisible(false);

}

ImportDialog::~ImportDialog()
{
    delete ui;
}

StatementTableModel *ImportDialog::getModel() const
{
    return ui->csvImportWidget->getModel();
}


void ImportDialog::on_cancel_clicked()
{
    close();
}

void ImportDialog::on_next_clicked()
{
    ui->csvConfigWidget->setVisible(false);
    ui->csvImportWidget->setVisible(true);
    ui->csvImportWidget->setTableModel(ui->csvConfigWidget->getTableModel());
    ui->finish->setVisible(true);
}

void ImportDialog::on_finish_clicked()
{
    close();
}
