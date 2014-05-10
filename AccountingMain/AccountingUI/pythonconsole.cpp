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

#include "pythonconsole.h"
#include "pythonapi.h"
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerpython.h>

QAction * PythonConsole::AddAction(QString text, QString icon_name, const char* tooltip)
{
    QString icon_path{":/icons/icons/%1.png"};
    QIcon icon(icon_path.arg(icon_name));
    QAction *run_action = toolbar->addAction(icon, text);
    run_action->setStatusTip(tr(tooltip));

    return run_action;
}

PythonConsole::PythonConsole(QWidget *parent) :
    QDialog(parent)
{
    toolbar = new QToolBar();

    QAction *run_action = AddAction("Run", "run", "Run script");
    QAction *save_action = AddAction("Save", "document-save", "Save script");
    QAction *save_as_action = AddAction("Save", "document-save-as", "Save script as ...");
    QAction *load_action = AddAction("Load", "document-open", "Load script");

    connect(run_action, SIGNAL(triggered()), this, SLOT(run()));

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(toolbar);

    code_window = new QsciScintilla(this);
    code_window->setLexer(new QsciLexerPython());
    layout->addWidget(code_window);

    output_window = new QTextEdit(this);
    output_window->setReadOnly(true);
    layout->addWidget(output_window);

    setFixedSize(600, 500);
}

PythonConsole::~PythonConsole()
{
}

void PythonConsole::run()
{
    PythonAPI::run(code_window->text().toStdString());
}

void PythonConsole::PushOutput(QString output)
{
    output_window->insertPlainText(output);
}

void PythonConsole::PushError(QString output)
{
    output_window->insertHtml(QString("<font color=\"red\">%1</font><br>").arg(output));
}
