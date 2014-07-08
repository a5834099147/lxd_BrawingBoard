#include <Windows.h>
#include <assert.h>
#include "Utility.h"

#include "LogManager.h"
#include "PathManager.h"

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

template<> PathManager* Singleton<PathManager>::m_Singleton = 0;

PathManager::PathManager(void)
{
}


PathManager::~PathManager(void)
{
}

std::string PathManager::getConfigurePath()
{
    return getExecuteDir() + "Configure\\";
}

std::string PathManager::getPlottingLogConfigFile()
{
    return getConfigurePath() + "Plottinglog.attr";
}

PathManager& PathManager::getSingleton( void )
{
    return *m_Singleton;
}

PathManager* PathManager::getSingletonPtr( void )
{
    return m_Singleton;
}

std::string PathManager::getCommunicationLogComfigFile()
{
    return getConfigurePath() + "Communicationlog.attr";
}
