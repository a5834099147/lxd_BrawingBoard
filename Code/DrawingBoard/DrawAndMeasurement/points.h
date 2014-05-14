#ifndef POINTS_H
#define POINTS_H

#include "shape.h"
#include <QtGui/QPolygonF>

class Points : public Shape
{
public:
  Points();

  void save(QDataStream &dataStream);
  void load(QDataStream &dataStream);

  void setStart(QPoint e);
  void paint(QPainter &painter);
  void setUnderway(QPoint e);

  QString className()
  {
	  return "Points";
  }

private:
   QVector<QPoint> lines_;
};

#endif // POINTS_H
