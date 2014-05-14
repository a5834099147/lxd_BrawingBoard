#include "serverthread.h"
#include "connectsocket.h"
#include "LogManager.h"

#include <boost/lexical_cast.hpp>

ServerThread::ServerThread(quint32 thrdfd, int sockfd, QObject *parent)
    : QThread(parent),
    disconnected_(true),
    thrd_fd_(thrdfd),
    sock_fd(sock_fd)
{
    sock_ = new ConnectSocket();
    sock_->setSocketDescriptor(sockfd);
    disconnected_ = false;
    connect(sock_, SIGNAL(disconnected()), sock_, SLOT(deleteLater()));
    connect(sock_, SIGNAL(disconnected()), this, SLOT(sockDisconnected()));
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

ServerThread::~ServerThread()
{
}

void ServerThread::run()
{
    LogManager::getSingleton().logDebug("线程: " + 
                boost::lexical_cast<std::string>(thrd_fd_) + "开始运行");
    exec();
}

void ServerThread::sockDisconnected()
{
    LogManager::getSingleton().logDebug("线程: " + 
                boost::lexical_cast<std::string>(thrd_fd_) + 
                "停止运行, 它的套接字号码为: " + 
                boost::lexical_cast<std::string>(sock_fd));
    emit exiting(thrd_fd_);
    exit();
}
