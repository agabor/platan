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

#ifndef PIESLICE_H
#define PIESLICE_H

#include <QColor>

class QPainter;

enum PieSliceState
{
    HOVER,
    CLICKED,
    NONE
};

class PieSlice
{
public:
    PieSlice(QColor base_color, QRect *pie_bounding_rect, int start_angle, int span_angle);
    void paint(QPainter *painter);
    void setState(PieSliceState state)
    {
        this->state = state;
    }
    int startAngle() const
    {
        return start_angle;
    }
    PieSliceState getState() const
    {
        return state;
    }

protected:
    QColor base_color;
    QColor hover_color;
    QColor click_color;
    QRect *pie_bounding_rect;
    int start_angle;
    int span_angle;
    PieSliceState state;

private:
    PieSlice(){}
friend class QVector<PieSlice>;
};

#endif // PIESLICE_H
