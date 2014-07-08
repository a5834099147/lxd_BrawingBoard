#include "ResourceManagers.h"
#include "LogManager.h"
#include "PathManager.h"

///< UTF-8��������, ����������ʾ������������, ǰ�����ĵ��ı����ʽΪUTF-8
#pragma execution_character_set("utf-8")

///< ��Դ��ʵ��, ��ʹ�÷�Χ�����ڱ��ļ���
static ResourceManagers resourceManagers;

///< ��ʼ�� static ����ָ��
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

    ///< �ͷ���ռ��ָ����ΪNULL
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
    std::string logPath = m_pathManager->getPlottingLogConfigFile();
    bool bSuc = m_logManager->createLog(logPath);

    if (!bSuc)
    {
        assert(false);
        return false;
    }

    LogManager::getSingleton().logDebug("ϵͳ����");
    return true;
}
