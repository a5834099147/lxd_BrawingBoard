#include "serverthread.h"
#include "connectsocket.h"
#include "CustomEventType.h"
#include "common.h"
#include "LogManager.h"

#include <boost/lexical_cast.hpp>
#include <QEvent>

ServerThread::ServerThread(quint32 thrdfd, int sockfd, QObject *parent)
    : QThread(parent),
    m_disconnected(true),
    m_thrd_fd(thrdfd),
    m_sock_fd(m_sock_fd)
{
    m_sock = new ConnectSocket();
    m_sock->setSocketDescriptor(sockfd);
    m_disconnected = false;
    connect(m_sock, SIGNAL(disconnected()), m_sock, SLOT(deleteLater()));
    connect(m_sock, SIGNAL(disconnected()), this, SLOT(sockDisconnected()));
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    connect(m_sock, SIGNAL(login(const QString&)), this, SIGNAL(login(const QString&)));
    connect(m_sock, SIGNAL(chatRequest(const QString&)),
            this, SIGNAL(chatRequest(const QString&)));
    connect(m_sock, SIGNAL(chatRequestResult(const QString&, bool)),
            this, SIGNAL(chatRequestResult(const QString&, bool)));
}

ServerThread::~ServerThread()
{
}

void ServerThread::run()
{
    LogManager::getSingleton().logDebug("线程: " + 
                boost::lexical_cast<std::string>(m_thrd_fd) + "开始运行");
    exec();
}

void ServerThread::sockDisconnected()
{
    LogManager::getSingleton().logDebug("线程: " + 
                boost::lexical_cast<std::string>(m_thrd_fd) + 
                "停止运行, 它的套接字号码为: " + 
                boost::lexical_cast<std::string>(m_sock_fd));
    emit exiting(m_thrd_fd);
    exit();
}

void ServerThread::customEvent( QEvent *event )
{
    if (event->type() == (QEvent::User + ET_CHANGETHELIST))
    {
        LogManager::getSingleton().logDebug("线程: " + 
                  boost::lexical_cast<std::string>(m_thrd_fd) + "收到列表更改请求");
        ChangeTheList* tarEvent = static_cast<ChangeTheList*>(event);
        m_sock->updataTheList(tarEvent->getUserName(), tarEvent->getOnLine());
    }
    else if (event->type() == (QEvent::User + ET_CHATREQUEST))
    {
        LogManager::getSingleton().logDebug("线程: " + 
                  boost::lexical_cast<std::string>(m_thrd_fd) + "收到聊天请求");
        ChatRequest* tarEvent = static_cast<ChatRequest*>(event);
        m_sock->sendChatRequest(tarEvent->getUserName());
    }
    else if (event->type() == (QEvent::User + ET_CHATREQUESTRESULT))
    {
        LogManager::getSingleton().logDebug("线程: " +
                  boost::lexical_cast<std::string>(m_sock_fd) + "收到聊天请求结果");
        ChatRequestResult* tarEvent = static_cast<ChatRequestResult*>(event);
        m_sock->sendRequestChatRequest(tarEvent->getUserName(), tarEvent->getResult());
    }

}
