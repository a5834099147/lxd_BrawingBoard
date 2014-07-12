#include "connectsocket.h"
#include "ComDataType.h"
#include "LogManager.h"
#include "OdbManager.h"

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
	///< 给收消息绑定信号/槽
    connect(this, SIGNAL(readyRead()), this, SLOT(receiveData()));
	///< 给关闭连接绑定信号/槽
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
}

ConnectSocket::~ConnectSocket()
{

}

void ConnectSocket::receiveData()
{

    ///< 接受消息
    LogManager::getSingleton().logDebug("Server Ready to accept data.");

    ///< 设置流为只读
    QDataStream receive(this);
    ///< 设置数据流的版本, 客户端与服务端相同
    receive.setVersion(QDataStream::Qt_4_8);

    ///< 读取数据大小
    qint32 reciveSize = 0;
    receive >> reciveSize;

    LogManager::getSingleton().logDebug("Receive data, data size is : " +
        boost::lexical_cast<std::string>(reciveSize) + ".");

    ///< 读取消息类型
    MsgType msgType;
    receive >> (qint32&)msgType;

    ComDataType* com = comDataFactory.createComData(msgType);
	///< 读取实体信息
    com->setData(receive);

	///< 根据消息种类, 分配给不同的处理函数
    receiveDataProcessing(msgType, com);
}

void ConnectSocket::requestLandingData(ComDataType* data)
{
    ///< 将基类指针转换为登陆结果实体指针
    LandingDataType* landingData = dynamic_cast<LandingDataType*>(data);

    ///< 判断是否发生转换异常
    if (NULL == landingData)
    {
        LogManager::getSingleton().logAlert("Type conversion fail");
        assert(false);
    }

	///< 在数据库中查询登陆结果
	bool result = OdbManager::getSingleton().logon(landingData->getAccount().toStdString(),
		landingData->getPassword().toStdString());		
		
	///< 发送登陆结果给客户机
    sandLogin_result(landingData->getAccount(), result);

    if (result)
    {
		///< 发送登陆成功信号给主线程
        emit login(landingData->getAccount());
    }

	///< 删除该信息
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
        LogManager::getSingleton().logAlert("Type conversion fail");
        assert(false);
    }

	///< 向数据库中加入该条注册信息, 并返回结果
	bool isOk = OdbManager::getSingleton().Register(registerData->getAccount().toStdString(),
		registerData->getPassword().toStdString(),
		registerData->getUserName().toStdString(),
		registerData->getSpelling().toStdString());		
		
	///< 发送注册信息给客户机
    sandRigister_result(isOk);

	///< 删除该消息
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
        LogManager::getSingleton().logAlert("Type conversion fail");
        assert(false);
    }
    
	///< 设置登录结果及用户名
    landingResult->setResult(result);
    landingResult->setAccount(userAccount);

	///< 发送登陆结果
    sandData(com);

    if (result)
    {
        LogManager::getSingleton().logDebug("Customer IP address : " + 
                    peerAddress().toString().toStdString() + ", Port is : " + 
                    boost::lexical_cast<std::string>(peerPort()));

		///< 将用户列表发给该用户
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
        LogManager::getSingleton().logAlert("Type conversion fail");
        assert(false);
    }

	///< 设置注册结果信息
    registerResult->setResult(reslut);
	
	///< 发送数据
    sandData(com);
}

void ConnectSocket::requestUserList()
{
	std::vector<User> user_v = OdbManager::getSingleton().detrainment();

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
        LogManager::getSingleton().logAlert("Type conversion fail");
        assert(false);
    }

	///< 设置更新列表内容
    returnTheListData->setAccount(account.c_str());
    returnTheListData->setUserName(userName.c_str());
    returnTheListData->setSpelling(userPinyin.c_str());
    returnTheListData->setOnLine(on_line);
    returnTheListData->setIsUpdate(isUpdate);

    sandData(com);
}


