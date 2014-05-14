#ifndef SHAPE_H
#define SHAPE_H

#include <QObject>
#include <QtGui/QtGui>
#include <QDataStream>

class Shape : public QObject
{
	Q_OBJECT

public:
  Shape();

  enum Code{
    Line,
    Rect,
    Round,
    RoundRect,
    Points
  };

  enum Colour{
    Black,
    Blue,
    Green,
    None,
    Red,
    Custom
  };

  enum PenStyle{
    SolidLine,
    DashLine,
    DotLine,
    DashDotLine,
    DashDotDotLine,
    NoPen
  };

  enum BrushStyle
  {
    SolidPattern,
    Dense1Pattern,
    Dense2Pattern,
    Dense3Pattern,
    Dense4Pattern,
    Dense5Pattern,
    Dense6Pattern,
    Dense7Pattern,
    HorPattern,
    VerPattern,
    CrossPattern,
    BDiagPattern,
    FDiagPattern,
    DiagCrossPat,
    NoBrush
  };

  QPoint startPoint()
  {
    return start;
  }

  QPoint endPoint()
  {
    return end;
  }

  void setPenColour(QColor penC)
  {
    penColour_ = penC;
  }

  void setBrushColour(QColor brushC)
  {
    brushColour_ = brushC;
  }

  void setPenStyle(Qt::PenStyle style)
  {
    penStyle_ = style;
  }

  void setBrushStyle(Qt::BrushStyle style)
  {
    BrushStyle_ = style;
  }

  void penRoughNess(int p)
  {
    penRoughNess_ = p;
  }

  void setPen()
  {
    pen_.setWidth(penRoughNess_);
    pen_.setColor(penColour_);
    pen_.setStyle(penStyle_);
  }

  void setBrush()
  {
    brush_.setColor(brushColour_);
    brush_.setStyle(BrushStyle_);
  }

  void updata()
  {
	  setPen();
	  setBrush();
  }

  void virtual setStart(QPoint e) = 0;
  void virtual setUnderway(QPoint e) = 0;
  void virtual paint(QPainter& painter) = 0;

  void virtual save(QDataStream &dataStream) = 0;
  void virtual load(QDataStream &dataStream) = 0;

  QString virtual className() = 0;
  
protected:
  QPoint start;
  QPoint end;
  QPen pen_;
  QBrush brush_;
  QColor penColour_;
  QColor brushColour_;
  int penRoughNess_;
  Qt::PenStyle penStyle_;
  Qt::BrushStyle BrushStyle_;
};

#endif // SHAPE_H
