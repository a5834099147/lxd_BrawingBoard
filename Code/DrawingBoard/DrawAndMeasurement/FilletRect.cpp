#include "FilletRect.h"

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

FilletRect::FilletRect()
{
}

void FilletRect::paint(QPainter &painter)
{  
  painter.setPen(pen_);  
  painter.setBrush(brush_);
  painter.drawRoundRect(start.x(), start.y(),
                      end.x() - start.x(),
                      end.y() - start.y());
}

void FilletRect::setUnderway(QPoint e)
{
  end = e;
}

void FilletRect::setStart(QPoint e)
{
  start = e;
}

void FilletRect::save(QDataStream &dataStream)
{
	dataStream << start << end << pen_ << brush_;
}

void FilletRect::load(QDataStream &dataStream)
{
	dataStream >> start >> end >> pen_ >> brush_;
}


