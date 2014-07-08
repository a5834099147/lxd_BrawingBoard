#include "round.h"

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

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
