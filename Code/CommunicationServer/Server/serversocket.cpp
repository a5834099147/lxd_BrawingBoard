#include "serversocket.h"
#include "serverthread.h"
#include "LogManager.h"

#include <boost/lexical_cast.hpp>
#include <QHostAddress>
#include <cassert>

quint32 ServerSocket::thrd_fd_ = 0;

ServerSocket::ServerSocket(quint16 port, QObject *parent)
    : QTcpServer(parent), port_(port)
{
    threads_.empty();
    mutex_ = new QMutex();
    assert(mutex_ != NULL);
}

ServerSocket::~ServerSocket()
{
    mutex_->lock();
    threads_.clear();
    mutex_->unlock();

    delete mutex_;
    mutex_ = NULL;
}

bool ServerSocket::start()
{
    ///< 开启服务, 调用监听
    if (!listen(QHostAddress::Any, port_))
    {
        LogManager::getSingleton().logAlert("系统调用监听失败");
        LogManager::getSingleton().logDebug("失败原因:" + errorString().toStdString());
        return false;
    }
    else 
    {
        LogManager::getSingleton().logDebug("系统调用监听成功");
        return true;
    }
}

void ServerSocket::incomingConnection(int sockfd)
{
    ///< 客户端有SOCKET加入
    ServerThread* thrd = new ServerThread(++thrd_fd_, sockfd);
    ///< 绑定退出消息于该线程, 当线程退出时调用该方法
    connect(thrd, SIGNAL(exiting(quint32)), this, SLOT(thrdExiting(quint32)));

    ///< 将线程加入到线程链表中
    mutex_->lock();
    threads_.push_back(thrd);
    mutex_->unlock();

    ///< 开启线程
    LogManager::getSingleton().logDebug("新作业线程开启");
    thrd->start();
}

void ServerSocket::thrdExiting(quint32 thrdfd)
{
    mutex_->lock();
    int count = threads_.count();
    for (int i = 0; i < count; ++i)
    {
        if (threads_.at(i)->getThrdfd() == thrdfd)
        {
            LogManager::getSingleton().logDebug("删除现有第 " + 
                        boost::lexical_cast<std::string>(i + 1) + "个线程");
            threads_.removeAt(i);
            break;
        }
    }
    mutex_->unlock();
}