#include <QtCore/QCoreApplication>
#include <QtCore/QTextCodec>

#include "LogManager.h"
#include "ResourceManagers.h"
#include "serversocket.h"
#include "odb_user.h"

int main(int argc, char* argv[])
{
    ///< 各类资源的初始化
    if (!ResourceManagers::getSingleton().initiateResources())
    {
        return 0;
    }

    QCoreApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));

    odb_user odb;

    ServerSocket server(7733);
    if (!server.start())
    {
        LogManager::getSingleton().logAlert("当服务器调用监听时出错");
        return 0;
    }

    return a.exec();
}