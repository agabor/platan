#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include <QVector>
#include <QLocale>
#include <QDebug>
#include "country.h"



NewProjectDialog::NewProjectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
    QVector<CountryData> countryDatas;
    for (auto country : getAllCountries())
        countryDatas.append(CountryData(country));
    qSort(countryDatas);
    for (CountryData data : countryDatas)
    {
        QString res(":/icons/icons/%1.png");
        res = res.arg(data.code);
        ui->comboBox->addItem(QIcon(res), data.name);
    }
    ui->comboBox->setCurrentIndex(-1);
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

