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

#ifndef ADDRULEDIALOG_H
#define ADDRULEDIALOG_H

#include <QDialog>

namespace Ui {
class AddRuleDialog;
}

class Rule;
class Statement;

class AddRuleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddRuleDialog(QWidget *parent = 0);
    ~AddRuleDialog();
    Rule getRule() const;
    QString name() const;
    void setRow(Statement row);
    void setColumn(int idx);


private:
    Ui::AddRuleDialog *ui;
};

#endif // ADDRULEDIALOG_H
