#include "Log.h"



Log::Log( const std::string& name, log4cpp::Category& c )
    : m_LogName(name), m_Category(c)
{

}

std::string Log::getLogName()
{
    return(m_LogName);
}

LogMessageLevel Log::getMessageLevel()
{
    return(static_cast<LogMessageLevel>(m_Category.getPriority()));
}

void Log::setMessageLevel( LogMessageLevel level )
{
    m_Category.setPriority(static_cast<log4cpp::Priority::Value>(level));
}

void Log::logMessage( const std::string& messge, LogMessageLevel level /*= LML_DEBUG*/ )
{
    m_Category << static_cast<log4cpp::Priority::Value>(level) << messge;
}



