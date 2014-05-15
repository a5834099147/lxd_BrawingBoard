#include "serversocket.h"
#include "serverthread.h"
#include "LogManager.h"
#include "CustomEventType.h"

#include <boost/lexical_cast.hpp>
#include <QCoreApplication>
#include <QHostAddress>
#include <cassert>

quint32 ServerSocket::thrd_fd_ = 0;

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
    connect(thrd, SIGNAL(exiting(quint32)), this, SLOT(logout(quint32)));
    connect(thrd, SIGNAL(login(const QString&)), this, SLOT(login(const QString&)));

    ///< 将线程加入到线程链表中
    m_mutex->lock();
    m_threads.push_back(thrd);
    m_mutex->unlock();

    ///< 开启线程
    LogManager::getSingleton().logDebug("新作业线程开启");
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
            LogManager::getSingleton().logDebug("删除现有第 " + 
                        boost::lexical_cast<std::string>(i + 1) + "个线程");
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
            LogManager::getSingleton().logDebug("找到登出的用户线程, 用户名为: " +
                                                targetUser.toStdString());
            m_userHash.remove(iter.key());
        }
    }
    m_mutex->unlock();

    if (targetThred == NULL)
    {
        LogManager::getSingleton().logWarn("出现没有登陆的用户退出系统");
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

    LogManager::getSingleton().logDebug("主线程获得登陆信息,用户名为: " + 
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
