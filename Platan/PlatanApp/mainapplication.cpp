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

#include <QMessageBox>

#include <assert.h>

#include <mainapplication.h>
#include <country.h>
#include <pythonide.h>
#include <mainwindow.h>
#include <projectswindow.h>
#include <rule.h>
#include <pythonapi.h>
#include <applicationdb.h>

using namespace std;

MainApplication *MainApplication::instance(nullptr);


MainApplication::MainApplication(int &argc, char *argv[], ApplicationDB &applicationDB, SQLiteDB &project_db) :
    QApplication(argc, argv),
    settings("configs/platan.ini", QSettings::IniFormat),
    project_db(project_db),
    applicationDB(applicationDB),
    ruleMapper(project_db)
{
    assert(instance == nullptr);
    instance = this;
#ifdef PYTHON_API
    PythonAPI::init(this);
#endif
}


void MainApplication::showProjectWindow()
{
    for(Country c : applicationDB.getCountries())
        countryCodes.insert(c.code, c.id);

    projects_window.reset(new ProjectsWindow(this));
    projects_window->show();
}

bool MainApplication::OpenProject(QString project_path)
{
    if(project_db.isOpen())
        project_db.close();
    project_db.setPath(project_path);
    if(!project_db.open())
        return false;

    mainWindow->init();

    mainWindow->InitChart();
    mainWindow->InitLegend();

    mainWindow->show();

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
    return applicationDB.getRulesForCountry(*it);
}

MainApplication *MainApplication::getInstance()
{
    return instance;
}

void MainApplication::create(QString data_base_path, QString countryCode)
{
    project_db.setPath(data_base_path);
    project_db.create();
    project_db.beginTransaction();
    auto rules = getRulesForCountry(countryCode);
    for (Rule r : rules)
        ruleMapper.insert(r);
    project_db.endTransaction();
}

void MainApplication::setMainWindow(std::shared_ptr<MainWindow> mainWindow)
{
    this->mainWindow = mainWindow;
}


void MainApplication::setDateRange(QDate start, QDate end)
{
    mainWindow->setDateRange(start, end);
}

MainApplication::~MainApplication()
{
#ifdef PYTHON_API
    PythonAPI::finalize();
#endif
}
