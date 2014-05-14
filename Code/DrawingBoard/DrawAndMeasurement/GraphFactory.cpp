//////////////////////////////////////////////////////////////////////////
///< ����Ϊ����ͼ�εĹ�����
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
            LogManager::getSingleton().logDebug("����ֱ��ʵ��");
            shape = new Line();
            break;
        }
    case Shape::Rect:
        {
            LogManager::getSingleton().logDebug("������Բʵ��");
            shape = new Rect();
            break;
        }
    case Shape::Round:
        {
            LogManager::getSingleton().logDebug("��������ʵ��");
            shape = new Round();
            break;
        }
    case Shape::RoundRect:
        {
            LogManager::getSingleton().logDebug("����Բ�Ǿ���ʵ��");
            shape = new RoundRect();
            break;
        }
    case Shape::Points:
        {
            LogManager::getSingleton().logDebug("��������ֱ��ʵ��");
            shape = new Points;
            break;
        }
    default:
        {
            LogManager::getSingleton().logError("���ַǷ�����");
            assert(false);
        }
    }

    return shape;
}
