﻿#include "ComDataTypeFactory.h"

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
            LogManager::getSingleton().logDebug("创建一个登陆消息");
            comDataType = new LandingDataType();
            break;
        }
    case MT_LANDING_RESULT:
        {
            LogManager::getSingleton().logDebug("创建一个登陆结果消息");
            comDataType = new LandingResultType();
            break;
        }
    case MT_REGISTER_DATA:
        {
            LogManager::getSingleton().logDebug("创建一个注册消息");
            comDataType = new RegisterDataType();
            break;
        }
    case MT_REGISTER_RESULT:
        {
            LogManager::getSingleton().logDebug("创建一个注册结果消息");
            comDataType = new RegisterResultType();
            break;
        }
    case MT_RETURNTHELIST_DATA:
        {
            LogManager::getSingleton().logDebug("创建一个列表信息消息");
            comDataType = new ReturnTheListDataType();
            break;
        }
    case MT_SENDMESSAGE_MESSAGE:
        {
            LogManager::getSingleton().logDebug("创建一个会话消息");
            comDataType = new SandMessageType();
            break;
        }
    case MT_CHATREQUESTS_DATA:
        {
            LogManager::getSingleton().logDebug("创建一个聊天请求消息");
            comDataType = new ChatRequestDataType();
            break;
        }
    case MT_CHATREQUESTS_RESULT:
        {
            LogManager::getSingleton().logDebug("创建一个聊天请求结果消息");
            comDataType = new ChatRequestResultType();
            break;
        }
    case MT_OPENCHATSPORT:
        {
            LogManager::getSingleton().logDebug("创建一个开始聊天消息");
            comDataType = new OpenChatsPort();
            break;
        }
    default:
        {
            LogManager::getSingleton().logError("出现非法创建消息");
            assert(false);
        }
    }

    return comDataType;
}
