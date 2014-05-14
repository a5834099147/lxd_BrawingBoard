#ifndef CONNECTSOCKET_H
#define CONNECTSOCKET_H
#include "common.h"
#include "ComDataTypeFactory.h"

#include <QtNetwork/QTcpSocket>

class ComDataType;

class ConnectSocket : public QTcpSocket
{
    Q_OBJECT

public:
    ConnectSocket(QObject *parent = NULL);
    ~ConnectSocket();

signals:
    void closing(int sockfd);

protected:
    void requestLandingData(ComDataType* data);
    void requestSendMessage_Message(ComDataType* data);	
    void requestSendMessage_Data(ComDataType* data);
    void requestRegister(ComDataType* data);

    void requestUserList();

    void sandLogin_result(bool result);
    void sandRigister_result(bool reslut);
    void sandUserList(std::string account, std::string userName, std::string userPinyin, char on_line, char replace);

    private slots:
        void receiveData();

private:
    void sandData(ComDataType* data);
    ComDataTypeFactory comDataFactory;
};

#endif // CONNECTSOCKET_H
