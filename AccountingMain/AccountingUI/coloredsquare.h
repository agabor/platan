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

#ifndef COLOREDSQUARE_H
#define COLOREDSQUARE_H

#include <QWidget>
#include <QPainter>
#include <QColor>


class ColoredSquare : public QWidget
{
    Q_OBJECT
public:
    explicit ColoredSquare(QWidget *parent, QColor color, int size) : QWidget(parent), color(color), size(size)
    {
        this->setFixedSize(size,size);
    }
    void paintEvent(QPaintEvent *event)
    {
        QPainter painter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush(color);
        painter.drawRect(0,0,size, size);
    }
private:
    QColor color;
    int size;
};

#endif // COLOREDSQUARE_H
