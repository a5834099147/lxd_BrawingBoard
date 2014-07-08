﻿#include <QTime>

#include "clientsocket.h"
#include "ComDataType.h"
#include "chat.h"

#include <boost/lexical_cast.hpp>
#include <cassert>
#include <QtNetwork/QHostAddress>
#include <string>

#include <QMessageBox>

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

ClientSocket::ClientSocket(QObject *parent)
    : QTcpSocket(parent)
{
    ///< 当有数据可以读取时, 调用读取槽
    connect(this, SIGNAL(readyRead()), this, SLOT(receiveData()));
}

ClientSocket::~ClientSocket()
{
}

bool ClientSocket::connectToServer(const char* ip, quint16 port)
{
    ///< 连接服务器
    QString strip(ip);
    QHostAddress addr;
    addr.setAddress(strip);
    connectToHost(addr, port);
    return waitForConnected();
}

void ClientSocket::sandMessage_Message(std::string meg)
{

}

void ClientSocket::landing_Data(std::string account, std::string password)
{
    ///< 创建信息发送实体
    ComDataType* com = m_comDataFactory.createComData(MT_LANDING_DATA);

    ///< 将基类指针转换为发送实体指针
    LandingDataType* landingData = dynamic_cast<LandingDataType*>(com);

    ///< 判断是否发生转换异常
    if (NULL == landingData)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    ///< 设置发送实体账号
    landingData->setAccount(account.c_str());
    ///< 设置发送实体密码
    landingData->setPassword(password.c_str());

    ///< 发送该实体
    sandData(com);
}

void ClientSocket::sandRegister(std::string account, std::string password, std::string userName, std::string pinYin)
{
    ///< 创建信息注册实体
    ComDataType* com = m_comDataFactory.createComData(MT_REGISTER_DATA);

    ///< 将基类指针转换为注册实体指针
    RegisterDataType* registerDate = dynamic_cast<RegisterDataType*>(com);

    ///< 判断是否发生转换异常
    if (NULL == registerDate)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    ///< 设置注册账号
    registerDate->setAccount(account.c_str());
    ///< 设置注册密码
    registerDate->setPassword(password.c_str());
    ///< 设置注册用户名
    registerDate->setUserName(userName.c_str());
    ///< 设置注册用户名拼音
    registerDate->setSpelling(pinYin.c_str());

    ///< 发送该实体
    sandData(com);
}

void ClientSocket::sandRequestChat( const QString& account )
{

    ///< 创建信息注册实体
    ComDataType* com = m_comDataFactory.createComData(MT_CHATREQUESTS_DATA);

    ///< 将基类指针转换为聊天请求实体指针
    ChatRequestDataType* chatRequestDate = dynamic_cast<ChatRequestDataType*>(com);

    ///< 判断是否发生转换异常
    if (NULL == chatRequestDate)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    if (account == m_selfAccount)
    {
        LogManager::getSingleton().logWarn("与自己建立连接请求被回绝");
        QMessageBox::warning(NULL, "警告", "您试图与自己建立通信, 这是非法的!",
                             QMessageBox::Ok, QMessageBox::Ok);
        delete com;
        return;
    }

    if (-1 != m_talkAccountList.indexOf(account))
    {
        LogManager::getSingleton().logWarn("与正在通话的连接发起对话, 被回绝");
        QMessageBox::warning(NULL, "警告", "您已经与用户: " +
                             account + "建立了连接, 这是非法的!", 
                             QMessageBox::Ok, QMessageBox::Ok);
        delete com;
        return;
    }

    if (-1 != m_askForAccountList.indexOf(account))
    {
        LogManager::getSingleton().logWarn("与已经提交申请但是没有回复的用户发起通话, 被系统回绝");
        QMessageBox::warning(NULL, "警告", "您已经向用户: " +
                             account + "提出了申请, 对方还没有给您回复, 请等待",
                             QMessageBox::Ok, QMessageBox::Ok);
        delete com;
        return;
    }

    ///< 设置注册账号
    chatRequestDate->setAccount(account);

    ///< 在请求列表中添加该用户
    m_askForAccountList.append(account);

    ///< 发送该实体
    sandData(com);
}


void ClientSocket::sandRequestChatResult( std::string account, bool result )
{

    ///< 创建信息注册实体
    ComDataType* com = m_comDataFactory.createComData(MT_CHATREQUESTS_RESULT);

    ///< 将基类指针转换为聊天请求结果实体指针
    ChatRequestResultType* chatRequestResult = dynamic_cast<ChatRequestResultType*>(com);

    ///< 判断是否发生转换异常
    if (NULL == chatRequestResult)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    ///< 设置注册账号
    chatRequestResult->setAccount(account.c_str());
    chatRequestResult->setResult(result);

    ///< 发送该实体
    sandData(com);
}

