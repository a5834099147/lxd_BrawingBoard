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
    void sendChatRequest(QString& account);
    void sendRequestChatRequest(QString& account, bool result);
    QString getPeerAddress();
    void sendOpenChatPort(QString userName, QString targAddress, 
        quint32 localPort, quint32 targPort);

signals:
    void closing(int sockfd);
    void login(const QString& userName);
    void chatRequest(const QString& account);
    void chatRequestResult(const QString& account, bool result);

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
    void requestChatResult(ComDataType* data);
    void requestUserList();

    void sandLogin_result(bool result);
    void sandRigister_result(bool reslut);
    void sandUserList(std::string account, std::string userName, 
        std::string userPinyin, bool on_line, bool isUpdate);
private:
    ComDataTypeFactory comDataFactory;
};

#endif // CONNECTSOCKET_H
