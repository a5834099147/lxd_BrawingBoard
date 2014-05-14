#include <QtCore/QCoreApplication>
#include <QtCore/QTextCodec>
#include "serversocket.h"
#include "odb_user.h"

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));

	odb_user odb;

	ServerSocket server(7733);
	if (!server.start())
	{
		printf("start server failed !\n");
		return 0;
	}

	return a.exec();
}