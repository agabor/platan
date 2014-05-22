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
//
// This file is a modified version of a software published under
// the following licence:
//
// Copyright (C) 2011 Mateusz Loskot <mateusz@loskot.net>
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Blog article: http://mateusz.loskot.net/?p=2819

#include <functional>
#include <iostream>
#include <string>
#include <Python.h>
#include "pythonstdout.h"


struct Stdout
{
    PyObject_HEAD
    stdout_write_type write;
};

PyObject* Stdout_write(PyObject* self, PyObject* args)
{
    std::size_t written(0);
    Stdout* selfimpl = reinterpret_cast<Stdout*>(self);
    if (selfimpl->write)
    {
        char* data;
        if (!PyArg_ParseTuple(args, "s", &data))
            return 0;

        std::string str(data);
        selfimpl->write(str);
        written = str.size();
    }
    return PyLong_FromSize_t(written);
}

PyObject* Stdout_flush(PyObject* self, PyObject* args)
{
    // no-op
    return Py_BuildValue("");
}

PyMethodDef Stdout_methods[] =
{
    {"write", Stdout_write, METH_VARARGS, "sys.stdout.write"},
    {"flush", Stdout_flush, METH_VARARGS, "sys.stdout.write"},
    {0, 0, 0, 0} // sentinel
};

PyTypeObject StdoutType =
{
    PyVarObject_HEAD_INIT(0, 0)
    "emb.StdoutType",     /* tp_name */
    sizeof(Stdout),       /* tp_basicsize */
    0,                    /* tp_itemsize */
    0,                    /* tp_dealloc */
    0,                    /* tp_print */
    0,                    /* tp_getattr */
    0,                    /* tp_setattr */
    0,                    /* tp_reserved */
    0,                    /* tp_repr */
    0,                    /* tp_as_number */
    0,                    /* tp_as_sequence */
    0,                    /* tp_as_mapping */
    0,                    /* tp_hash  */
    0,                    /* tp_call */
    0,                    /* tp_str */
    0,                    /* tp_getattro */
    0,                    /* tp_setattro */
    0,                    /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,   /* tp_flags */
    "emb.Stdout objects", /* tp_doc */
    0,                    /* tp_traverse */
    0,                    /* tp_clear */
    0,                    /* tp_richcompare */
    0,                    /* tp_weaklistoffset */
    0,                    /* tp_iter */
    0,                    /* tp_iternext */
    Stdout_methods,       /* tp_methods */
    0,                    /* tp_members */
    0,                    /* tp_getset */
    0,                    /* tp_base */
    0,                    /* tp_dict */
    0,                    /* tp_descr_get */
    0,                    /* tp_descr_set */
    0,                    /* tp_dictoffset */
    0,                    /* tp_init */
    0,                    /* tp_alloc */
    0,                    /* tp_new */
};

PyModuleDef embmodule =
{
    PyModuleDef_HEAD_INIT,
    "emb", 0, -1, 0,
};

// Internal state
PyObject* g_stdout;
PyObject* g_stdout_saved;

struct Output
{
    Output(std::string name)
    {
        out = nullptr;
        out_saved = nullptr;
        this->name = name;
    }

    PyObject* out;
    PyObject* out_saved;
    std::string name;
};

Output py_stdout("stdout");
Output py_stderr("stderr");

PyObject*  PyInit_emb()
{
    StdoutType.tp_new = PyType_GenericNew;
    if (PyType_Ready(&StdoutType) < 0)
        return 0;

    PyObject* m = PyModule_Create(&embmodule);
    if (m)
    {
        Py_INCREF(&StdoutType);
        PyModule_AddObject(m, "Stdout", reinterpret_cast<PyObject*>(&StdoutType));
    }
    return m;
}

void set_output(stdout_write_type write, Output output)
{
    if (!output.out)
    {
        output.out_saved = PySys_GetObject(output.name.c_str()); // borrowed
        output.out = StdoutType.tp_new(&StdoutType, 0, 0);
    }

    Stdout* impl = reinterpret_cast<Stdout*>(output.out);
    impl->write = write;
    PySys_SetObject(output.name.c_str(), output.out);
}

void emb::set_stdout(stdout_write_type write)
{
    set_output(write, py_stdout);
}


void emb::set_stderr(stdout_write_type write)
{
    set_output(write, py_stderr);
}

void reset_output(Output output)
{
    if (output.out_saved)
        PySys_SetObject(output.name.c_str(), output.out_saved);

    Py_XDECREF(output.out);
    output.out = nullptr;
}

void emb::reset_stdout()
{
    reset_output(py_stdout);
}

void emb::reset_stderr()
{
    reset_output(py_stderr);
}



void emb::init()
{
    PyImport_AppendInittab("emb", &PyInit_emb);
}


void emb::import()
{
    PyImport_ImportModule("emb");
}
