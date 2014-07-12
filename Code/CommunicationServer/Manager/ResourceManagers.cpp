#include "ResourceManagers.h"
#include "LogManager.h"
#include "PathManager.h"
#include "OdbManager.h"

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

///< 资源类实体, 将使用范围现在在本文件内
static ResourceManagers resourceManagers;

///< 初始化 static 类型指针
template<> ResourceManagers* Singleton<ResourceManagers>::m_Singleton = 0;

ResourceManagers::ResourceManagers(void)
    :m_isInitiated(false)
{
	///< 创建日志实体
    m_logManager = new LogManager();
	///< 创建地址实体
    m_pathManager = new PathManager();
	///< 创建数据库实体
	m_odbManager = new OdbManager();
}


ResourceManagers::~ResourceManagers(void)
{
	///< 删除各实例
    delete m_logManager;
    delete m_pathManager;
	delete m_odbManager;

    ///< 释放类空间后将指针置为NULL
    m_logManager = NULL;
    m_pathManager = NULL;
	m_odbManager = NULL;
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
	///< 得到日志文件配置文件地址
    std::string logPath = m_pathManager->getCommunicationLogComfigFile();
	///< 根据配置文件创建日志文件
    bool bSuc = m_logManager->createLog(logPath);

	///< 查看日志是否创建成功
    if (!bSuc)
    {
        assert(false);
        return false;
    }

    LogManager::getSingleton().logDebug("Log server start.");
    return true;
}
