#include "serversocket.h"
#include "serverthread.h"

#include <QHostAddress>
#include <cassert>

quint32 ServerSocket::thrd_fd_ = 0;

ServerSocket::ServerSocket(quint16 port, QObject *parent)
	: QTcpServer(parent), port_(port)
{
	threads_.empty();
	mutex_ = new QMutex();
	assert(mutex_ != NULL);
}

ServerSocket::~ServerSocket()
{
	mutex_->lock();
	threads_.clear();
	mutex_->unlock();

	delete mutex_;
	mutex_ = NULL;
}

bool ServerSocket::start()
{
	if (!listen(QHostAddress::Any, port_))
	{
		return false;
	}
	else 
	{
		return true;
	}
}

void ServerSocket::incomingConnection(int sockfd)
{
	ServerThread* thrd = new ServerThread(++thrd_fd_, sockfd);
	connect(thrd, SIGNAL(exiting(quint32)), this, SLOT(thrdExiting(quint32)));

	mutex_->lock();	
	threads_.push_back(thrd);
	mutex_->unlock();

	thrd->start();
}

void ServerSocket::thrdExiting(quint32 thrdfd)
{
	mutex_->lock();
	int count = threads_.count();
	for (int i = 0; i < count; ++i)
	{
		if (threads_.at(i)->getThrdfd() == thrdfd)
		{
			threads_.removeAt(i);
			break;
		}		
	}
	mutex_->unlock();
}