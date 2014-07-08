#ifndef __PATHMANAGER_H__
#define __PATHMANAGER_H__

#include <string>

#include "Singleton.h"


class PathManager : public Singleton<PathManager>
{
public:
    PathManager(void);

    ~PathManager(void);

    ///< 获取单件类的实例对象
    static PathManager& getSingleton(void);

    ///< 获得单件类的实例指针
    static PathManager* getSingletonPtr(void);

public:
    ///< 获得绘图工作区日志配置文件路径
    std::string getPlottingLogConfigFile();

    ///< 获取服务器工作区日志配置文件路径
    std::string getCommunicationLogComfigFile();

private:
    ///< 获取配置文件路径
    std::string getConfigurePath();

};

#endif  ///< __PATHMANAGER_H__
