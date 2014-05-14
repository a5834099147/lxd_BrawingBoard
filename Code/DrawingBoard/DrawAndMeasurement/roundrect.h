#ifndef ROUNDRECT_H
#define ROUNDRECT_H

#include "shape.h"

class RoundRect : public Shape
{
public:
  RoundRect();

  void save(QDataStream &dataStream);
  void load(QDataStream &dataStream);

  void setStart(QPoint e);
  void setUnderway(QPoint e);
  void paint(QPainter &painter);

  QString className()
  {
	  return "R_oundRect";
  }
};

#endif // ROUNDRECT_H
