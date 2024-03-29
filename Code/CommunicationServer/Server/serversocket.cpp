﻿#include "serversocket.h"
#include "serverthread.h"
#include "LogManager.h"
#include "CustomEventType.h"

#include <boost/lexical_cast.hpp>
#include <QCoreApplication>
#include <QHostAddress>
#include <cassert>

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

quint32 ServerSocket::thrd_fd_ = 0;
quint32 ServerSocket::port_fd_ = 49152;

ServerSocket::ServerSocket(quint16 port, QObject *parent)
    : QTcpServer(parent), m_port(port)
{
    m_threads.empty();
    m_mutex = new QMutex();
    assert(m_mutex != NULL);
}

ServerSocket::~ServerSocket()
{
    m_mutex->lock();
    m_threads.clear();
    m_mutex->unlock();

    delete m_mutex;
    m_mutex = NULL;
}

bool ServerSocket::start()
{
    ///< 开启服务, 调用监听
    if (!listen(QHostAddress::Any, m_port))
    {
        LogManager::getSingleton().logAlert("Listening for alerts.");
        LogManager::getSingleton().logDebug("File result: " + errorString().toStdString() + ".");
        return false;
    }
    else 
    {
        LogManager::getSingleton().logDebug("Listening success.");
        return true;
    }
}

void ServerSocket::incomingConnection(int sockfd)
{
    ///< 客户端有SOCKET加入
    ServerThread* thrd = new ServerThread(++thrd_fd_, sockfd);
    ///< 绑定退出消息于该线程, 当线程退出时调用该方法
    connect(thrd, SIGNAL(exiting(quint32)), this, SLOT(thrdExiting(quint32)));
    connect(thrd, SIGNAL(exiting(quint32)), this, SLOT(logout(quint32)));
    connect(thrd, SIGNAL(login(const QString&)), this, SLOT(login(const QString&)));
    connect(thrd, SIGNAL(chatRequest(const QString&)),
        this, SLOT(chatRequest(const QString&)));
    connect(thrd, SIGNAL(chatRequestResult(const QString&, bool)),
        this, SLOT(chatRequestResult(const QString&, bool)));

    ///< 将线程加入到线程链表中
    m_mutex->lock();
    m_threads.push_back(thrd);
    m_mutex->unlock();

    ///< 开启线程
    LogManager::getSingleton().logDebug("Open a new thread.");
    thrd->start();
}

void ServerSocket::thrdExiting(quint32 thrdfd)
{
    m_mutex->lock();
    int count = m_threads.count();
    for (int i = 0; i < count; ++i)
    {
        if (m_threads.at(i)->getThrdfd() == thrdfd)
        {
            LogManager::getSingleton().logDebug("Delete thread, thread order is  " + 
                        boost::lexical_cast<std::string>(i + 1) + ".");
            m_threads.removeAt(i);
            break;
        }
    }
    m_mutex->unlock();
}

void ServerSocket::logout( quint32 thrdfd )
{
    m_mutex->lock();
    ServerThread* targetThred = NULL;
    QString targetUser;
    QHashIterator<QString, ServerThread*> iter(m_userHash);

    while (iter.hasNext())
    {
        if (iter.next().value()->getThrdfd() == thrdfd)
        {
            targetThred = iter.value();
            targetUser = iter.key();
            LogManager::getSingleton().logDebug("Found the out thread , account is: " +
                                                targetUser.toStdString());
            m_userHash.remove(iter.key());
        }
    }
    m_mutex->unlock();

    if (targetThred == NULL)
    {
        LogManager::getSingleton().logWarn("Found not Login but exit thread");
        return;
    }

    iter = m_userHash;
    while (iter.hasNext())
    {
        targetThred = iter.next().value();
        QCoreApplication::postEvent(targetThred, new ChangeTheList(targetUser, false));
    }
}

void ServerSocket::login( const QString& userName )
{
    ServerThread* thread = qobject_cast<ServerThread*>(sender());

    m_mutex->lock();
    ///< 没有找到该线程
    assert(-1 != m_threads.indexOf(thread));
    m_mutex->unlock(); 

    LogManager::getSingleton().logDebug("Main thread recive login message, account is : " + 
                                        userName.toStdString());
    m_mutex->lock();
    m_userHash.insert(userName, thread);

    ///< 向所有在线用户发送更改状态
    QHashIterator<QString, ServerThread*> iter(m_userHash);
    while (iter.hasNext())
    {
        ServerThread* targetThread = iter.next().value();
        QCoreApplication::postEvent(targetThread, new ChangeTheList(userName, true));
    }

    ///< 向新加入用户线程发送现有用户登陆状况
    iter.toFront();
    while (iter.hasNext())
    {
        const QString& onLineUser = iter.next().key();
        QCoreApplication::postEvent(thread, new ChangeTheList(onLineUser, true));
    }
    m_mutex->unlock();
}

void ServerSocket::chatRequest( const QString& account )
{

    ServerThread* thread = qobject_cast<ServerThread*>(sender());

    m_mutex->lock();
    ///< 没有找到该线程
    assert(-1 != m_threads.indexOf(thread));
    QString requestAccount = m_userHash.key(thread, "");
    m_mutex->unlock(); 

    if (requestAccount == "")
    {
        LogManager::getSingleton().logAlert("Main thread recived chat request, but not found request account.");
        assert(false);
    }

    LogManager::getSingleton().logDebug("Main thread recived chat request, request account is : " + 
        requestAccount.toStdString() + ".");
    m_mutex->lock();
    ServerThread* targetThread = m_userHash.value(account, NULL);
    m_mutex->unlock();

    if (NULL == targetThread)
    {
        LogManager::getSingleton().logAlert("Main thread recived chat request, but not found " 
                    + account.toStdString() + " socket thread.");
        assert(false);
    }

    QCoreApplication::postEvent(targetThread, new ChatRequest(requestAccount));
}

void ServerSocket::chatRequestResult( const QString& account, bool result )
{
    ServerThread* thread = qobject_cast<ServerThread*>(sender());

    m_mutex->lock();
    ///< 没有找到该线程
    assert(-1 != m_threads.indexOf(thread));
    QString requestAccount = m_userHash.key(thread, "");
    m_mutex->unlock(); 

    if (requestAccount == "")
    {
        LogManager::getSingleton().logAlert("Main thread recived chat request result, but not found request account.");
        assert(false);
    }

    LogManager::getSingleton().logDebug("Main thread recived chat request result, request account is : " + 
        requestAccount.toStdString() + ".");
    m_mutex->lock();
    ServerThread* targetThread = m_userHash.value(account, NULL);
    m_mutex->unlock();

    if (NULL == targetThread)
    {
        LogManager::getSingleton().logAlert("Main thread recived chat request result, but not found account " 
            + account.toStdString() + " socket thread." );
        assert(false);
    }

    QCoreApplication::postEvent(targetThread, new ChatRequestResult(requestAccount, result));

    if (result)
    {
        ///< 获得两个通信端口
        quint32 port1, port2;
        getPort(port1, port2);

        QString userIpFirst, userIpSecond;
        userIpFirst = thread->getPeerAddress();
        userIpSecond = targetThread->getPeerAddress();

        QCoreApplication::postEvent(targetThread, new OpenChatPort(userIpFirst, requestAccount, port1, port2));
        QCoreApplication::postEvent(thread, new OpenChatPort(userIpSecond, account, port2, port1));
    }
}

void ServerSocket::getPort( quint32& port1, quint32& port2 )
{
    port1 = ++port_fd_;
    port2 = ++port_fd_;
}
