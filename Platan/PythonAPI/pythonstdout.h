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

#ifndef PYTHONSTDOUT_H
#define PYTHONSTDOUT_H


#include <Python.h>

typedef std::function<void(std::string)> stdout_write_type;

namespace emb
{
    void init();
    void set_stdout(stdout_write_type write);
    void reset_stdout();
    void set_stderr(stdout_write_type write);
    void reset_stderr();
    void import();
}
#endif // PYTHONSTDOUT_H