void ClientSocket::sandData(ComDataType* data)
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

void ClientSocket::receiveData()
{
    ///< 接受消息
    LogManager::getSingleton().logDebug("准备接收数据");

    ///< 设置流为只读
    QDataStream receive(this);
    ///< 设置数据流的版本, 客户端与服务端相同
    receive.setVersion(QDataStream::Qt_4_8);

    while (!receive.atEnd())
    {
        ///< 读取数据大小
        qint32 reciveSize = 0;
        receive >> reciveSize;

        LogManager::getSingleton().logDebug("收到服务器发来信息, 封装时数据大小为: " +
            boost::lexical_cast<std::string>(reciveSize));

        ///< 读取消息类型
        MsgType msgType;
        receive >> (qint32&)msgType;

        ComDataType* com = m_comDataFactory.createComData(msgType);
        com->setData(receive);

        switch(msgType)
        {
        case MT_LANDING_RESULT:
            {
                LogManager::getSingleton().logDebug("交由登陆结果函数处理结果信息");
                requestLanding_Result(com);
                break;
            }
        case MT_GETMESSAGE_DATA:
            {
                LogManager::getSingleton().logDebug("交由获取消息数据类函数处理数据信息");
                requestGetMessage_Message(com);
                break;
            }
        case MT_GETMESSAGE_MESSAGE:
            {
                LogManager::getSingleton().logDebug("交由获取消息消息类函数处理消息信息");
                requestGetMessage_Data(com);
                break;
            }
        case MT_REGISTER_RESULT:
            {
                LogManager::getSingleton().logDebug("交由注册结果处理函数处理注册结果");
                requestRegister_Result(com);
                break;
            }
        case MT_RETURNTHELIST_DATA:
            {
                LogManager::getSingleton().logDebug("交由数据表处理函数处理数据表信息");
                requestUserList(com);
                break; 
            }
        case MT_CHATREQUESTS_DATA:
            {
                LogManager::getSingleton().logDebug("交由聊天请求处理函数处理请求信息");
                requestChat(com);
                break;
            }
        case MT_CHATREQUESTS_RESULT:
            {
                LogManager::getSingleton().logDebug("交由聊天请求结果处理函数处理该回执信息");
                requestChatResult(com);
                break;
            }
        case MT_OPENCHATSPORT:
            {
                LogManager::getSingleton().logDebug("交由开始聊天处理函数处理该请求消息");
                requestOpenChat(com);
                break;
            }
        default:
            {
                LogManager::getSingleton().logError("收到非法消息类型");
                assert(false);
            }
        }
    }
}

void ClientSocket::requestLanding_Result(ComDataType* data)
{
    ///< 将基类指针转换为登陆结果实体指针
    LandingResultType* landingResult = dynamic_cast<LandingResultType*>(data);

    ///< 判断是否发生转换异常
    if (NULL == landingResult)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    LogManager::getSingleton().logDebug("发送登陆处理结果消息");
    emit recive_login(landingResult->getResult());

    if (landingResult->getResult()) 
    {
        m_selfAccount = landingResult->getAccount();
    }

    delete data;
    data = NULL;
}

void ClientSocket::requestRegister_Result(ComDataType* data)
{
    ///< 将基类指针转换为注册结果实体指针
    RegisterResultType* registerResult = dynamic_cast<RegisterResultType*>(data);

    ///< 判断是否发生转换异常
    if (NULL == registerResult)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    LogManager::getSingleton().logDebug("发送注册处理结果消息");

    emit recive_register(registerResult->getResult());

    delete data;
    data = NULL;
}

void ClientSocket::requestGetMessage_Data(ComDataType* data)
{

}

void ClientSocket::requestGetMessage_Message(ComDataType* data)
{

}

void ClientSocket::requestUserList(ComDataType* data)
{
    ///< 将基类指针转换为注册结果实体指针
    ReturnTheListDataType* returnTheListData = dynamic_cast<ReturnTheListDataType*>(data);

    ///< 判断是否发生转换异常
    if (NULL == returnTheListData)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    LogManager::getSingleton().logDebug("接受到用户列表信息");
    emit recive_userList(returnTheListData->getAccount().toStdString(),
                         returnTheListData->getUserName().toStdString(),
                         returnTheListData->getSpelling().toStdString(),
                         returnTheListData->getOnLine(),
                         returnTheListData->getIsUpdate());

    delete data;
    data = NULL;
}

