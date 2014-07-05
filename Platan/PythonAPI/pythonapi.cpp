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

#include "pythonapi.h"
#include <Python.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cerrno>
#include "mainapplication.h"
#include "pythonstdout.h"
#include <memory>
#include <pythonide.h>

#define MODUL_NAME "Platan"

using namespace std;

string get_file_contents(const char *filename)
{
  ifstream in(filename, ios::in | ios::binary);
  if (in)
  {
    string contents;
    in.seekg(0, ios::end);
    contents.resize(in.tellg());
    in.seekg(0, ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

static PyObject* PyInit_Platan();

MainApplication *PythonAPI::main_application;
PythonIDE *PythonAPI::pythonIDE;

PythonAPI::PythonAPI()
{
}

void PythonAPI::init(MainApplication *main_application)
{
    PythonAPI::main_application = main_application;
    PyImport_AppendInittab(MODUL_NAME, &PyInit_Platan);
    emb::init();
    Py_Initialize();
    PyImport_ImportModule(MODUL_NAME);
    emb::import();
}

void PythonAPI::finalize()
{
    Py_Finalize();
}

void PythonAPI::run(string script)
{
    stdout_write_type write_output = [&pythonIDE] (string s) { pythonIDE->PushOutput(QString(s.c_str())); };
    emb::set_stdout(write_output);
    stdout_write_type write_error = [&pythonIDE] (string s) { pythonIDE->PushError(QString(s.c_str())); };
    emb::set_stderr(write_error);
    PyRun_SimpleString(script.c_str());
    emb::reset_stdout();
    emb::reset_stderr();
}

static PyObject* setDateRange(PyObject *self, PyObject *args)
{
    int y1, m1, d1;
    int y2, m2, d2;
    if(!PyArg_ParseTuple(args, "iiiiii", &y1, &m1, &d1, &y2, &m2, &d2))
        return NULL;

    PythonAPI::main_application->setDateRange(QDate(y1,m1,d1),QDate(y2,m2,d2));

    return PyLong_FromLong(0);
}


static PyObject* runScript(PyObject *self, PyObject *args)
{
    char *script;
    if(!PyArg_ParseTuple(args, "s", &script))
        return NULL;

    cout << script << endl;

    PythonAPI::run(string(script));

    return PyLong_FromLong(0);
}

static PyMethodDef PlatanMethods[] = {
    {"setDateRange", setDateRange, METH_VARARGS, "Set current date range in application. e.g.: Platan.setDateRange(2013, 8, 1, 2013, 8, 31)"},
    {"runScript", runScript, METH_VARARGS, "Run python script e.g.: Platan.runScript(\"print(\\\"hello\\\")\")"},
    {NULL, NULL, 0, NULL}
};


static PyModuleDef PlatanModule = {
    PyModuleDef_HEAD_INIT, MODUL_NAME, NULL, -1, PlatanMethods,
    NULL, NULL, NULL, NULL
};


vector<pair<QString, QString> > PythonAPI::GetFunctionDocs()
{
    vector<pair<QString, QString> > result;
    for(PyMethodDef& def : PlatanMethods)
    {
        if (def.ml_name == NULL)
            break;
        result.push_back(pair<QString, QString>(def.ml_name, def.ml_doc));
    }
    return result;
}

void PythonAPI::setPythonIDE(PythonIDE *pythonIDE)
{
    PythonAPI::pythonIDE = pythonIDE;
}

static PyObject* PyInit_Platan()
{
    return PyModule_Create(&PlatanModule);
}
