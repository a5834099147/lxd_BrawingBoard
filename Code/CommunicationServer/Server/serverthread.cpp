#include "serverthread.h"
#include "connectsocket.h"

ServerThread::ServerThread(quint32 thrdfd, int sockfd, QObject *parent)
	: QThread(parent), disconnected_(true), 
	  thrd_fd_(thrdfd), sock_fd(sock_fd)
{
	sock_ = new ConnectSocket();
	sock_->setSocketDescriptor(sockfd);
	disconnected_ = false;
	connect(sock_, SIGNAL(disconnected()), sock_, SLOT(deleteLater()));
	connect(sock_, SIGNAL(disconnected()), this, SLOT(sockDisconnected()));
	connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

ServerThread::~ServerThread()
{
}

void ServerThread::run()
{
	printf( "Thread %02d: thread start running !\n", thrd_fd_);  
	exec();
}

void ServerThread::sockDisconnected()
{
	printf( "Thread %02d: connect socket %d disconnected !\n", thrd_fd_, sock_fd);  
	emit exiting(thrd_fd_);
	exit();
}
