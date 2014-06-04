#include "paintwidget.h"

#include <QtGui/QColorDialog>
#include "LogManager.h"
#include "GraphFactory.h"


const int PaintWidget::symbol = 1311515;

PaintWidget::PaintWidget(QWidget *parent) 
    : QWidget(parent),
    currShapeCode(Shape::Line),
    m_shape(NULL),
    perm(false),
    penRoughNess(1),
    penStyle(Qt::SolidLine),
    brushStyle(Qt::SolidPattern),
    penColor(Qt::black),
    brushColor(Qt::white),
    isChanged(false)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void PaintWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawRect(0, 0, size().width(), size().height());

    foreach (Shape *shape, shapeList) {
        shape->paint(painter);
    }
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    GraphFactory graphFactor;
    m_shape = graphFactor.CreateGraph(currShapeCode);

    if (m_shape != NULL)
    {
        perm = false;

        if (!isChanged)
        {
            isChanged = true;
            LogManager::getSingleton().logDebug("文档发生改变");
            emit dateIsChanged();
        }

        shapeList << m_shape;
        LogManager::getSingleton().logDebug("正在绘制形状加入到图形队列中");

        m_shape->setBrushStyle(brushStyle);
        m_shape->setBrushColour(brushColor);
        m_shape->setPenStyle(penStyle);
        m_shape->penRoughNess(penRoughNess);
        m_shape->setPenColour(penColor);
        m_shape->setStart(event->pos());
        m_shape->setUnderway(event->pos());
        m_shape->updata();
    }
    else
    {

    }
}

bool PaintWidget::writeFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, tr("画板"),
            tr("无法写入文件 %1:\n%2.")
            .arg(file.fileName())
            .arg(file.errorString()));
        LogManager::getSingleton().logError((QString("写入文件发生错误, 文件名为: ") += fileName).toStdString());
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_8);

    out << quint32(symbol);

    QApplication::setOverrideCursor(Qt::WaitCursor);
    foreach (Shape *shape, shapeList) {
        QString className = shape->className();
        out << className;
        shape->save(out);
    }
    QApplication::restoreOverrideCursor();
    isChanged = false;
    return true;
}

bool PaintWidget::readFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, tr("画板"),
            tr("无法保存文件 %1:\n%2.")
            .arg(file.fileName())
            .arg(file.errorString()));
        LogManager::getSingleton().logError((QString("读出文件发生错误, 文件名为: ") 
            += fileName).toStdString());
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_8);
    QString className;
    quint32 symbolThisFile;
    in >> symbolThisFile;

    if (symbol != symbolThisFile)
    {
        QMessageBox::warning(this, tr("画板"),
            tr("这个文件不是一个合法的画板文件"));
        LogManager::getSingleton().logError((QString("读取文件非正规画板文件: ") += fileName).toStdString());
        return false;
    }

    cleanWidget();

    QApplication::setOverrideCursor(Qt::WaitCursor);
    while(!in.atEnd())
    {
        className.clear();
        in >> className;
        if (className == QString("Line"))
        {
            m_shape = new Line;
        } else if (className == QString("Points"))
        {
            m_shape = new Points;
        } else if (className == QString("R_oundRect"))
        {
            m_shape = new RoundRect;
        } else if (className == QString("Rect"))
        {
            m_shape = new Rect;
        } else if (className == QString("Round"))
        {
            m_shape = new Round;
        }

        if (m_shape != NULL)
        {
            m_shape->load(in);
        }
        shapeList << m_shape;
        m_shape = NULL;
    }

    QApplication::restoreOverrideCursor();
    update();
    isChanged = false;
    return true;
}

void PaintWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_shape && !perm)
    {
        m_shape->setUnderway(event->pos());
        update();
    }
}

void PaintWidget::mouseReleaseEvent(QMouseEvent* event)
{
    perm = true;
    LogManager::getSingleton().logDebug("绘制图形成功");
}

void PaintWidget::setPenColour(Shape::Colour c)
{
    if (c != PenColour)
    {
        LogManager::getSingleton().logDebug("重新选择画笔颜色");
        PenColour = c;
        penColor = findColour(PenColour);
    }  
}

void PaintWidget::setBrushColour(Shape::Colour c)
{
    if (c != BrushColour)
    {
        LogManager::getSingleton().logDebug("重新选择画刷颜色");
        BrushColour = c;
        brushColor = findColour(BrushColour);
    }
}

QColor PaintWidget::findColour(Shape::Colour colour)
{
    QColor tmp;

    switch(colour)
    {
    case Shape::Black:
        {
            tmp = QColor(0, 0, 0);
            break;
        }
    case Shape::Blue:
        {
            tmp = QColor(0, 0, 205);
            break;
        }
    case Shape::Green:
        {
            tmp = QColor(124, 252, 0);
            break;
        }
    case Shape::Red:
        {
            tmp = QColor(255, 0, 0);
            break;
        }
    case Shape::None:
        {
            tmp = QColor(255, 255, 255);
            break;
        }
    case Shape::Custom:
        {
            tmp = QColorDialog::getColor(Qt::white, this);
            break;
        }
    }

    return tmp;
}



void PaintWidget::setPenStyle(Shape::PenStyle s)
{
    switch(s)
    {
    case Shape::SolidLine     : penStyle = Qt::SolidLine;       break;
    case Shape::DashLine      : penStyle = Qt::DashLine;        break;
    case Shape::DotLine       : penStyle = Qt::DotLine;         break;
    case Shape::DashDotLine   : penStyle = Qt::DashDotLine;     break;
    case Shape::DashDotDotLine: penStyle = Qt::DashDotDotLine;  break;
    case Shape::NoPen         : penStyle = Qt::NoPen;           break;

    }
}

void PaintWidget::setBrushStyle(Shape::BrushStyle s)
{
    switch(s)
    {
    case Shape::SolidPattern    : brushStyle = Qt::SolidPattern;    break;
    case Shape::Dense1Pattern   : brushStyle = Qt::Dense1Pattern;   break;
    case Shape::Dense2Pattern   : brushStyle = Qt::Dense2Pattern;   break;
    case Shape::Dense3Pattern   : brushStyle = Qt::Dense3Pattern;   break;
    case Shape::Dense4Pattern   : brushStyle = Qt::Dense4Pattern;   break;
    case Shape::Dense5Pattern   : brushStyle = Qt::Dense5Pattern;   break;
    case Shape::Dense6Pattern   : brushStyle = Qt::Dense6Pattern;   break;
    case Shape::Dense7Pattern   : brushStyle = Qt::Dense7Pattern;   break;
    case Shape::HorPattern      : brushStyle = Qt::HorPattern;      break;
    case Shape::VerPattern      : brushStyle = Qt::VerPattern;      break;
    case Shape::CrossPattern    : brushStyle = Qt::CrossPattern;    break;
    case Shape::BDiagPattern    : brushStyle = Qt::BDiagPattern;    break;
    case Shape::FDiagPattern    : brushStyle = Qt::FDiagPattern;    break;
    case Shape::DiagCrossPat    : brushStyle = Qt::DiagCrossPattern;break;
    case Shape::NoBrush         : brushStyle = Qt::NoBrush;         break;
    }
}

void PaintWidget::cleanWidget()
{
    shapeList.clear();
    update();
    isChanged = false;
}

