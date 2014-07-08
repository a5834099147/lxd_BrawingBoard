#include "ResourceManagers.h"
#include "LogManager.h"
#include "PathManager.h"

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

///< 资源类实体, 将使用范围现在在本文件内
static ResourceManagers resourceManagers;

///< 初始化 static 类型指针
template<> ResourceManagers* Singleton<ResourceManagers>::m_Singleton = 0;

ResourceManagers::ResourceManagers(void)
    :m_isInitiated(false)
{
    m_logManager = new LogManager();
    m_pathManager = new PathManager();
}


ResourceManagers::~ResourceManagers(void)
{
    delete m_logManager;
    delete m_pathManager;

    ///< 释放类空间后将指针置为NULL
    m_logManager = NULL;
    m_pathManager = NULL;
}

ResourceManagers& ResourceManagers::getSingleton()
{
    assert(m_Singleton);
    return *m_Singleton;
}

ResourceManagers* ResourceManagers::getSingletonPtr()
{
    assert(m_Singleton);
    return m_Singleton;
}

bool ResourceManagers::initiateResources()
{
    std::string logPath = m_pathManager->getCommunicationLogComfigFile();
    bool bSuc = m_logManager->createLog(logPath);

    if (!bSuc)
    {
        assert(false);
        return false;
    }

    LogManager::getSingleton().logDebug("服务器开启");
    return true;
}
