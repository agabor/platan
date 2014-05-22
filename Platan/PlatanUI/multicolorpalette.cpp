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

#include "multicolorpalette.h"
#include <stdexcept>

void MultiColorPalette::init(int count)
{
    this->count = count;
    if (count == 0)
        return;
    float item_size = interval_size() / count;
    int idx = 0;

    for (auto &i : *this)
    {
        int item_count = i.size() / item_size;
        if (idx++ == size() -1)
        {
            i.init(count);
        } else
        {
            i.init(item_count);
            count -= item_count;
        }
    }
}

QColor MultiColorPalette::getColor(int i)
{
    i = i % count;
    for (auto &it : *this)
    {
        if (it.count() > i)
            return it.getColor(i);
        i -= it.count();
    }
    throw std::exception();
}
