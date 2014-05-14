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
    LogManager::getSingleton().logDebug("�߳�: " + 
                boost::lexical_cast<std::string>(thrd_fd_) + "��ʼ����");
    exec();
}

void ServerThread::sockDisconnected()
{
    LogManager::getSingleton().logDebug("�߳�: " + 
                boost::lexical_cast<std::string>(thrd_fd_) + 
                "ֹͣ����, �����׽��ֺ���Ϊ: " + 
                boost::lexical_cast<std::string>(sock_fd));
    emit exiting(thrd_fd_);
    exit();
}
