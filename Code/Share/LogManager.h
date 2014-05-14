

#ifndef __LogManager_H__
#define __LogManager_H__

#include "Log.h"
#include "SmartPointer.h"
#include "Singleton.h"

/// ��־������
class LogManager : public Singleton<LogManager>
{
public:
    LogManager();

    ~LogManager();

    /// ��ȡ�������ʵ������
    static LogManager& getSingleton();

    /// ��ȡ�������ʵ��ָ��
    static LogManager* getSingletonPtr();

public:
    /// �������ļ��д�����־��
    bool createLog(std::string propertyFile);

    /// �����Ϣ
    void logMessage(const std::string& messge, LogMessageLevel level = LML_DEBUG);

    /// ���������Ϣ
    void logEmerg(const std::string& messge){logMessage(messge, LML_EMERG);}

    void logAlert(const std::string& messge){logMessage(messge, LML_ALERT);}

    /// ���������Ϣ
    void logError(const std::string& messge){logMessage(messge, LML_ERROR);}

    /// ���������Ϣ
    void logWarn(const std::string& messge){logMessage(messge, LML_WARN);}

    /// ���������Ϣ
    void logDebug(const std::string& messge){logMessage(messge, LML_DEBUG);}

private:
    /// �ͷ���־�����Դ
    void releaseLog();

private:
    typedef std::vector<SharePtr<Log>::type> LogList;

    /// �������е���־
    LogList m_Logs;
};

#endif

