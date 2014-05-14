//////////////////////////////////////////////////////////////////////////
///< 该类为创建图形的工厂类
//////////////////////////////////////////////////////////////////////////
#include "GraphFactory.h"

#include "line.h"
#include "points.h"
#include "rect.h"
#include "round.h"
#include "roundrect.h"
#include "shape.h"

#include "LogManager.h"

GraphFactory::GraphFactory(void)
{
}


GraphFactory::~GraphFactory(void)
{
}

Shape* GraphFactory::CreateGraph( Shape::Code type )
{
    Shape* shape = NULL;

    switch(type)
    {
    case Shape::Line:
        {
            LogManager::getSingleton().logDebug("创建直线实体");
            shape = new Line();
            break;
        }
    case Shape::Rect:
        {
            LogManager::getSingleton().logDebug("创建椭圆实体");
            shape = new Rect();
            break;
        }
    case Shape::Round:
        {
            LogManager::getSingleton().logDebug("创建矩形实体");
            shape = new Round();
            break;
        }
    case Shape::RoundRect:
        {
            LogManager::getSingleton().logDebug("创建圆角矩形实体");
            shape = new RoundRect();
            break;
        }
    case Shape::Points:
        {
            LogManager::getSingleton().logDebug("创建自由直线实体");
            shape = new Points;
            break;
        }
    default:
        {
            LogManager::getSingleton().logError("出现非法创建");
            assert(false);
        }
    }

    return shape;
}
