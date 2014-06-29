#include <coloredsquare.h>
#include <QPainter>

ColoredSquare::ColoredSquare(QWidget *parent, QColor color, int size)
    : QWidget(parent), color(color), size(size)
{
    this->setFixedSize(size,size);
}

void ColoredSquare::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.drawRect(0,0,size, size);
}
