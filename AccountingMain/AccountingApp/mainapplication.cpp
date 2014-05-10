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

using namespace std;

MainApplication::MainApplication(int &argc, char *argv[]) :
    QApplication(argc, argv)
{
    PythonAPI::init(this);

    OpenDataBase("../accountdb");


    main_window.reset(new MainWindow(this, statements));

    main_window->InitChart();
    main_window->InitLegend();

    //connect(main_window.tabWidget(),SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    main_window->show();

    python_console.reset(new PythonIDE);
}

void MainApplication::setDateRange(QDate start, QDate end)
{
    main_window->setDateRange(start, end);
}


void MainApplication::OpenDataBase(const char *data_base_path)
{
    statements.Open(data_base_path);
}

MainApplication::~MainApplication()
{
    PythonAPI::finalize();
}
