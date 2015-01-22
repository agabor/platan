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
    ui->comboBox->setCurrentIndex(87); // select Germany
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


void NewProjectDialog::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Create new project"), "",
                                                    tr("Platan files (*.plat)"));
    auto withoutPath = fileName.split('/').last();
    QStringList parts = withoutPath.split('.');
    parts.removeAll(QString());
    if (parts.count() < 2 || parts.last() != "plat")
        fileName += ".plat";
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
