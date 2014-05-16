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
    void logout(quint32 thrdfd);
    void login(const QString& userName);
    void chatRequest(const QString& account);
    void chatRequestResult(const QString& account, bool result);

private:
    ///< �˿ں�
    quint16 m_port;
    ///< ������
    QMutex *m_mutex;
    ///< �߳�����
    QList<ServerThread*> m_threads;
    ///< �߳����û�����Ӧ��ϣ��
    QHash<QString, ServerThread*> m_userHash;
    ///< �̱߳��
    static quint32 thrd_fd_;
};

#endif // SERVERSOCKET_H
