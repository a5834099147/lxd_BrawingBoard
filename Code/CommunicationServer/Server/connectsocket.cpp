﻿#include "connectsocket.h"
#include "ComDataType.h"
#include "odb_user.h"
#include "LogManager.h"

#include <boost/lexical_cast.hpp>
#include <cassert>
#include <QDataStream>
#include <string>
#include <QHostAddress>

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

ConnectSocket::ConnectSocket(QObject *parent)
    : QTcpSocket(parent)
{
    connect(this, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

ConnectSocket::~ConnectSocket()
{

}

void ConnectSocket::receiveData()
{

    ///< 接受消息
    LogManager::getSingleton().logDebug("服务器准备接收数据");

    ///< 设置流为只读
    QDataStream receive(this);
    ///< 设置数据流的版本, 客户端与服务端相同
    receive.setVersion(QDataStream::Qt_4_8);

    ///< 读取数据大小
    qint32 reciveSize = 0;
    receive >> reciveSize;

    LogManager::getSingleton().logDebug("收到服务器发来信息, 封装时数据大小为: " +
        boost::lexical_cast<std::string>(reciveSize));

    ///< 读取消息类型
    MsgType msgType;
    receive >> (qint32&)msgType;

    ComDataType* com = comDataFactory.createComData(msgType);
    com->setData(receive);

    receiveDataProcessing(msgType, com);
}

void ConnectSocket::requestLandingData(ComDataType* data)
{
    ///< 将基类指针转换为登陆结果实体指针
    LandingDataType* landingData = dynamic_cast<LandingDataType*>(data);

    ///< 判断是否发生转换异常
    if (NULL == landingData)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    bool result = odb_user::logon(landingData->getAccount().toStdString(), 
                                  landingData->getPassword().toStdString());	
    sandLogin_result(landingData->getAccount(), result);

    if (result)
    {
        emit login(landingData->getAccount());
    }

    delete data;
    data = NULL;
}

void ConnectSocket::requestSendMessage_Message(ComDataType* /*data*/)
{

}

void ConnectSocket::requestSendMessage_Data(ComDataType* /*data*/)
{

}

void ConnectSocket::requestRegister(ComDataType* data)
{
    ///< 将基类指针转换为登陆结果实体指针
    RegisterDataType* registerData = dynamic_cast<RegisterDataType*>(data);

    ///< 判断是否发生转换异常
    if (NULL == registerData)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    bool isOk = odb_user::registerTheUser(registerData->getAccount().toStdString(), 
                              registerData->getPassword().toStdString(),
                              registerData->getUserName().toStdString(),
                              registerData->getSpelling().toStdString());
    sandRigister_result(isOk);

    delete data;
    data = NULL;
}

void ConnectSocket::sandLogin_result(const QString& userAccount, bool result)
{

    ///< 创建信息注册实体
    ComDataType* com = comDataFactory.createComData(MT_LANDING_RESULT);

    ///< 将基类指针转换为注册实体指针
    LandingResultType* landingResult = dynamic_cast<LandingResultType*>(com);

    ///< 判断是否发生转换异常
    if (NULL == landingResult)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }
    
    landingResult->setResult(result);
    landingResult->setAccount(userAccount);

    sandData(com);

    if (result)
    {
        LogManager::getSingleton().logDebug("登陆机地址为: " + 
                    peerAddress().toString().toStdString() + "端口为: " + 
                    boost::lexical_cast<std::string>(peerPort()));
        LogManager::getSingleton().logDebug("服务器地址为: " + 
            localAddress().toString().toStdString() + "端口为: " + 
            boost::lexical_cast<std::string>(localPort()));
        requestUserList();
    }
}

QString ConnectSocket::getPeerAddress()
{
    return peerAddress().toString();
}

void ConnectSocket::sandRigister_result(bool reslut)
{

    ///< 创建信息注册实体
    ComDataType* com = comDataFactory.createComData(MT_REGISTER_RESULT);

    ///< 将基类指针转换为注册实体指针
    RegisterResultType* registerResult = dynamic_cast<RegisterResultType*>(com);

    ///< 判断是否发生转换异常
    if (NULL == registerResult)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    registerResult->setResult(reslut);

    sandData(com);
}

void ConnectSocket::requestUserList()
{
    std::vector<User> user_v = odb_user::detrainment();
    for (std::vector<User>::iterator it = user_v.begin(); it != user_v.end(); ++it)
    {
        sandUserList(it->Account(), it->UserName(), it->UserNamePinyin(), false, false);
    }
}

void ConnectSocket::sandUserList(std::string account, std::string userName, 
                                 std::string userPinyin, bool on_line, bool isUpdate)
{

    ///< 创建信息注册实体
    ComDataType* com = comDataFactory.createComData(MT_RETURNTHELIST_DATA);

    ///< 将基类指针转换为注册实体指针
    ReturnTheListDataType* returnTheListData = dynamic_cast<ReturnTheListDataType*>(com);

    ///< 判断是否发生转换异常
    if (NULL == returnTheListData)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    returnTheListData->setAccount(account.c_str());
    returnTheListData->setUserName(userName.c_str());
    returnTheListData->setSpelling(userPinyin.c_str());
    returnTheListData->setOnLine(on_line);
    returnTheListData->setIsUpdate(isUpdate);

    sandData(com);
}


void ConnectSocket::sendChatRequest( QString& account )
{
    ///< 创建信息注册实体
    ComDataType* com = comDataFactory.createComData(MT_CHATREQUESTS_DATA);

    ///< 将基类指针转换为聊天请求指针
    ChatRequestDataType* chatRequestData = dynamic_cast<ChatRequestDataType*>(com);

    ///< 判断是否发生转换异常
    if (NULL == chatRequestData)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    chatRequestData->setAccount(account);

    sandData(com);
}

void ConnectSocket::sendOpenChatPort( QString userName, QString targAddress,
    quint32 localPort, quint32 targPort )
{

    ///< 创建信息注册实体
    ComDataType* com = comDataFactory.createComData(MT_OPENCHATSPORT);

    ///< 将基类指针转换为开始聊天指针
    OpenChatsPort* openChatsPort = dynamic_cast<OpenChatsPort*>(com);

    ///< 判断是否发生转换异常
    if (NULL == openChatsPort)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    openChatsPort->setUserName(userName);
    openChatsPort->setAddress(targAddress);
    openChatsPort->setLocalPort(localPort);
    openChatsPort->setTargetPort(targPort);

    sandData(com);
}


void ConnectSocket::sendRequestChatRequest( QString& account, bool result )
{

    ///< 创建信息注册实体
    ComDataType* com = comDataFactory.createComData(MT_CHATREQUESTS_RESULT);

    ///< 将基类指针转换为聊天请求指针
    ChatRequestResultType* chatRequestResult = dynamic_cast<ChatRequestResultType*>(com);

    ///< 判断是否发生转换异常
    if (NULL == chatRequestResult)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    chatRequestResult->setAccount(account);
    chatRequestResult->setResult(result);

    sandData(com);
}

void ConnectSocket::sandData(ComDataType* data)
{
    ///< 需要发送的字节序
    QByteArray block;
    ///< 设置流为只写
    QDataStream sand(&block, QIODevice::WriteOnly);
    ///< 设置数据流的版本, 客户端与服务端相同
    sand.setVersion(QDataStream::Qt_4_8);

    ///< 设置发送长度初始值为0
    sand << (qint32) 0;
    ///< 设置发送的内容
    data->getData(sand);
    ///< 回到字节流起始位置
    sand.device()->seek(0);
    ///< 重置字节流长度
    sand << (qint32)(block.size() - sizeof(qint32));

    LogManager::getSingleton().logDebug("准备发送数据: 数据大小为: " + 
    boost::lexical_cast<std::string>(block.size()));

    int sandSize = write(block);

    LogManager::getSingleton().logDebug("发送数据进入缓冲区: 数据大小为: " + 
    boost::lexical_cast<std::string>(sandSize));

    waitForBytesWritten();

    LogManager::getSingleton().logDebug("发送数据成功");

    ///< 删除消息
    delete data;
    data = NULL;
}

void ConnectSocket::requestChatRequest( ComDataType* data )
{

    ///< 将基类指针转换为聊天请求实体指针
    ChatRequestDataType* charRequestData = dynamic_cast<ChatRequestDataType*>(data);

    ///< 判断是否发生转换异常
    if (NULL == charRequestData)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    LogManager::getSingleton().logDebug("获得交谈请求并转发给主线程处理");
    emit chatRequest(charRequestData->getAccount());

    delete data;
    data = NULL;
}

void ConnectSocket::requestChatResult( ComDataType* data )
{
    ///< 将基类指针转换为聊天请求结果实体指针
    ChatRequestResultType* charRequest = dynamic_cast<ChatRequestResultType*>(data);

    ///< 判断是否发生转换异常
    if (NULL == charRequest)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    LogManager::getSingleton().logDebug("获得交谈请求结果并转发给主线程处理");
    emit chatRequestResult(charRequest->getAccount(), charRequest->getResult());

    delete data;
    data = NULL;
}


void ConnectSocket::receiveDataProcessing( MsgType type, ComDataType* data )
{
    switch(type)
    {
    case MT_LANDING_DATA:
        {
            LogManager::getSingleton().logDebug("将收到的信息交由登陆处理函数处理");
            requestLandingData(data);
            break;
        }
    case MT_SENDMESSAGE_MESSAGE:
        {
            LogManager::getSingleton().logDebug("将收到的消息交由接受消息函数处理");
            requestSendMessage_Message(data);
            break;
        }
    case MT_SENDMESSAGE_DATA:
        {
            LogManager::getSingleton().logDebug("将收到的消息交由接受数据函数处理");
            requestSendMessage_Data(data);
            break;
        }
    case MT_REGISTER_DATA:
        {
            LogManager::getSingleton().logDebug("将收到的消息交由注册处理函数处理");
            requestRegister(data);
            break;
        }
    case MT_CHATREQUESTS_DATA:
        {
            LogManager::getSingleton().logDebug("将收到的消息交由聊天请求处理函数处理");
            requestChatRequest(data);
            break;
        }
    case MT_CHATREQUESTS_RESULT:
        {
            LogManager::getSingleton().logDebug("将收到的消息交由聊天结果请求函数处理");
            requestChatResult(data);
            break;
        }
    default:
        {
            LogManager::getSingleton().logDebug("收到非法消息类型");
            assert(false);
        }
    }
}

void ConnectSocket::updataTheList( QString& account, bool state )
{
    LogManager::getSingleton().logDebug("发送更新用户名:" 
                + account.toStdString() + "的状态为" + (state ? "在线": "不在线"));
    sandUserList(account.toStdString(), "", "", state, true);
}


