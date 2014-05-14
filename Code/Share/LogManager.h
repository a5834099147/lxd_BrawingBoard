

#ifndef __LogManager_H__
#define __LogManager_H__

#include "Log.h"
#include "SmartPointer.h"
#include "Singleton.h"

/// 日志管理类
class LogManager : public Singleton<LogManager>
{
public:
    LogManager();

    ~LogManager();

    /// 获取单件类的实例对像
    static LogManager& getSingleton();

    /// 获取单件类的实例指针
    static LogManager* getSingletonPtr();

public:
    /// 从配置文件中创建日志类
    bool createLog(std::string propertyFile);

    /// 输出消息
    void logMessage(const std::string& messge, LogMessageLevel level = LML_DEBUG);

    /// 输出紧急消息
    void logEmerg(const std::string& messge){logMessage(messge, LML_EMERG);}

    void logAlert(const std::string& messge){logMessage(messge, LML_ALERT);}

    /// 输出错误消息
    void logError(const std::string& messge){logMessage(messge, LML_ERROR);}

    /// 输出警告消息
    void logWarn(const std::string& messge){logMessage(messge, LML_WARN);}

    /// 输出调试信息
    void logDebug(const std::string& messge){logMessage(messge, LML_DEBUG);}

private:
    /// 释放日志相关资源
    void releaseLog();

private:
    typedef std::vector<SharePtr<Log>::type> LogList;

    /// 保存所有的日志
    LogList m_Logs;
};

#endif

