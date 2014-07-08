#include "mainwindow.h"
#include "login.h"

#include "ResourceManagers.h"
#include "LogManager.h"


#include <QApplication>
#include <QtCore/QTextCodec>
#include <QMessageBox>

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

int main(int argc, char *argv[])
{
    ///< 各类资源的初始化
    if (!ResourceManagers::getSingleton().initiateResources())
    {
        return 0;
    }

    QApplication a(argc, argv);

    LogManager::getSingleton().logDebug("设置程序的字体为 GBK");

    LogManager::getSingleton().logDebug("程序登录成功");

    MainWindow w;

    LogManager::getSingleton().logDebug("启动画板系统");

    w.show();

    return a.exec();
}
