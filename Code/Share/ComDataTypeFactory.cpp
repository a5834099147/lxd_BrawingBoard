#include "ComDataTypeFactory.h"

#include "LogManager.h"


ComDataTypeFactory::ComDataTypeFactory(void)
{
}


ComDataTypeFactory::~ComDataTypeFactory(void)
{
}

ComDataType* ComDataTypeFactory::createComData( MsgType msgType )
{
    ComDataType* comDataType = NULL;

    switch(msgType)
    {
    case MT_LANDING_DATA:
        {
            LogManager::getSingleton().logDebug("����һ����½��Ϣ");
            comDataType = new LandingDataType();
            break;
        }
    case MT_LANDING_RESULT:
        {
            LogManager::getSingleton().logDebug("����һ����½�����Ϣ");
            comDataType = new LandingResultType();
            break;
        }
    case MT_REGISTER_DATA:
        {
            LogManager::getSingleton().logDebug("����һ��ע����Ϣ");
            comDataType = new RegisterDataType();
            break;
        }
    case MT_REGISTER_RESULT:
        {
            LogManager::getSingleton().logDebug("����һ��ע������Ϣ");
            comDataType = new RegisterResultType();
            break;
        }
    case MT_RETURNTHELIST_DATA:
        {
            LogManager::getSingleton().logDebug("����һ���б���Ϣ��Ϣ");
            comDataType = new ReturnTheListDataType();
            break;
        }
    case MT_SENDMESSAGE_MESSAGE:
        {
            LogManager::getSingleton().logDebug("����һ���Ự��Ϣ");
            comDataType = new SandMessageType();
            break;
        }
    case MT_CHATREQUESTS_DATA:
        {
            LogManager::getSingleton().logDebug("����һ������������Ϣ");
            comDataType = new ChatRequestDataType();
            break;
        }
    case MT_CHATREQUESTS_RESULT:
        {
            LogManager::getSingleton().logDebug("����һ��������������Ϣ");
            comDataType = new ChatRequestResultType();
            break;
        }
    default:
        {
            LogManager::getSingleton().logError("���ַǷ�������Ϣ");
            assert(false);
        }
    }

    return comDataType;
}
