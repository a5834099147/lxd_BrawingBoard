#include "line.h"

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
