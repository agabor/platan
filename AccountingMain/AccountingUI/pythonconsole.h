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

#ifndef PYTHONSCRIPTWINDOW_H
#define PYTHONSCRIPTWINDOW_H

#include <QDialog>
#include <QString>
#include <QPlainTextEdit>
#include <QTextEdit>


class QsciScintilla;

class PythonConsole : public QDialog
{
    Q_OBJECT

public:
    explicit PythonConsole(QWidget *parent = 0);
    ~PythonConsole();
    void PushOutput(QString output);
    void PushError(QString output);
private slots:
    void run();

private:
    QsciScintilla *code_window;
    QTextEdit *output_window;

};

#endif // PYTHONSCRIPTWINDOW_H
