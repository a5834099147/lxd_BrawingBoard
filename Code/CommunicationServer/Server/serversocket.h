#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <QTcpServer>
#include <QList>
#include <QMutex>

class ServerThread;

class ServerSocket : public QTcpServer
{
    Q_OBJECT

public:
    ServerSocket(quint16 port, QObject *parent = NULL);
    ~ServerSocket();

    bool start();

protected:
    void incomingConnection(int sockfd);

private slots:
    void thrdExiting(quint32 thrdfd);

private:
    ///< 端口号
    quint16 port_;
    ///< 互斥锁
    QMutex *mutex_;
    ///< 线程链表
    QList<ServerThread*> threads_;

    ///< 线程编号
    static quint32 thrd_fd_;
};

#endif // SERVERSOCKET_H
