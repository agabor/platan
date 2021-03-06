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

#ifndef QPIECHART_H
#define QPIECHART_H

#include <QWidget>
#include <QPainter>
#include <QVector>

#include "pieslice.h"

class QMouseEvent;
class ColorPalette;
template <class Key, class T>
struct QPair;

class QPieChart : public QWidget
{
    Q_OBJECT
public:
    explicit QPieChart(QWidget *parent);
    void init(QVector<QPair<QColor, float>> values);
signals:
    void sliceClicked(int idx);
protected:
    void mouseMoveEvent ( QMouseEvent * event ) override;
	void mouseReleaseEvent(QMouseEvent * event) override;
	void mousePressEvent(QMouseEvent * event) override;
	void paintEvent(QPaintEvent *) override;
	void resizeEvent(QResizeEvent *) override;
    int pie_num;
    QPainter painter;
    QVector<PieSlice> slices;
    PieSlice *pressed;
    QRect bounding_rect;
    int center_x, center_y;
    float r;
    int highlighted_slice;
    inline void highlight(int new_highlighted);
    inline int getAngle(int rel_x, int rel_y);
    inline int getSliceIndex(int angle);
    inline bool isInPie(int rel_x, int rel_y);
    inline void setHighlightedSlice(int rel_x, int rel_y);
    float inner_r() const
    {
        return r / 3;
    }
    int getSliceIndex(int rel_x, int rel_y);
};

#endif // QPIECHART_H
