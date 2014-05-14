#include "roundrect.h"

RoundRect::RoundRect()
{
}

void RoundRect::paint(QPainter &painter)
{  
  painter.setPen(pen_);  
  painter.setBrush(brush_);
  painter.drawRoundRect(start.x(), start.y(),
                      end.x() - start.x(),
                      end.y() - start.y());
}

void RoundRect::setUnderway(QPoint e)
{
  end = e;
}

void RoundRect::setStart(QPoint e)
{
  start = e;
}

void RoundRect::save(QDataStream &dataStream)
{
	dataStream << start << end << pen_ << brush_;
}

void RoundRect::load(QDataStream &dataStream)
{
	dataStream >> start >> end >> pen_ >> brush_;
}


