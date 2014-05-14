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
		return thrd_fd_;
	}

protected:
	void run();

signals:
	void exiting(quint32 thrdfd);

private slots:
	void sockDisconnected();

private:
	bool disconnected_;
	quint32 thrd_fd_;
	int sock_fd;
	ConnectSocket* sock_;	
};

#endif // SERVERTHREAD_H
