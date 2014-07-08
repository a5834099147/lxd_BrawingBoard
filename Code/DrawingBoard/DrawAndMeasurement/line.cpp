#include "line.h"

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

Line::Line()
{	
}

void Line::paint(QPainter &painter)
{   
    painter.setPen(pen_);  
    painter.setBrush(brush_);
    painter.drawLine(start, end);
}

void Line::setUnderway(QPoint e)
{
    end = e;
}

void Line::setStart(QPoint e)
{
    start = e;
}

void Line::save(QDataStream &dataStream)
{
    dataStream << start << end << pen_ << brush_;
}

void Line::load(QDataStream &dataStream)
{
    dataStream >> start >> end >> pen_ >> brush_;
}
