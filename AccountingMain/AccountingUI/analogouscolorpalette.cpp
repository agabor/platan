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

#include "analogouscolorpalette.h"

AnalogousColorPalette::AnalogousColorPalette(int colorcode, int hue_size)
    : hue_size (hue_size)
{
    base_color = colorFromCode(colorcode);
    hue_interval_start = base_color.hsvHue();
    hue_interval_end = hue_interval_start + hue_size;
}

void AnalogousColorPalette::init(int count)
{
    if (count == 1)
    {
        colors.push_back(base_color);
        return;
    }
    const float hue_angle = hue_size / (count - 1);
    QColor color = base_color;
    const float base_hue = base_color.hsvHue();
    float hue = base_hue;
    int sat = color.hsvSaturation();
    int val = color.value();
    for (int i = 0; i < count; ++i)
    {
        colors.push_back(color);
        hue += hue_angle;
        if (hue > hue_interval_end)
            hue -= hue_interval_start + hue - hue_interval_end;
        color.setHsv((int)hue, sat, val);
    }
}

QColor AnalogousColorPalette::colorFromCode(int colorcode)
{
    const int blue = colorcode % 256;
    colorcode = colorcode >> 8;
    const int green = colorcode % 256;
    colorcode = colorcode >> 8;
    const int red = colorcode;
    return QColor(red, green, blue);
}

QColor AnalogousColorPalette::getColor(int i)
{
    return colors[i % colors.size()];
}
