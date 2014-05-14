#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QColor>

#include "shape.h"
#include "line.h"
#include "rect.h"
#include "round.h"
#include "roundrect.h"
#include "points.h"

class PaintWidget : public QWidget
{
  Q_OBJECT
public:
  explicit PaintWidget(QWidget *parent = 0);
  void cleanWidget();

signals:
  void dateIsChanged();

public slots:
  void setCurrentShape(Shape::Code s)
  {
    if (s != currShapeCode)
      {
        currShapeCode = s;
      }
  }  

  void setPenRoughNess(int r)
  {
    penRoughNess = r;
  }

  void setPenStyle(Shape::PenStyle s);
  void setBrushStyle(Shape::BrushStyle s);

  void setPenColour(Shape::Colour c);
  void setBrushColour(Shape::Colour);

  bool writeFile(const QString &fileName);
  bool readFile(const QString &fileName);  

private:
  QColor findColour(Shape::Colour);    

protected:
  void paintEvent(QPaintEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);

private:
  Shape::Code currShapeCode;
  Shape::Colour PenColour;
  Shape::Colour BrushColour;

  Shape *m_shape;
  bool perm;

  const static int symbol; 

  QColor penColor;
  QColor brushColor;
  int penRoughNess;
  Qt::PenStyle penStyle;
  Qt::BrushStyle brushStyle;
  bool isChanged;

  QList<Shape*> shapeList;
};

#endif // PAINTWIDGET_H
