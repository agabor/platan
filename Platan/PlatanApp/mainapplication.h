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
#include "mainwindow.h"
#include <QMap>
#include <QVector>
#include <QTableView>
#include "statementtablemodel.h"
#include <memory>
#include "statements.h"
#include "pythonide.h"
#include "projectswindow.h"
#include <QSettings>

class MainApplication : public QApplication
{
    Q_OBJECT
public:
    explicit MainApplication(int &argc, char *argv[]);
    void setDateRange(QDate start, QDate end);
    ~MainApplication();
    PythonIDE *getPythonConsole() const
    {
        return python_console.get();
    }
    bool OpenProject(QString project_path);
    QVector<QString> RecentProjects();
    void SaveProjectPaths(QVector<QString> path_list);
private:
    std::unique_ptr<MainWindow> main_window;
    std::unique_ptr<PythonIDE> python_console;
    std::unique_ptr<ProjectsWindow> projects_window;
    Statements statements;
    QSettings settings;
};

#endif // MAINAPPLICATION_H
