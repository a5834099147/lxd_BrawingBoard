#include <Windows.h>
#include <assert.h>

#include "LogManager.h"
#include "PathManager.h"

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

std::string PathManager::getExecuteDir()
{
    char szPath[256];
    if (!GetModuleFileNameA(NULL, szPath, 256))
    {
        LogManager::getSingletonPtr()->logError("获取当前运行目录时出错");
        assert(false);
    }

    std::string path(szPath);

    ///< 找到当前 exe 运行程序的路径后裁剪到上级目录
    path = path.substr(0, path.rfind("\\") + 1);
    return path;
}

std::string PathManager::getLogConfigFile()
{
    return getConfigurePath() + "log.attr";
}

PathManager& PathManager::getSingleton( void )
{
    return *m_Singleton;
}

PathManager* PathManager::getSingletonPtr( void )
{
    return m_Singleton;
}
