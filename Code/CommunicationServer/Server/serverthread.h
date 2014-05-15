#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>

class ConnectSocket; 

class ServerThread : public QThread
{
    Q_OBJECT

public:
    ServerThread(quint32 thrdfd, int sockfd, QObject *parent = NULL);
    ~ServerThread();

    quint32 getThrdfd()
    const
    {
        return m_thrd_fd;
    }

protected:
    void run();
    void customEvent(QEvent *event);

signals:
    void exiting(quint32 thrdfd);
    void login(const QString& userName);

private slots:
    void sockDisconnected();

private:
    bool m_disconnected;
    quint32 m_thrd_fd;
    int m_sock_fd;
    ConnectSocket* m_sock;
};

#endif // SERVERTHREAD_H
