#include "CustomEventType.h"
#include "common.h"

ChangeTheList::ChangeTheList( const QString& name, bool onLine )
    :CustomEventType(ET_CHANGETHELIST),
    m_userName(name),
    m_isOnLine(onLine)
{
}


ChatRequest::ChatRequest( const QString& name )
    :CustomEventType(ET_CHATREQUEST),
    m_userName(name)
{

}

ChatRequestResult::ChatRequestResult( const QString& name, bool result )
    :CustomEventType(ET_CHATREQUESTRESULT),
    m_userName(name),
    m_result(result)
{

}

OpenChatPort::OpenChatPort( const QString& ipAddress, const QString& userName, quint32 localPort, quint32 targetPort )
    :CustomEventType(ET_OPENCHATPORT),
    m_userName(userName),
    m_ipAddress(ipAddress),
    m_localPort(localPort),
    m_targetPort(targetPort)
{

}
