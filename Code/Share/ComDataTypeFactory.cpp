#include "ComDataTypeFactory.h"

#include "LogManager.h"

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

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
            LogManager::getSingleton().logDebug("Create a Loading Message.");
            comDataType = new LandingDataType();
            break;
        }
    case MT_LANDING_RESULT:
        {
            LogManager::getSingleton().logDebug("Create a Loading result Message.");
            comDataType = new LandingResultType();
            break;
        }
    case MT_REGISTER_DATA:
        {
            LogManager::getSingleton().logDebug("Create a register Message.");
            comDataType = new RegisterDataType();
            break;
        }
    case MT_REGISTER_RESULT:
        {
            LogManager::getSingleton().logDebug("Create a register result Message.");
            comDataType = new RegisterResultType();
            break;
        }
    case MT_RETURNTHELIST_DATA:
        {
            LogManager::getSingleton().logDebug("Create a updata list Message.");
            comDataType = new ReturnTheListDataType();
            break;
        }
    case MT_SENDMESSAGE_MESSAGE:
        {
            LogManager::getSingleton().logDebug("Create a chat Message.");
            comDataType = new SandMessageType();
            break;
        }
    case MT_CHATREQUESTS_DATA:
        {
            LogManager::getSingleton().logDebug("Create a chat request Message.");
            comDataType = new ChatRequestDataType();
            break;
        }
    case MT_CHATREQUESTS_RESULT:
        {
            LogManager::getSingleton().logDebug("Create a chat result Message.");
            comDataType = new ChatRequestResultType();
            break;
        }
    case MT_OPENCHATSPORT:
        {
            LogManager::getSingleton().logDebug("Create a open chat port Message.");
            comDataType = new OpenChatsPort();
            break;
        }
    default:
        {
            LogManager::getSingleton().logError("Create a error Message.");
            assert(false);
        }
    }

    return comDataType;
}
