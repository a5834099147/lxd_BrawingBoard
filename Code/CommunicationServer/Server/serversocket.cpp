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
    ///< ��������, ���ü���
    if (!listen(QHostAddress::Any, m_port))
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
    connect(thrd, SIGNAL(exiting(quint32)), this, SLOT(logout(quint32)));
    connect(thrd, SIGNAL(login(const QString&)), this, SLOT(login(const QString&)));

    ///< ���̼߳��뵽�߳�������
    m_mutex->lock();
    m_threads.push_back(thrd);
    m_mutex->unlock();

    ///< �����߳�
    LogManager::getSingleton().logDebug("����ҵ�߳̿���");
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
            LogManager::getSingleton().logDebug("ɾ�����е� " + 
                        boost::lexical_cast<std::string>(i + 1) + "���߳�");
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
            LogManager::getSingleton().logDebug("�ҵ��ǳ����û��߳�, �û���Ϊ: " +
                                                targetUser.toStdString());
            m_userHash.remove(iter.key());
        }
    }
    m_mutex->unlock();

    if (targetThred == NULL)
    {
        LogManager::getSingleton().logWarn("����û�е�½���û��˳�ϵͳ");
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
    ///< û���ҵ����߳�
    assert(-1 != m_threads.indexOf(thread));
    m_mutex->unlock(); 

    LogManager::getSingleton().logDebug("���̻߳�õ�½��Ϣ,�û���Ϊ: " + 
                                        userName.toStdString());
    m_mutex->lock();
    m_userHash.insert(userName, thread);

    ///< �����������û����͸���״̬
    QHashIterator<QString, ServerThread*> iter(m_userHash);
    while (iter.hasNext())
    {
        ServerThread* targetThread = iter.next().value();
        QCoreApplication::postEvent(targetThread, new ChangeTheList(userName, true));
    }

    ///< ���¼����û��̷߳��������û���½״��
    iter.toFront();
    while (iter.hasNext())
    {
        const QString& onLineUser = iter.next().key();
        QCoreApplication::postEvent(thread, new ChangeTheList(onLineUser, true));
    }
    m_mutex->unlock();
}
