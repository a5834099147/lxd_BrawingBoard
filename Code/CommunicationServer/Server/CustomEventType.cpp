#include "CustomEventType.h"
#include "common.h"

ChangeTheList::ChangeTheList( const QString& name, bool onLine )
    :CustomEventType(ET_CHANGETHELIST),
    m_userName(name),
    m_isOnLine(onLine)
{
}

