#ifndef __Log_H__
#define __Log_H__

#include <log4cpp/Priority.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

// 根据log4cpp的级别重新定义消息级别
enum LogMessageLevel
{
    LML_EMERG = log4cpp::Priority::EMERG,
    LML_FATAL = log4cpp::Priority::EMERG,
    LML_ALERT = log4cpp::Priority::ALERT,
    LML_CRIT = log4cpp::Priority::CRIT,
    LML_ERROR = log4cpp::Priority::ERROR, 
    LML_WARN = log4cpp::Priority::WARN,
    LML_NOTICE = log4cpp::Priority::NOTICE,
    LML_INFO = log4cpp::Priority::INFO,
    LML_DEBUG = log4cpp::Priority::DEBUG,
    LML_NOTSET = log4cpp::Priority::NOTSET
};

/// 输出日志到标准输出或文件
class Log
{
public:
    /// 构建函数
    Log(const std::string& name, log4cpp::Category& c);

    /// 析构函数
    virtual ~Log(){};

public:
    /// 获取名称
    std::string getLogName();

    /// 设置消息级别
    void setMessageLevel(LogMessageLevel level);

    /// 获取消息级别
    LogMessageLevel getMessageLevel();

    /// 输出消息
    void logMessage(const std::string& messge, LogMessageLevel level = LML_DEBUG);

protected:
    std::string m_LogName;
    log4cpp::Category& m_Category;
};

#endif

