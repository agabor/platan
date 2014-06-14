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

#include "mainapplication.h"
#include "PythonAPI/pythonapi.h"
#include <QMessageBox>
#include <sqlitedb.h>
#include <assert.h>
#include <country.h>
using namespace std;

MainApplication *MainApplication::instance(nullptr);

MainApplication::MainApplication(int &argc, char *argv[]) :
    QApplication(argc, argv),
    settings("configs/platan.ini", QSettings::IniFormat),
    db(getSchema(), "app"),
    countryMapper(db),
    ruleMapper(db)
{
    assert(instance == nullptr);
    instance = this;
    PythonAPI::init(this);
    projects_window.reset(new ProjectsWindow(this, statements));
    projects_window->show();

    db.setPath("platandata");
    db.open();
    for(Country c : countryMapper.getAll())
        countryCodes.insert(c.code, c.id);
}


bool MainApplication::OpenProject(QString project_path)
{
    if(!statements.open(project_path))
    {
        return false;
    }

    main_window.reset(new MainWindow(this, statements));

    main_window->InitChart();
    main_window->InitLegend();

    main_window->show();

    python_console.reset(new PythonIDE(main_window.get()));

    return true;
}

const QString projectpathkey{"misc/projectpath%1"};
const QString projectnumkey{"misc/projectnum"};

QVector<QString> MainApplication::RecentProjects()
{
    QString projectNum = settings.value(projectnumkey, QString{"0"}).toString();
    bool ok;
    int count = projectNum.toInt(&ok);
    QVector<QString> result;
    for(int i = 0; i < count; ++i)
    {

        QString projectpath = settings.value(projectpathkey.arg(i), QString{""}).toString();
        if (!projectpath.isEmpty())
            result.push_back(projectpath);
    }
    return result;
}

void MainApplication::SaveProjectPaths(QVector<QString> path_list)
{
    settings.setValue(projectnumkey, QString::number(path_list.length()));
    int i = 0;
    for(QString path : path_list)
    {
        settings.setValue(projectpathkey.arg(i++), path);
    }

}

bool MainApplication::countryExists(QString code) const
{
    return countryCodes.keys().contains(code);
}

QVector<Rule> MainApplication::getRulesForCountry(QString code) const
{
    auto it = countryCodes.find(code);
    if (it == countryCodes.end())
        return QVector<Rule>();
    return ruleMapper.getAll(*it);
}

MainApplication *MainApplication::getInstance()
{
    return instance;
}

DataBaseSchema MainApplication::getSchema()
{
    DataBaseSchema schema;
    schema.addTable(RuleMapper::getStructureWithCountry());

    TableStructure countries{"countries"};
    countries.addField("ID", SQLType::Integer().PK());
    countries.addField("Code", SQLType::Text());
    schema.addTable(countries);
    return schema;
}

void MainApplication::setDateRange(QDate start, QDate end)
{
    main_window->setDateRange(start, end);
}

MainApplication::~MainApplication()
{
    PythonAPI::finalize();
}
