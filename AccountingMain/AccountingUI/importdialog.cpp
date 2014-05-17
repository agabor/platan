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
//    dbModel = std::unique_ptr<StatementTableModel> (new StatementTableModel());
//    dbModel->Ammount_tr = new FloatTransformation(8, ',', true);
//    dbModel->Type_tr = new IdentityTransFormation(3, true);
//    dbModel->Date_tr = new DateTransformation(2, DMY, '.', true);
//    dbModel->Payee_tr = new IdentityTransFormation(5, true);
//    dbModel->PayeeAccount_tr = new IdentityTransFormation(6, true);
//    dbModel->Description_tr = new IdentityTransFormation(4, true);
//    dbModel->setData(model);

//    ui->tableView->setModel(model);
//    ui->tableViewDB->setModel(dbModel.get());
}

ImportDialog::~ImportDialog()
{
    delete ui;
}

//void ImportDialog::on_buttonBox_accepted()
//{
//    AccDataBase db;
//    db.SetPath("../accountdb");
//    db.InsertData(*dbModel);
//}

void ImportDialog::on_cancel_clicked()
{
    close();
}

void ImportDialog::on_next_clicked()
{
    ui->csvConfigWidget->setVisible(false);
    ui->csvImportWidget->setVisible(true);
    ui->csvImportWidget->setTableModel(ui->csvConfigWidget->getTableModel());
}
