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

#include "qstatemenview.h"
#include <QAction>
#include <QMenu>
#include <QMouseEvent>
#include "addruledialog.h"

QStatemenView::QStatemenView(QWidget *parent) :
    QTableView(parent)
{
    QAction* setClass = new QAction("Set class", this);
    addAction(setClass);
    connect(setClass, SIGNAL(triggered()), this, SLOT(onSetClass()));
}

void QStatemenView::mouseReleaseEvent(QMouseEvent *ev)
{
    QMenu MyMenu(this);
    MyMenu.addActions(this->actions());
    MyMenu.exec(ev->globalPos());
}

void QStatemenView::onSetClass()
{
    QModelIndex index = selectionModel()->currentIndex();
    emit SetClass(index);
}

