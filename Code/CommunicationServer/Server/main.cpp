#include <QtCore/QCoreApplication>
#include <QtCore/QTextCodec>
#include <windows.h>
#include <stdlib.h>

#include "LogManager.h"
#include "ResourceManagers.h"
#include "serversocket.h"
#include "odb_user.h"

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

int main(int argc, char* argv[])
{
	///< 各类资源的初始化
    if (!ResourceManagers::getSingleton().initiateResources())
    {
        return 0;
    }

    QCoreApplication a(argc, argv);

    odb_user odb;

    ServerSocket server(7733);
    if (!server.start())
    {
        LogManager::getSingleton().logAlert("当服务器调用监听时出错");
        return 0;
    }

    return a.exec();
}