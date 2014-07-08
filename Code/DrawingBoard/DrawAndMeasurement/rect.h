#ifndef RECT_H
#define RECT_H

#include "shape.h"

class Rect : public Shape
{
public:
  Rect();

  void save(QDataStream &dataStream);
  void load(QDataStream &dataStream);

  void setStart(QPoint e);
  void setUnderway(QPoint e);
  void paint(QPainter &painter);

  QString className()
  {
	  return "Rect";
  }
};

#endif // RECT_H
