#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QtNetwork/QTcpSocket>

#include "ComDataTypeFactory.h"
#include "LogManager.h"

class ComDataType;

class ClientSocket : public QTcpSocket
{
    Q_OBJECT

public:
    ClientSocket(QObject *parent = NULL);
    ~ClientSocket();

signals:
    void recive_login(bool);
    void recive_register(bool);
    void recive_userList(std::string account, std::string userName, std::string userPinyin, bool on_line);

public:
    bool connectToServer(const char* ip, quint16 port);
    void sandMessage_Message(std::string meg);
    void sandRegister(std::string account, std::string password, std::string userName, std::string pinYin);
    void landing_Data(std::string accoutn, std::string password);

private:
    void sandData(ComDataType* data);
    void requestLanding_Result(ComDataType* data);
    void requestGetMessage_Message(ComDataType* data);
    void requestGetMessage_Data(ComDataType* data);
    void requestRegister_Result(ComDataType* data);
    void requestUserList(ComDataType* data);

private slots:
    void receiveData();

private:
    ComDataTypeFactory comDataFactory;
};

#endif // CLIENTSOCKET_H