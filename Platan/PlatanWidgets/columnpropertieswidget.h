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

#ifndef IMPORTCOLUMNPROPERTIES_H
#define IMPORTCOLUMNPROPERTIES_H

#include <QWidget>
#include <datetransformation.h>
class QComboBox;
class QLabel;
enum class ColumnType;

class ColumnPropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColumnPropertiesWidget(QWidget *parent = 0);
    ColumnType type();
    void setType(ColumnType type);
    void setSeparator(char c);
    void setDateOrder(DateOrder o);

signals:
    void typeChanged(ColumnType type);
    void separatorChanged(char c);
    void formatChanged(DateOrder o);
private slots:
    void setType(int idx);
    void setSeparator(int idx);
    void setDateOrder(int idx);

private:
    QComboBox *useAs;
    QLabel *useAsLabel;
    QComboBox *separator;
    QLabel *separatorLabel;
    QComboBox *format;
    QLabel *formatLabel;
    QLabel *explanation;
};

#endif // IMPORTCOLUMNPROPERTIES_H
