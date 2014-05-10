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
#include <accdatabase.h>
#include <QMap>
#include <QVector>
#include <QTableView>
#include "statementtablemodel.h"
#include <memory>
#include "statements.h"
#include "pythonide.h"

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
private:
    void OpenDataBase(const char *data_base_path);
    std::unique_ptr<MainWindow> main_window;
    std::unique_ptr<PythonIDE> python_console;
    Statements statements;

};

#endif // MAINAPPLICATION_H
