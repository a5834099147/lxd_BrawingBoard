#ifndef ROUND_H
#define ROUND_H

#include "shape.h"

class Round : public Shape
{
public:
  Round();

  void save(QDataStream &dataStream);
  void load(QDataStream &dataStream);

  void setStart(QPoint e);
  void setUnderway(QPoint e);
  void paint(QPainter &painter);

  QString className()
  {
	  return "Round";
  }
};

#endif // ROUND_H
