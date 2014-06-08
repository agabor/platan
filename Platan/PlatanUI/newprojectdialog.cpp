#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include <QVector>
#include <QLocale>

QVector<QLocale::Country> getAllCountries();

class CountryData
{
public:
    CountryData()
    {}
    CountryData(QLocale::Country l)
    {
        name = QLocale::countryToString(l);
        code = QLocale(QLocale::AnyLanguage, l).name().split('_').at(1).toLower();
    }

    QString name;
    QString code;
    bool operator==(const CountryData &other) const
    {
        return name == other.name;
    }

    bool operator!=(const CountryData &other) const
    {
        return name == other.name;
    }

    bool operator<(const CountryData &other) const
    {
        return name < other.name;
    }

    bool operator>(const CountryData &other) const
    {
        return name > other.name;
    }

    bool operator<=(const CountryData &other) const
    {
        return name <= other.name;
    }

    bool operator>=(const CountryData &other) const
    {
        return name >= other.name;
    }
};

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
        ui->comboBox->addItem(QIcon(QString(":/icons/icons/%1.png").arg(data.code)), data.name);
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}

QVector<QLocale::Country> getAllCountries()
{
    QVector<QLocale::Country> result;
    for (int i = 1; i <= QLocale::LastCountry; ++i)
        result.push_back((QLocale::Country)i);
    return result;
}
