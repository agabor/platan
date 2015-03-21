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

#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <QApplication>
#include <QMap>
#include <QVector>
#include <QSettings>

#include <memory>

#include <rulemapper.h>
#include <projectswindow.h>

class DataBaseSchema;
class Rule;
class MainWindow;
class ApplicationDB;
class ProjectDB;

class MainApplication : public QApplication
{
    Q_OBJECT
public:
    explicit MainApplication(int &argc, char *argv[], ApplicationDB &applicationDB, ProjectDB &projectDB);
    void setDateRange(QDate start, QDate end);

    bool OpenProject(QString project_path);
    QVector<QString> RecentProjects();
    void SaveProjectPaths(QVector<QString> path_list);
    QVector<Rule> getRulesForCountry(QString code) const;
    static MainApplication *getInstance();
    void create(QString data_base_path, QString countryCode);
    void setMainWindow(std::shared_ptr<MainWindow> mainWindow);
    void showProjectWindow();
private:
    std::unique_ptr<ProjectsWindow> projects_window;
    QMap<QString, int> countryCodes;
    QSettings settings;
    ProjectDB &projectDB;
    ApplicationDB &applicationDB;
    std::shared_ptr<MainWindow> mainWindow;
    static MainApplication *instance;
private slots:
    void cleanUp();
};

#endif // MAINAPPLICATION_H
