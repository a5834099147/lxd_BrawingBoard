#include "clientsocket.h"
#include "ComDataType.h"

#include <boost/lexical_cast.hpp>
#include <cassert>
#include <QtNetwork/QHostAddress>
#include <string>

ClientSocket::ClientSocket(QObject *parent)
    : QTcpSocket(parent)
{
    ///< �������ݿ��Զ�ȡʱ, ���ö�ȡ��
    connect(this, SIGNAL(readyRead()), this, SLOT(receiveData()));
}

ClientSocket::~ClientSocket()
{
}

bool ClientSocket::connectToServer(const char* ip, quint16 port)
{
    ///< ���ӷ�����
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
    ///< ������Ϣ����ʵ��
    ComDataType* com = comDataFactory.createComData(MT_LANDING_DATA);

    ///< ������ָ��ת��Ϊ����ʵ��ָ��
    LandingDataType* landingData = dynamic_cast<LandingDataType*>(com);

    ///< �ж��Ƿ���ת���쳣
    if (NULL == landingData)
    {
        LogManager::getSingleton().logAlert("����ת��ʱ���ִ���, ������ָ���쳣");
        assert(false);
    }

    ///< ���÷���ʵ���˺�
    landingData->setAccount(account.c_str());
    ///< ���÷���ʵ������
    landingData->setPassword(password.c_str());

    ///< ���͸�ʵ��
    sandData(com);
}

void ClientSocket::sandRegister(std::string account, std::string password, std::string userName, std::string pinYin)
{
    ///< ������Ϣע��ʵ��
    ComDataType* com = comDataFactory.createComData(MT_REGISTER_DATA);

    ///< ������ָ��ת��Ϊע��ʵ��ָ��
    RegisterDataType* registerDate = dynamic_cast<RegisterDataType*>(com);

    ///< �ж��Ƿ���ת���쳣
    if (NULL == registerDate)
    {
        LogManager::getSingleton().logAlert("����ת��ʱ���ִ���, ������ָ���쳣");
        assert(false);
    }

    ///< ����ע���˺�
    registerDate->setAccount(account.c_str());
    ///< ����ע������
    registerDate->setPassword(password.c_str());
    ///< ����ע���û���
    registerDate->setUserName(userName.c_str());
    ///< ����ע���û���ƴ��
    registerDate->setSpelling(pinYin.c_str());

    ///< ���͸�ʵ��
    sandData(com);
}

void ClientSocket::sandData(ComDataType* data)
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

void ClientSocket::receiveData()
{
    ///< ������Ϣ
    LogManager::getSingleton().logDebug("׼����������");

    ///< ������Ϊֻ��
    QDataStream receive(this);
    ///< �����������İ汾, �ͻ�����������ͬ
    receive.setVersion(QDataStream::Qt_4_8);

    while (!receive.atEnd())
    {
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

        switch(msgType)
        {
        case MT_LANDING_RESULT:
            {
                LogManager::getSingleton().logDebug("���ɵ�½���������������Ϣ");
                requestLanding_Result(com);
                break;
            }
        case MT_GETMESSAGE_DATA:
            {
                LogManager::getSingleton().logDebug("���ɻ�ȡ��Ϣ�����ຯ������������Ϣ");
                requestGetMessage_Message(com);
                break;
            }
        case MT_GETMESSAGE_MESSAGE:
            {
                LogManager::getSingleton().logDebug("���ɻ�ȡ��Ϣ��Ϣ�ຯ��������Ϣ��Ϣ");
                requestGetMessage_Data(com);
                break;
            }
        case MT_REGISTER_RESULT:
            {
                LogManager::getSingleton().logDebug("����ע��������������ע����");
                requestRegister_Result(com);
                break;
            }

        case MT_RETURNTHELIST_DATA:
            {
                LogManager::getSingleton().logDebug("�������ݱ������������ݱ���Ϣ");
                requestUserList(com);
                break; 
            }
        default:
            {
                LogManager::getSingleton().logError("�յ��Ƿ���Ϣ����");
                assert(false);
            }
        }
    }
}

void ClientSocket::requestLanding_Result(ComDataType* data)
{
    ///< ������ָ��ת��Ϊ��½���ʵ��ָ��
    LandingResultType* landingResult = dynamic_cast<LandingResultType*>(data);

    ///< �ж��Ƿ���ת���쳣
    if (NULL == landingResult)
    {
        LogManager::getSingleton().logAlert("����ת��ʱ���ִ���, ������ָ���쳣");
        assert(false);
    }

    LogManager::getSingleton().logDebug("���͵�½��������Ϣ");
    emit recive_login(landingResult->getResult());

    delete data;
    data = NULL;
}

void ClientSocket::requestRegister_Result(ComDataType* data)
{
    ///< ������ָ��ת��Ϊע����ʵ��ָ��
    RegisterResultType* registerResult = dynamic_cast<RegisterResultType*>(data);

    ///< �ж��Ƿ���ת���쳣
    if (NULL == registerResult)
    {
        LogManager::getSingleton().logAlert("����ת��ʱ���ִ���, ������ָ���쳣");
        assert(false);
    }

    LogManager::getSingleton().logDebug("����ע�ᴦ������Ϣ");

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
    ///< ������ָ��ת��Ϊע����ʵ��ָ��
    ReturnTheListDataType* returnTheListData = dynamic_cast<ReturnTheListDataType*>(data);

    ///< �ж��Ƿ���ת���쳣
    if (NULL == returnTheListData)
    {
        LogManager::getSingleton().logAlert("����ת��ʱ���ִ���, ������ָ���쳣");
        assert(false);
    }

    LogManager::getSingleton().logDebug("���ܵ��û��б���Ϣ");
    emit recive_userList(returnTheListData->getAccount().toStdString(),
                         returnTheListData->getUserName().toStdString(),
                         returnTheListData->getSpelling().toStdString(),
                         returnTheListData->getOnLine());

    delete data;
    data = NULL;
}
