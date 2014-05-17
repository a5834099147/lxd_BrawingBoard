#include "ChatDataTypeFactory.h"
#include "LogManager.h"


ChatDataTypeFactory::ChatDataTypeFactory(void)
{
}


ChatDataTypeFactory::~ChatDataTypeFactory(void)
{
}

ChatDataType* ChatDataTypeFactory::createChatMessageType( ChatMessageType msgType )
{
    ChatDataType* chatDataType = NULL;

    switch (msgType)
    {
    case CM_MESSAGE :
        {
            LogManager::getSingleton().logDebug("����һ��������Ϣ");
            chatDataType = new ChatMessageDataType();
            break;
        }
    case CM_EXIT :
        {
            LogManager::getSingleton().logDebug("����һ���˳���Ϣ");
            chatDataType = new ChatExitType();
            break;
        }
    case CM_FILENAME:
        {
            LogManager::getSingleton().logDebug("����һ���ļ���������Ϣ");
            assert(false);
            break;
        }
    default:
        {
            LogManager::getSingleton().logError("����һ��û�ж����������Ϣ");
            assert(false);
        }
    }

    return chatDataType;
}
