//********************************************************************
//    RESOURCEMANAGERS.H    �ļ�ע��
//    �ļ���  : RESOURCEMANAGERS.H
//    ����    : ������
//    TEL     : 18628980285
//    ����    : a5834099147@126.com
//    ����ʱ��: 2014/5/12 20:46
//    �ļ�����: ����Ϊ��Դ������, ά������־�Լ���ַ�ĵ���ʵ��. �Ҹ��౾��Ϊ����, ��
//              ����һ����̬ʵ��, ��ϵͳ����ʱ, ��ϵͳ�����ڴ�, ��������������־���ַ
//              ����ʵ����ڴ�ռ�
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

    ///< ��ȡ�������ʵ������
    static ResourceManagers& getSingleton();

    ///< ��ȡ�������ʵ��ָ��
    static ResourceManagers* getSingletonPtr();

public:
    ///< ��ʼ����־��
    bool ResourceManagers::initiateResources();

protected:
    ///< �Ƿ��Ѿ���ʼ��
    bool m_isInitiated;

    ///< ��־��ʵ��ָ��
    LogManager* m_logManager;
    ///< ��ַ��ʵ��ָ��
    PathManager* m_pathManager;
};

#endif __RESOURCEMANAGERS_H__
