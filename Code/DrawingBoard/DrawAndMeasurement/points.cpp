#include "points.h"

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

Points::Points()
{
}

void Points::paint(QPainter &painter)
{  
    painter.setPen(pen_); 
    painter.setBrush(brush_);
    painter.drawLines(lines_);
}

void Points::setUnderway(QPoint e)
{
    lines_ << e;
    lines_ << e;
}

void Points::setStart(QPoint e)
{
    lines_ << e;
}

void Points::save(QDataStream &dataStream)
{
    dataStream << start << end << pen_ << brush_ << lines_;
}

void Points::load(QDataStream &dataStream)
{
    dataStream >> start >> end >> pen_ >> brush_ >> lines_;
}
