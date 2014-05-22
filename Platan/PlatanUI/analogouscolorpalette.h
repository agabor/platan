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

#ifndef ANALOGOUSCOLORPALETTE_H
#define ANALOGOUSCOLORPALETTE_H

#include <QColor>
#include <QVector>
#include "colorpalette.h"

class AnalogousColorPalette : public ColorPalette
{
public:
    AnalogousColorPalette(int colorcode, int hue_size = 359);
    void init(int count);
    QColor getColor(int i);
    int size() const
    {
        return hue_size;
    }
    int count() const
    {
        return colors.size();
    }

protected:
    QVector<QColor> colors;
    QColor base_color;
    QColor colorFromCode(int colorcode);
    int hue_interval_start, hue_interval_end;
    int hue_size;
private:
    AnalogousColorPalette() {}
    friend class QVector<AnalogousColorPalette>;
};

#endif // ANALOGOUSCOLORPALETTE_H
