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

#include "maintabwidget.h"

MainTabWidget::MainTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

void MainTabWidget::addTableViewTab(QTableView *table_view, const QString &name)
{
    addTab(table_view, name);
}

void MainTabWidget::removeCloseButtons()
{
    for(int i = 0; i < count(); ++i)
    {
        tabBar()->setTabButton(i, QTabBar::RightSide, 0);
        tabBar()->setTabButton(i, QTabBar::LeftSide, 0);
    }
}

void MainTabWidget::openLastTab()
{
    setCurrentIndex(count()-1);
}

bool MainTabWidget::isOpen(QString name) const
{
    for(int i = 0; i < count(); ++i)
    {
        if (tabBar()->tabText(i) == name)
            return true;
    }
    return false;
}

int MainTabWidget::getIndex(QString name) const
{
    for(int i = 0; i < count(); ++i)
    {
        if (tabBar()->tabText(i) == name)
            return i;
    }
    return -1;
}

void MainTabWidget::closeTab(int idx)
{
    removeTab(idx);
    setCurrentIndex(0);
}
