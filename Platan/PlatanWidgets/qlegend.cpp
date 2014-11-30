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

#include <QLayout>
#include <QColor>
#include <QString>

#include <qlegend.h>
#include <qlegenditem.h>

const int minimal_height = 10;

QLegend::QLegend(QWidget *parent) :
    QWidget(parent)
{
    setLayout(new QVBoxLayout());
    setFixedHeight(minimal_height);
}

void QLegend::addItem(QColor color, QString name)
{
    auto li = new QLegendItem(color, name, this);
    layout()->addWidget(li);
    setFixedHeight(height() + li->height());
    items.append(li);
}

void QLegend::clear()
{
    while(!items.empty())
    {
        layout()->removeWidget(items.first());
        delete items.first();
        items.pop_front();
    }
    setFixedHeight(minimal_height);
}
