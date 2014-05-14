#include <assert.h>

#include "LogManager.h"

template<> LogManager* Singleton<LogManager>::m_Singleton = 0;
LogManager::LogManager()
{
    m_Logs.clear();
}

LogManager::~LogManager()
{
    releaseLog();
}

LogManager& LogManager::getSingleton()
{
    assert(m_Singleton);
    return *m_Singleton;
}

LogManager* LogManager::getSingletonPtr()
{
    assert(m_Singleton);
    return m_Singleton;
}

bool LogManager::createLog( std::string propertyFile )
{
    try{
        // 加载log配置文件
        log4cpp::PropertyConfigurator::configure(propertyFile);

        // 解析配置文件Category
        // getCurrentCategories返回的指针如果不删除会造成内存泄露
        boost::shared_ptr<std::vector<log4cpp::Category*> >ret(log4cpp::Category::getCurrentCategories());
        // std::vector<log4cpp::Category*>* ret = log4cpp::Category::getCurrentCategories();
        std::vector<log4cpp::Category*>::iterator itor = ret->begin();
        for (; itor != ret->end(); ++itor)
        {
            {
                std::string str = (*itor)->getName();
                log4cpp::Category& temp = log4cpp::Category::getInstance(str);
                m_Logs.push_back(MakeShared<Log>(str, temp));
            }
        }
    }
    catch(log4cpp::ConfigureFailure& f)
    {
        std::string err = "Log4Cpp Configure Problem : ";
        err += f.what();
        OutputDebugStringA(err.c_str());
        assert(false);
    }

    return(true);
}

void LogManager::logMessage( const std::string& messge, LogMessageLevel level /*= LML_DEBUG*/ )
{
    for each (SharePtr<Log>::type log in m_Logs)
    {
        log->logMessage(messge, level);
    }
}

void LogManager::releaseLog()
{
    m_Logs.clear();
}
