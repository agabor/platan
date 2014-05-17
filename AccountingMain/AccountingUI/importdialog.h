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

#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H

#include <QDialog>
#include <QFile>
#include <QAbstractTableModel>
#include <statementtablemodel.h>
#include <memory>
#include <csvreader.h>

namespace Ui {
class ImportDialog;
}

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDialog(QWidget *parent, QString filename);
    ~ImportDialog();

private slots:
    void on_buttonBox_accepted();
    void readCSV();

private:
    Ui::ImportDialog *ui;
    std::unique_ptr<StatementTableModel> dbModel;
    CSVReader reader;
    QByteArray input;
};

#endif // IMPORTDIALOG_H
