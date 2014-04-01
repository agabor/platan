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

#include "choosedatedialog.h"
#include "ui_choosedatedialog.h"

ChooseDateDialog::ChooseDateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseDateDialog)
{
    ui->setupUi(this);
    _ok = false;
}

ChooseDateDialog::~ChooseDateDialog()
{
    delete ui;
}

QDate ChooseDateDialog::date() const
{
   return ui->calendarWidget->selectedDate();
}

bool ChooseDateDialog::ok() const
{
    return _ok;
}

void ChooseDateDialog::on_ChooseDateDialog_accepted()
{
    _ok = true;
}
