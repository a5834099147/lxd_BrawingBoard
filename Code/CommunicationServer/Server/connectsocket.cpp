#include "connectsocket.h"
#include "ComDataType.h"
#include "odb_user.h"
#include "LogManager.h"

#include <boost/lexical_cast.hpp>
#include <cassert>
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

    ///< ������Ϣ
    LogManager::getSingleton().logDebug("������׼����������");

    ///< ������Ϊֻ��
    QDataStream receive(this);
    ///< �����������İ汾, �ͻ�����������ͬ
    receive.setVersion(QDataStream::Qt_4_8);

    ///< ��ȡ���ݴ�С
    qint32 reciveSize = 0;
    receive >> reciveSize;

    LogManager::getSingleton().logDebug("�յ�������������Ϣ, ��װʱ���ݴ�СΪ: " +
        boost::lexical_cast<std::string>(reciveSize));

    ///< ��ȡ��Ϣ����
    MsgType msgType;
    receive >> (qint32&)msgType;

    ComDataType* com = comDataFactory.createComData(msgType);
    com->setData(receive);

    receiveDataProcessing(msgType, com);
}

void ConnectSocket::requestLandingData(ComDataType* data)
{
    ///< ������ָ��ת��Ϊ��½���ʵ��ָ��
    LandingDataType* landingData = dynamic_cast<LandingDataType*>(data);

    ///< �ж��Ƿ���ת���쳣
    if (NULL == landingData)
    {
        LogManager::getSingleton().logAlert("����ת��ʱ���ִ���, ������ָ���쳣");
        assert(false);
    }

    bool result = odb_user::logon(landingData->getAccount().toStdString(), 
                                  landingData->getPassword().toStdString());	
    sandLogin_result(result);

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
    ///< ������ָ��ת��Ϊ��½���ʵ��ָ��
    RegisterDataType* registerData = dynamic_cast<RegisterDataType*>(data);

    ///< �ж��Ƿ���ת���쳣
    if (NULL == registerData)
    {
        LogManager::getSingleton().logAlert("����ת��ʱ���ִ���, ������ָ���쳣");
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

void ConnectSocket::sandLogin_result(bool result)
{

    ///< ������Ϣע��ʵ��
    ComDataType* com = comDataFactory.createComData(MT_LANDING_RESULT);

    ///< ������ָ��ת��Ϊע��ʵ��ָ��
    LandingResultType* landingResult = dynamic_cast<LandingResultType*>(com);

    ///< �ж��Ƿ���ת���쳣
    if (NULL == landingResult)
    {
        LogManager::getSingleton().logAlert("����ת��ʱ���ִ���, ������ָ���쳣");
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

    ///< ������Ϣע��ʵ��
    ComDataType* com = comDataFactory.createComData(MT_REGISTER_RESULT);

    ///< ������ָ��ת��Ϊע��ʵ��ָ��
    RegisterResultType* registerResult = dynamic_cast<RegisterResultType*>(com);

    ///< �ж��Ƿ���ת���쳣
    if (NULL == registerResult)
    {
        LogManager::getSingleton().logAlert("����ת��ʱ���ִ���, ������ָ���쳣");
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

    ///< ������Ϣע��ʵ��
    ComDataType* com = comDataFactory.createComData(MT_RETURNTHELIST_DATA);

    ///< ������ָ��ת��Ϊע��ʵ��ָ��
    ReturnTheListDataType* returnTheListData = dynamic_cast<ReturnTheListDataType*>(com);

    ///< �ж��Ƿ���ת���쳣
    if (NULL == returnTheListData)
    {
        LogManager::getSingleton().logAlert("����ת��ʱ���ִ���, ������ָ���쳣");
        assert(false);
    }

    returnTheListData->setAccount(account.c_str());
    returnTheListData->setUserName(userName.c_str());
    returnTheListData->setSpelling(userPinyin.c_str());
    returnTheListData->setOnLine(on_line);
    returnTheListData->setIsUpdate(isUpdate);

    sandData(com);
}

void ConnectSocket::sandData(ComDataType* data)
{
    ///< ��Ҫ���͵��ֽ���
    QByteArray block;
    ///< ������Ϊֻд
    QDataStream sand(&block, QIODevice::WriteOnly);
    ///< �����������İ汾, �ͻ�����������ͬ
    sand.setVersion(QDataStream::Qt_4_8);

    ///< ���÷��ͳ��ȳ�ʼֵΪ0
    sand << (qint32) 0;
    ///< ���÷��͵�����
    data->getData(sand);
    ///< �ص��ֽ�����ʼλ��
    sand.device()->seek(0);
    ///< �����ֽ�������
    sand << (qint32)(block.size() - sizeof(qint32));

    LogManager::getSingleton().logDebug("׼����������: ���ݴ�СΪ: " + 
    boost::lexical_cast<std::string>(block.size()));

    int sandSize = write(block);

    LogManager::getSingleton().logDebug("�������ݽ��뻺����: ���ݴ�СΪ: " + 
    boost::lexical_cast<std::string>(sandSize));

    waitForBytesWritten();

    LogManager::getSingleton().logDebug("�������ݳɹ�");

    ///< ɾ����Ϣ
    delete data;
    data = NULL;
}

void ConnectSocket::requestChatRequest( ComDataType* data )
{

}

void ConnectSocket::receiveDataProcessing( MsgType type, ComDataType* data )
{
    switch(type)
    {
    case MT_LANDING_DATA:
        {
            LogManager::getSingleton().logDebug("���յ�����Ϣ���ɵ�½����������");
            requestLandingData(data);
            break;
        }
    case MT_SENDMESSAGE_MESSAGE:
        {
            LogManager::getSingleton().logDebug("���յ�����Ϣ���ɽ�����Ϣ��������");
            requestSendMessage_Message(data);
            break;
        }
    case MT_SENDMESSAGE_DATA:
        {
            LogManager::getSingleton().logDebug("���յ�����Ϣ���ɽ������ݺ�������");
            requestSendMessage_Data(data);
            break;
        }
    case MT_REGISTER_DATA:
        {
            LogManager::getSingleton().logDebug("���յ�����Ϣ����ע�ᴦ��������");
            requestRegister(data);
            break;
        }
    case MT_CHATREQUESTS_DATA:
        {
            LogManager::getSingleton().logDebug("���յ�����Ϣ��������������������");
            requestChatRequest(data);
            break;
        }
    default:
        {
            LogManager::getSingleton().logDebug("�յ��Ƿ���Ϣ����");
            assert(false);
        }
    }
}

void ConnectSocket::updataTheList( QString& account, bool state )
{
    LogManager::getSingleton().logDebug("���͸����û���:" + account.toStdString() + "��״̬Ϊ" + (state ? "����": "������"));
    sandUserList(account.toStdString(), "", "", state, true);
}

