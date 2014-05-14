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
	quint16 port_;
	QMutex *mutex_;
	QList<ServerThread*> threads_;

	static quint32 thrd_fd_;
};

#endif // SERVERSOCKET_H