void ClientSocket::requestChat( ComDataType* data )
{

    ///< 将基类指针转换为聊天请求实体指针
    ChatRequestDataType* chatRequestData = dynamic_cast<ChatRequestDataType*>(data);

    ///< 判断是否发生转换异常
    if (NULL == chatRequestData)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    LogManager::getSingleton().logDebug("接受到聊天请求信息, 来自" + 
                chatRequestData->getAccount().toStdString());

    if (QMessageBox::question(NULL, "对话请求", "用户: " + 
        chatRequestData->getAccount() + " 期望与您交谈, 您是否允许?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
    {
        sandRequestChatResult(chatRequestData->getAccount().toStdString(), false);
    }
    else 
    {
        sandRequestChatResult(chatRequestData->getAccount().toStdString(), true);
    }

    delete data;
    data = NULL;
}

void ClientSocket::requestChatResult( ComDataType* data )
{

    ///< 将基类指针转换为注册结果实体指针
    ChatRequestResultType* chatRequestResult = dynamic_cast<ChatRequestResultType*>(data);

    ///< 判断是否发生转换异常
    if (NULL == chatRequestResult)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    LogManager::getSingleton().logDebug("接收到来自: " + 
                chatRequestResult->getAccount().toStdString() + "的聊天请求结果");

    if (chatRequestResult->getResult())
    {
        QMessageBox::information(NULL, "对话请求回执", "系统正在为您建立与用户 " +
            chatRequestResult->getAccount() + "之间的链接, 请稍候...",
            QMessageBox::Yes, QMessageBox::Yes);
    }
    else 
    {
        QMessageBox::information(NULL, "对话请求回执", "很抱歉用户  " +
            chatRequestResult->getAccount() + "拒绝了您的交谈请求",
            QMessageBox::Yes, QMessageBox::Yes);
    }

    int index = m_askForAccountList.indexOf(chatRequestResult->getAccount());
    if (-1 == index)
    {
        LogManager::getSingleton().logError("在请求列表中无法找到用户" + 
                    chatRequestResult->getAccount().toStdString());
        assert(false);
    }
    else 
    {
        m_askForAccountList.remove(index);
        LogManager::getSingleton().logDebug("在请求列表中删除用户" + 
                    chatRequestResult->getAccount().toStdString());
    }

    delete data;
    data = NULL;

}

void ClientSocket::requestOpenChat( ComDataType* data )
{

    ///< 将基类指针转换为打开聊天实体指针
    OpenChatsPort* openChatPort = dynamic_cast<OpenChatsPort*>(data);

    ///< 判断是否发生转换异常
    if (NULL == openChatPort)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    LogManager::getSingleton().logDebug("接受到打开聊天对话框, 来自" + 
        openChatPort->getUserName().toStdString());

    Chat* chat_ = new Chat(openChatPort->getUserName(), openChatPort->getAddress(), 
                          openChatPort->getLocalPort(), openChatPort->getTargetPort());
    chat_->show();

    m_talkAccountList.append(openChatPort->getUserName());
    ///< 将对话对话框加入到Vector中
    m_chatVector.append(chat_);
    connect(chat_, SIGNAL(exitTheChat()), this, SLOT(ChatFinished()));

    delete data;
    data = NULL;
}

void ClientSocket::ChatFinished()
{
    LogManager::getSingleton().logDebug("有一个交谈对话框结束了交谈, 正在析构");
    Chat* chat = qobject_cast<Chat*>(sender());
    ///< 在聊天对话框集合中找到要关闭的实体
    assert(-1 != m_chatVector.indexOf(chat));
    LogManager::getSingleton().logDebug("在聊天集合中找到要析构的Chat实体指针");
    ///< 在聊天用户列表中找到需要关闭实体的用户名
    assert(-1 != m_talkAccountList.indexOf(chat->getUserAccount()));
    LogManager::getSingleton().logDebug("在聊天姓名集合中找到要析构的Chat实体指针");

    ///< 删除
    m_talkAccountList.remove(m_talkAccountList.indexOf(chat->getUserAccount()));
    LogManager::getSingleton().logDebug("在聊天姓名集合中删除了该姓名信息");
    m_chatVector.remove(m_chatVector.indexOf(chat));
    LogManager::getSingleton().logDebug("在聊天集合中删除了该Chat实体指针");

    delete chat;
    LogManager::getSingleton().logDebug("delete了要删除的Chat实体指针");
    chat = NULL;
}



