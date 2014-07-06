#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include <QVector>
#include <QDebug>
#include "countrydata.h"
#include <mainapplication.h>
#include <QFileDialog>

NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
    QVector<CountryData> countryDatas;
    for (auto country : CountryData::getAllCountries())
        countryDatas.append(CountryData(country));
    qSort(countryDatas);
    for (CountryData data : countryDatas)
    {
        QString res(":/icons/icons/flags/%1.png");
        res = res.arg(data.code);
        ui->comboBox->addItem(QIcon(res), data.name, data.code);
    }
    ui->comboBox->setCurrentIndex(0);
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

QString NewProjectDialog::fileName() const
{
    return ui->path->text();
}

QString NewProjectDialog::countryCode() const
{
    return ui->comboBox->currentData().toString();
}


void NewProjectDialog::on_comboBox_currentIndexChanged(int index)
{
    QString code = ui->comboBox->itemData(index).toString();
    if (MainApplication::getInstance()->countryExists(code))
        ui->ruleInfo->setText(tr("There is a predefined ruleset for this country.\nYour statements will be categorized automatically!"));
    else
        ui->ruleInfo->setText(tr("There is no ruleset for this country yet.\nBut do not worry! It is easy to create one."));
}

void NewProjectDialog::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Create new project"), "",
                                                    tr("Platan files (*.plat)"));
    ui->path->setText(fileName);
}

void NewProjectDialog::on_okBtn_clicked()
{
    accept();
}

void NewProjectDialog::on_cancelBtn_clicked()
{
    reject();
}
