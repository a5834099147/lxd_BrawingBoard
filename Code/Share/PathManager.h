#ifndef __PATHMANAGER_H__
#define __PATHMANAGER_H__

#include <string>

#include "Singleton.h"


class PathManager : public Singleton<PathManager>
{
public:
    PathManager(void);

    ~PathManager(void);

    ///< ��ȡ�������ʵ������
    static PathManager& getSingleton(void);

    ///< ��õ������ʵ��ָ��
    static PathManager* getSingletonPtr(void);

public:
    ///< ��û�ͼ��������־�����ļ�·��
    std::string getPlottingLogConfigFile();

    ///< ��ȡ��������������־�����ļ�·��
    std::string getCommunicationLogComfigFile();

private:
    ///< ��ȡ�����ļ�·��
    std::string getConfigurePath();

private:
    ///< ��õ�ǰ��������Ŀ¼
    std::string getExecuteDir();
};

#endif  ///< __PATHMANAGER_H__
