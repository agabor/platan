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

std::string get_file_contents(const char *filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

static PyObject* PyInit_Accounting();

MainApplication *PythonAPI::main_application;

PythonAPI::PythonAPI()
{
}

void PythonAPI::init(MainApplication *main_application)
{
    PythonAPI::main_application = main_application;
    PyImport_AppendInittab("accounting", &PyInit_Accounting);
    emb::init();
    Py_Initialize();
    PyImport_ImportModule("accounting");
    emb::import();
}

void PythonAPI::finalize()
{
    Py_Finalize();
}

void PythonAPI::run(std::string script)
{
    auto console = main_application->getPythonConsole();
    stdout_write_type write_output = [&console] (std::string s) { console->PushOutput(QString(s.c_str())); };
    emb::set_stdout(write_output);
    stdout_write_type write_error = [&console] (std::string s) { console->PushError(QString(s.c_str())); };
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

    std::cout << script << std::endl;

    PythonAPI::run(std::string(script));

    return PyLong_FromLong(0);
}

static PyMethodDef AccountingMethods[] = {
    {"setDateRange", setDateRange, METH_VARARGS, "blabla"},
    {"runScript", runScript, METH_VARARGS, "blabla"},
    {NULL, NULL, 0, NULL}
};


static PyModuleDef AccountingModule = {
    PyModuleDef_HEAD_INIT, "accounting", NULL, -1, AccountingMethods,
    NULL, NULL, NULL, NULL
};

static PyObject* PyInit_Accounting()
{
    return PyModule_Create(&AccountingModule);
}
