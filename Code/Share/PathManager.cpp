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
        LogManager::getSingletonPtr()->logError("��ȡ��ǰ����Ŀ¼ʱ����");
        assert(false);
    }

    std::string path(szPath);

    ///< �ҵ���ǰ exe ���г����·����ü����ϼ�Ŀ¼
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
