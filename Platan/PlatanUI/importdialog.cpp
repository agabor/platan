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

#include <importdialog.h>
#include <ui_importdialog.h>
#include <statement.h>

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

QVector<Statement> ImportDialog::getImportedStatements() const
{
    return importedStatements;
}

void ImportDialog::on_cancel_clicked()
{
    reject();
}

void ImportDialog::on_next_clicked()
{
    ui->csvConfigWidget->setVisible(false);
    ui->csvImportWidget->setVisible(true);
    ui->csvImportWidget->setTableModel(ui->csvConfigWidget->getTableModel());
    ui->finish->setVisible(true);
    ui->next->setVisible(false);
}

QString ImportDialog::getFieldNameList(QVector<ColumnType> fields)
{
    QString fieldNameList;
    for (ColumnType type : fields)
    {
        if (!fieldNameList.isEmpty())
            fieldNameList += ", ";
        fieldNameList += getFieldName(type);
    }

    return fieldNameList;
}

void ImportDialog::on_finish_clicked()
{
    const TableTransformer &transformer = ui->csvImportWidget->getTransformer();
    QVector<ColumnType> unsetMandatoryFields = transformer.unsetMandatoryFields();
    if (unsetMandatoryFields.isEmpty())
    {
        QVector<ColumnType> unsetNotMandatoryFields = transformer.unsetNotMandatoryFields();

        if (!unsetNotMandatoryFields.isEmpty())
        {

            QMessageBox::StandardButton reply;
            QString msg(tr("The following fields are unset: %1. Do you want to continue?"));
            reply = QMessageBox::question(this, "Continue", msg.arg(getFieldNameList(unsetNotMandatoryFields)),
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::No)
                return;
        }

        importedStatements = ui->csvImportWidget->getImportedStatements();

        if (transformer.errorInImport())
        {
            QMessageBox::StandardButton reply;
            QString msg(tr("There were errors while importing:\n%1\nDo you want to continue?"));

            reply = QMessageBox::question(this, "Continue", msg.arg(transformer.getErrorMessage()), QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::No)
                return;
        }
        accept();
    }
    else
    {
        QString msg(tr("The following mandatory fields are unset: "));
        QMessageBox::warning(this, tr("Error"), msg +  getFieldNameList(unsetMandatoryFields));
    }
}

QString ImportDialog::getFieldName(ColumnType type)
{
    switch (type) {
    case ColumnType::Amount:
        return tr("Amount");
    case ColumnType::Date:
        return tr("Date");
    case ColumnType::Payee:
        return tr("Payee");
    case ColumnType::PayeeAccount:
        return tr("Payee account");
    case ColumnType::Type:
        return tr("Type");
    case ColumnType::Description:
        return tr("Description");
    }
    return QString();
}
