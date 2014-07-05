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
#include <PythonAPI/pythonapi.h>
#include <country.h>
#include <pythonide.h>
#include <mainwindow.h>
#include <projectswindow.h>
#include <rule.h>

using namespace std;

MainApplication *MainApplication::instance(nullptr);

MainApplication::MainApplication(int &argc, char *argv[]) :
    QApplication(argc, argv),
    settings("configs/platan.ini", QSettings::IniFormat),
    application_db(getApplicationDBSchema(), "app"),
    project_db(getProjectDBSchema(), "pro"),
    countryMapper(application_db),
    ruleMapper(project_db),
    statements(project_db),
    rules(project_db),
    viewModel(statements)
{
    assert(instance == nullptr);
    instance = this;
#ifdef PYTHON_API
    PythonAPI::init(this);
#endif
    projects_window.reset(new ProjectsWindow(this, statements));
    projects_window->show();

    application_db.setPath("platandata");
    application_db.open();
    for(Country c : countryMapper.getAll())
        countryCodes.insert(c.code, c.id);
}


bool MainApplication::OpenProject(QString project_path)
{
    if(project_db.isOpen())
        project_db.close();
    project_db.setPath(project_path);
    if(!project_db.open())
        return false;

    statements.init();
    rules.init();

    main_window.reset(new MainWindow(this, statements, rules, viewModel));

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


DataBaseSchema MainApplication::getProjectDBSchema()
{
    DataBaseSchema schema;
    schema.addTable(RuleMapper::getStructure());
    schema.addTable(StatementMapper::getStructure());

    return schema;
}

DataBaseSchema MainApplication::getApplicationDBSchema()
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
#ifdef PYTHON_API
    PythonAPI::finalize();
#endif
}
