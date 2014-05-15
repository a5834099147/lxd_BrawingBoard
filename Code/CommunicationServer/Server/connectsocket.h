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

public:
    void updataTheList(QString& account, bool state);

signals:
    void closing(int sockfd);
    void login(const QString& userName);

private slots:
    void receiveData();

private:
    void sandData(ComDataType* data);

    void receiveDataProcessing(MsgType type, ComDataType* data);

    void requestLandingData(ComDataType* data);
    void requestSendMessage_Message(ComDataType* data);
    void requestSendMessage_Data(ComDataType* data);
    void requestRegister(ComDataType* data);
    void requestChatRequest(ComDataType* data);
    void requestUserList();

    void sandLogin_result(bool result);
    void sandRigister_result(bool reslut);
    void sandUserList(std::string account, std::string userName, 
        std::string userPinyin, bool on_line, bool isUpdate);

private:
    ComDataTypeFactory comDataFactory;
};

#endif // CONNECTSOCKET_H
