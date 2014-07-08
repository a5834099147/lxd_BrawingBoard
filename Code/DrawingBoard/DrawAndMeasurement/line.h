#ifndef LINE_H
#define LINE_H

#include "shape.h"

class Line : public Shape
{
public:
    Line();

    void save(QDataStream &dataStream);
    void load(QDataStream &dataStream);

    void setStart(QPoint e);
    void setUnderway(QPoint e);
    void paint(QPainter &painter);

    QString className()
    {
        return "Line";
    }
};

#endif // LINE_H
