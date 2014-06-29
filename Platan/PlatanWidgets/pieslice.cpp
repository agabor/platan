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

#include "pieslice.h"
#include <QPainter>

PieSlice::PieSlice(QColor color, QRect *pie_bounding_rect, int start_angle, int span_angle)
    : base_color(color), pie_bounding_rect(pie_bounding_rect), start_angle(start_angle), span_angle(span_angle)
{
    hover_color = base_color;
    hover_color.setHsv(hover_color.hsvHue(), hover_color.hsvSaturation()/2, hover_color.value());
    click_color = base_color;
    click_color.setHsv(click_color.hsvHue(), (click_color.hsvSaturation() + 255)/2, click_color.value());
    state = NONE;
}

void PieSlice::paint(QPainter *painter)
{
    switch(state)
    {
    case HOVER:
        painter->setBrush(hover_color);
        break;
    case CLICKED:
        painter->setBrush(click_color);
        break;
    case NONE:
        painter->setBrush(base_color);
        break;
    }

    painter->drawPie(*pie_bounding_rect, start_angle, span_angle);
}
