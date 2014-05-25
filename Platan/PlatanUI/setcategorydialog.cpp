#include "setcategorydialog.h"
#include "ui_setcategorydialog.h"
#include <statements.h>

SetCategoryDialog::SetCategoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetCategoryDialog)
{
    ui->setupUi(this);
    ui->categories->addItems(Statements::categoryList);
}

SetCategoryDialog::~SetCategoryDialog()
{
    delete ui;
}

int SetCategoryDialog::category() const
{
    return ui->categories->currentIndex();
}
