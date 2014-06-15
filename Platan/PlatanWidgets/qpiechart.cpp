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

#include "qpiechart.h"
#include <QPainter>
#include <math.h>
#include <iostream>
#define M_PI 3.14159265358979323846


QPieChart::QPieChart(QWidget *parent) :
    QWidget(parent)
{
    setMouseTracking(true);
    bounding_rect.setTop(0);
    highlighted_slice = -1;
    pressed = 0;
}

void QPieChart::init(QVector<float> values, ColorPalette *palette)
{
    pie_num = values.size();
    float sum = 0;
    for (int i = 0; i < pie_num; ++i)
    {
        sum += values[i];
    }

    int start_angle = 0;

    slices.clear();
    for (int i = 0; i < pie_num; ++i)
    {
        const int angle = (values[i] / sum) * (360 * 16);
        slices.push_back(PieSlice(palette->getColor(i), &bounding_rect, start_angle, angle));
        start_angle = start_angle + angle;
    }

    highlighted_slice = -1;
}

void QPieChart::paintEvent(QPaintEvent *event)
{
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(palette().background(), 2.0));
    for (int i = 0; i < pie_num; ++i)
    {
        slices[i].paint(&painter);
    }
    painter.setBrush(palette().background());
    painter.drawEllipse(QPointF(center_x, center_y), inner_r(), inner_r());
    painter.end();
}

void QPieChart::resizeEvent(QResizeEvent *)
{
    int side = width() < height() ? width() : height();
    bounding_rect.setLeft((width() - side) / 2);
    bounding_rect.setWidth(side);
    bounding_rect.setHeight(side);
    r = side / 2;
    center_y = r;
    center_x = bounding_rect.left() + center_y;
}

void QPieChart::mouseMoveEvent ( QMouseEvent * event )
{
    const int rel_y = center_y - event->y();
    const int rel_x = event->x() - center_x;

    setHighlightedSlice(rel_x, rel_y);
}

void QPieChart::mouseReleaseEvent(QMouseEvent *event)
{
    if (pressed == 0)
        return;

    pressed->setState(NONE);
    const int rel_y = center_y - event->y();
    const int rel_x = event->x() - center_x;

    const int idx = getSliceIndex(rel_x, rel_y);
    if (idx != -1 && pressed == &slices[idx])
        emit sliceClicked(idx);
    pressed = 0;
    repaint();
}

void QPieChart::mousePressEvent(QMouseEvent *event)
{
    const int rel_y = center_y - event->y();
    const int rel_x = event->x() - center_x;

    const int idx = getSliceIndex(rel_x, rel_y);
    if (idx == -1)
        return;

    pressed = &slices[idx];
    pressed->setState(CLICKED);
    repaint();
}

void QPieChart::setHighlightedSlice(int rel_x, int rel_y)
{
    int new_highlighted = getSliceIndex(rel_x, rel_y);

    if (new_highlighted == -1)
        return;

    highlight(new_highlighted);
}

int QPieChart::getSliceIndex(int rel_x, int rel_y)
{
    if (!isInPie(rel_x, rel_y))
    {
        highlight(-1);
        return -1;
    }

    int angle = getAngle(rel_x, rel_y);

    return getSliceIndex(angle);
}

bool QPieChart::isInPie(int rel_x, int rel_y)
{
    int dist2 = rel_y * rel_y + rel_x * rel_x;
    const float innerr = inner_r();
    return dist2 <= r * r && dist2 >= innerr * innerr;
}

int QPieChart::getSliceIndex(int angle)
{
    for (int i = 0; i < pie_num - 1; ++i)
    {
        if (slices[i + 1].startAngle() > angle)
        {
            return i;
        }
    }
    return pie_num - 1;
}

int QPieChart::getAngle(int rel_x, int rel_y)
{
    int angle = atan2(rel_y, rel_x) * 180.0 / M_PI;
    if (angle < 0)
        angle += 360;
    angle *= 16;
    return angle;
}

void QPieChart::highlight (int new_highlighted)
{
    if (new_highlighted != highlighted_slice)
    {
        if (highlighted_slice != -1)
        {
            PieSlice *s = &slices[highlighted_slice];
            if (s->getState() != CLICKED)
                s->setState(NONE);
        }

        if (new_highlighted != -1)
        {
            PieSlice *s = &slices[new_highlighted];
            if (s->getState() != CLICKED)
                slices[new_highlighted].setState(HOVER);
        }

        highlighted_slice = new_highlighted;

        repaint();
    }
}
