#pragma once

#include "ComDataType.h"
#include "common.h"

class ComDataTypeFactory
{
public:
    ComDataTypeFactory(void);
    ~ComDataTypeFactory(void);

    ComDataType* createComData(MsgType msgType);
};

