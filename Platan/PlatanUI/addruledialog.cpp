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

#include "addruledialog.h"
#include "ui_addruledialog.h"

AddRuleDialog::AddRuleDialog(QMap<int, QString> classes, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddRuleDialog)
{
    ui->setupUi(this);
    accepted = false;
}

AddRuleDialog::~AddRuleDialog()
{
    delete ui;
}

int AddRuleDialog::Class() const
{
    return 0;
}

QString AddRuleDialog::name() const
{
    return QString();
}

bool AddRuleDialog::ok() const
{
  return accepted;
}

void AddRuleDialog::on_AddRuleDialog_accepted()
{
    accepted = true;
}
