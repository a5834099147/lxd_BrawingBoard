#include "round.h"

Round::Round()
{
}

void Round::paint(QPainter &painter)
{
  
  painter.setPen(pen_);  
  painter.setBrush(brush_);
  painter.drawEllipse(start.x(), start.y(),
                      end.x() - start.x(),
                      end.y() - start.y());
}

void Round::setUnderway(QPoint e)
{
  end = e;
}

void Round::setStart(QPoint e)
{
  start = e;
}

void Round::save(QDataStream &dataStream)
{
	dataStream << start << end << pen_ << brush_;
}

void Round::load(QDataStream &dataStream)
{
	dataStream >> start >> end >> pen_ >> brush_;
}
