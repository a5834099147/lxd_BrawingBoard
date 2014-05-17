#pragma once

#include "ChatDataType.h"
#include "common.h"

class ChatDataTypeFactory
{
public:
    ChatDataTypeFactory(void);
    ~ChatDataTypeFactory(void);

    ChatDataType* createChatMessageType(ChatMessageType msgType);
};

