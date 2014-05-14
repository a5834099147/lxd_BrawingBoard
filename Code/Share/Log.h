#ifndef __Log_H__
#define __Log_H__

#include <log4cpp/Priority.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/PropertyConfigurator.hh>

// ����log4cpp�ļ������¶�����Ϣ����
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

/// �����־����׼������ļ�
class Log
{
public:
    /// ��������
    Log(const std::string& name, log4cpp::Category& c);

    /// ��������
    virtual ~Log(){};

public:
    /// ��ȡ����
    std::string getLogName();

    /// ������Ϣ����
    void setMessageLevel(LogMessageLevel level);

    /// ��ȡ��Ϣ����
    LogMessageLevel getMessageLevel();

    /// �����Ϣ
    void logMessage(const std::string& messge, LogMessageLevel level = LML_DEBUG);

protected:
    std::string m_LogName;
    log4cpp::Category& m_Category;
};

#endif

