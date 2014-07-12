#include <QtCore/QCoreApplication>
#include <Boost/lexical_cast.hpp>

#include "LogManager.h"
#include "ResourceManagers.h"
#include "serversocket.h"


///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

///< 服务器监听的端口号
const static int ServerPort = 7733;

int main(int argc, char* argv[])
{
	
	///< 各类资源的初始化
    if (!ResourceManagers::getSingleton().initiateResources())
    {
        return 0;
    }

	///< QT控制台程序实例
    QCoreApplication a(argc, argv);

	///< 服务器类实体
	ServerSocket server(ServerPort);

    if (!server.start())
    {
        LogManager::getSingleton().logAlert("Listening for alerts from this appliance on Port " + boost::lexical_cast<std::string>(ServerPort));
        return 0;
    }


    return a.exec();
}