void ConnectSocket::sendChatRequest( const QString& account )
{
    ///< 创建信息注册实体
    ComDataType* com = comDataFactory.createComData(MT_CHATREQUESTS_DATA);

    ///< 将基类指针转换为聊天请求指针
    ChatRequestDataType* chatRequestData = dynamic_cast<ChatRequestDataType*>(com);

    ///< 判断是否发生转换异常
    if (NULL == chatRequestData)
    {
        LogManager::getSingleton().logAlert("Type conversion fail");
        assert(false);
    }

	///< 设置对话请求内容
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
        LogManager::getSingleton().logAlert("Type conversion fail");
        assert(false);
    }

	///< 设置打开聊天端口内容
    openChatsPort->setUserName(userName);
    openChatsPort->setAddress(targAddress);
    openChatsPort->setLocalPort(localPort);
    openChatsPort->setTargetPort(targPort);

    sandData(com);
}


void ConnectSocket::sendRequestChatRequest( const QString& account, bool result )
{

    ///< 创建信息注册实体
    ComDataType* com = comDataFactory.createComData(MT_CHATREQUESTS_RESULT);

    ///< 将基类指针转换为聊天请求指针
    ChatRequestResultType* chatRequestResult = dynamic_cast<ChatRequestResultType*>(com);

    ///< 判断是否发生转换异常
    if (NULL == chatRequestResult)
    {
        LogManager::getSingleton().logAlert("Type conversion fail");
        assert(false);
    }

	///< 设置聊天请求结果消息内容
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

    LogManager::getSingleton().logDebug("Ready to send data, data size is " + 
    boost::lexical_cast<std::string>(block.size()) + " .");

    int sandSize = write(block);

    LogManager::getSingleton().logDebug("Sand data into the buffer, buffer size is : " + 
    boost::lexical_cast<std::string>(sandSize) + " .");

    waitForBytesWritten();

    LogManager::getSingleton().logDebug("Send data success.");

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
        LogManager::getSingleton().logAlert("Type conversion fail.");
        assert(false);
    }

    LogManager::getSingleton().logDebug("Receive chat request, give chatRequest func processing.");

	///< 向主线程发送聊天请求
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
        LogManager::getSingleton().logAlert("Type conversion fail.");
        assert(false);
    }

    LogManager::getSingleton().logDebug("Receive char request result, give main thread processing.");

	///< 向主线程发送聊天结果请求
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
            LogManager::getSingleton().logDebug("Message to requestLandingData Fun processing.");
            requestLandingData(data);
            break;
        }
    case MT_SENDMESSAGE_MESSAGE:
        {
            LogManager::getSingleton().logDebug("Message to requestSendMessage Fun processing.");
            requestSendMessage_Message(data);
            break;
        }
    case MT_SENDMESSAGE_DATA:
        {
            LogManager::getSingleton().logDebug("Message to requestSendData Fun processing.");
            requestSendMessage_Data(data);
            break;
        }
    case MT_REGISTER_DATA:
        {
            LogManager::getSingleton().logDebug("Message to requestRegister Fun processing.");
            requestRegister(data);
            break;
        }
    case MT_CHATREQUESTS_DATA:
        {
            LogManager::getSingleton().logDebug("Message to requestChatRequest Fun processing.");
            requestChatRequest(data);
            break;
        }
    case MT_CHATREQUESTS_RESULT:
        {
            LogManager::getSingleton().logDebug("Message to requestChatResult Fun processing.");
            requestChatResult(data);
            break;
        }
    default:
        {
            LogManager::getSingleton().logAlert("Message to Error.");
            assert(false);
        }
    }
}

void ConnectSocket::updataTheList( const QString& account, bool state )
{
    LogManager::getSingleton().logDebug("Updata account: " 
                + account.toStdString() + " state " + (state ? "on line.": "off line"));

	///< 更新用户列表
    sandUserList(account.toStdString(), "", "", state, true);
}


