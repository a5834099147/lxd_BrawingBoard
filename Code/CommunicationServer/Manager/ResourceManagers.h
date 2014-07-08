//********************************************************************
//    RESOURCEMANAGERS.H    文件注释
//    文件名  : RESOURCEMANAGERS.H
//    作者    : 李晓东
//    TEL     : 18628980285
//    邮箱    : a5834099147@126.com
//    创建时间: 2014/5/12 20:46
//    文件描述: 该类为资源管理类, 维护了日志以及地址的单件实体. 且该类本身为单件, 但
//              具有一个静态实体, 当系统析构时, 有系统回收内存, 并由它来回收日志如地址
//              两个实体的内存空间
//*********************************************************************
#ifndef __RESOURCEMANAGERS_H__
#define __RESOURCEMANAGERS_H__

#include "Singleton.h"

class LogManager;
class PathManager;

class ResourceManagers : public Singleton<ResourceManagers>
{
public:
    ResourceManagers(void);

    ~ResourceManagers(void);

    ///< 获取单件类的实例对象
    static ResourceManagers& getSingleton();

    ///< 获取单件类的实例指针
    static ResourceManagers* getSingletonPtr();

public:
    ///< 初始化日志类
    bool ResourceManagers::initiateResources();

protected:
    ///< 是否已经初始化
    bool m_isInitiated;

    ///< 日志类实体指针
    LogManager* m_logManager;
    ///< 地址类实体指针
    PathManager* m_pathManager;
};

#endif __RESOURCEMANAGERS_H__
