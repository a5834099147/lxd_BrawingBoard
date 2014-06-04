#include "rect.h"

Rect::Rect()
{
}

void Rect::paint(QPainter &painter)
{  
    painter.setPen(pen_);  
    painter.setBrush(brush_);
    painter.drawRect(start.x(), start.y(),
        end.x() - start.x(),
        end.y() - start.y());
}

void Rect::setUnderway(QPoint e)
{
    end = e;
}

void Rect::setStart(QPoint e)
{
    start = e;
}

void Rect::save(QDataStream &dataStream)
{
    dataStream << start << end << pen_ << brush_;
}

void Rect::load(QDataStream &dataStream)
{
    dataStream >> start >> end >> pen_ >> brush_;
}