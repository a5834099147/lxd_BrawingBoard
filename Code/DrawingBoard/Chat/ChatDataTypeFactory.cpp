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
            LogManager::getSingleton().logDebug("创建一个聊天消息");
            chatDataType = new ChatMessageDataType();
            break;
        }
    case CM_EXIT :
        {
            LogManager::getSingleton().logDebug("创建一个退出消息");
            chatDataType = new ChatExitType();
            break;
        }
    case CM_FILENAME:
        {
            LogManager::getSingleton().logDebug("创建一个文件名接受消息");
            assert(false);
            break;
        }
    default:
        {
            LogManager::getSingleton().logError("创建一个没有定义的聊天消息");
            assert(false);
        }
    }

    return chatDataType;
}
