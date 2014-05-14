#include "connectsocket.h"
#include "ComDataType.h"
#include "odb_user.h"

#include <boost/lexical_cast.hpp>
#include <QDataStream>
#include <string>

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
    //LogManager::getSingleton().logDebug("准备接收数据");

    ///< 设置流为只读
    QDataStream receive(this);
    ///< 设置数据流的版本, 客户端与服务端相同
    receive.setVersion(QDataStream::Qt_4_8);

    ///< 读取数据大小
    qint32 reciveSize = 0;
    receive >> reciveSize;

    /*LogManager::getSingleton().logDebug("收到服务器发来信息, 封装时数据大小为: " +
        boost::lexical_cast<std::string>(reciveSize));*/

    ///< 读取消息类型
    MsgType msgType;
    receive >> (qint32&)msgType;

    ComDataType* com = comDataFactory.createComData(msgType);
    com->setData(receive);

    switch(msgType)
    {
    case MT_LANDING_DATA:
        requestLandingData(com);
        break;		
    case MT_SENDMESSAGE_MESSAGE:
        requestSendMessage_Message(com);
        break;	
    case MT_SENDMESSAGE_DATA:
        requestSendMessage_Data(com);
        break;
    case MT_REGISTER_DATA:
        requestRegister(com);
        break;
    }
}

void ConnectSocket::requestLandingData(ComDataType* data)
{
    ///< 将基类指针转换为登陆结果实体指针
    LandingDataType* landingData = dynamic_cast<LandingDataType*>(data);

    ///< 判断是否发生转换异常
    if (NULL == landingData)
    {
        //LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    bool result = odb_user::logon(landingData->getAccount().toStdString(), 
                                  landingData->getPassword().toStdString());	
    sandLogin_result(result);
}

void ConnectSocket::requestSendMessage_Message(ComDataType* data)
{

}

void ConnectSocket::requestSendMessage_Data(ComDataType* data)
{

}

void ConnectSocket::requestRegister(ComDataType* data)
{
    ///< 将基类指针转换为登陆结果实体指针
    RegisterDataType* registerData = dynamic_cast<RegisterDataType*>(data);

    ///< 判断是否发生转换异常
    if (NULL == registerData)
    {
        //LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    odb_user::registerTheUser(registerData->getAccount().toStdString(), 
                              registerData->getPassword().toStdString(),
                              registerData->getUserName().toStdString(),
                              registerData->getSpelling().toStdString());
    sandRigister_result(true);
}

void ConnectSocket::sandLogin_result(bool result)
{

    ///< 创建信息注册实体
    ComDataType* com = comDataFactory.createComData(MT_LANDING_RESULT);

    ///< 将基类指针转换为注册实体指针
    LandingResultType* landingResult = dynamic_cast<LandingResultType*>(com);

    ///< 判断是否发生转换异常
    if (NULL == landingResult)
    {
        //LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }
    
    landingResult->setResult(result);

    sandData(com);

    if (result)
    {
        requestUserList();
    }
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
        //LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    registerResult->setResult(reslut);

    sandData(com);
}

void ConnectSocket::requestUserList()
{
    std::vector<User> user_v = odb_user::export();
    for (std::vector<User>::iterator it = user_v.begin(); it != user_v.end(); ++it)
    {
        sandUserList(it->Account(), it->UserName(), it->UserNamePinyin(), 1, 0);
    }
}

void ConnectSocket::sandUserList(std::string account, std::string userName, std::string userPinyin, char on_line, char replace)
{

    ///< 创建信息注册实体
    ComDataType* com = comDataFactory.createComData(MT_RETURNTHELIST_DATA);

    ///< 将基类指针转换为注册实体指针
    ReturnTheListDataType* returnTheListData = dynamic_cast<ReturnTheListDataType*>(com);

    ///< 判断是否发生转换异常
    if (NULL == returnTheListData)
    {
        //LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    returnTheListData->setAccount(account.c_str());
    returnTheListData->setUserName(userName.c_str());
    returnTheListData->setSpelling(userPinyin.c_str());
    returnTheListData->setOnLine(on_line);

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

    /*LogManager::getSingleton().logDebug("准备发送数据: 数据大小为: " + 
    boost::lexical_cast<std::string>(block.size()));*/

    int sandSize = write(block);

    /*LogManager::getSingleton().logDebug("发送数据进入缓冲区: 数据大小为: " + 
    boost::lexical_cast<std::string>(sandSize));*/

    waitForBytesWritten();

    //LogManager::getSingleton().logDebug("发送数据成功");

    ///< 删除消息
    delete data;
    data = NULL;
}

