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

#ifndef MULTICOLORPALETTE_H
#define MULTICOLORPALETTE_H

#include "colorpalette.h"
#include "analogouscolorpalette.h"
#include <QVector>

class MultiColorPalette : public ColorPalette, public QVector<AnalogousColorPalette>
{
public:
    MultiColorPalette(){}
    void order();
    virtual void init(int count);
    virtual QColor getColor(int i);
protected:
    int interval_size() const
    {
        int result = 0;
        for (int i = 0; i < size(); ++i)
        {
            result += (*this)[i].size();
        }
        return result;
    }
    int count;
};

#endif // MULTICOLORPALETTE_H
