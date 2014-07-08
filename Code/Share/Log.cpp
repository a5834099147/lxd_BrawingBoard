#include "Log.h"

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

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



