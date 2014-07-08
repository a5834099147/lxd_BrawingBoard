#ifndef ROUNDRECT_H
#define ROUNDRECT_H

#include "shape.h"

class FilletRect : public Shape
{
public:
  FilletRect();

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
