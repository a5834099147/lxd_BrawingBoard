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
    ///< ��������, ���ü���
    if (!listen(QHostAddress::Any, port_))
    {
        LogManager::getSingleton().logAlert("ϵͳ���ü���ʧ��");
        LogManager::getSingleton().logDebug("ʧ��ԭ��:" + errorString().toStdString());
        return false;
    }
    else 
    {
        LogManager::getSingleton().logDebug("ϵͳ���ü����ɹ�");
        return true;
    }
}

void ServerSocket::incomingConnection(int sockfd)
{
    ///< �ͻ�����SOCKET����
    ServerThread* thrd = new ServerThread(++thrd_fd_, sockfd);
    ///< ���˳���Ϣ�ڸ��߳�, ���߳��˳�ʱ���ø÷���
    connect(thrd, SIGNAL(exiting(quint32)), this, SLOT(thrdExiting(quint32)));

    ///< ���̼߳��뵽�߳�������
    mutex_->lock();
    threads_.push_back(thrd);
    mutex_->unlock();

    ///< �����߳�
    LogManager::getSingleton().logDebug("����ҵ�߳̿���");
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
            LogManager::getSingleton().logDebug("ɾ�����е� " + 
                        boost::lexical_cast<std::string>(i + 1) + "���߳�");
            threads_.removeAt(i);
            break;
        }
    }
    mutex_->unlock();
}