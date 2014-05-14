#include "mainwindow.h"
#include "login.h"

#include "ResourceManagers.h"
#include "LogManager.h"


#include <QApplication>
#include <QtCore/QTextCodec>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    ///< 各类资源的初始化
    if (!ResourceManagers::getSingleton().initiateResources())
    {
        return 0;
    }

    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));

    LogManager::getSingleton().logDebug("设置程序的字体为 GBK");

    LogManager::getSingleton().logDebug("程序登录成功");

    MainWindow w;

    LogManager::getSingleton().logDebug("启动画板系统");

    w.show();

    return a.exec();
}
