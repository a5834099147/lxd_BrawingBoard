#include "ChatDataType.h"

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

ChatDataType::ChatDataType(void)
{
}

ChatDataType::ChatDataType( ChatMessageType cMType )
    :m_cMType(cMType)
{

}

ChatDataType::~ChatDataType(void)
{
}

///< CHATMESSAGEDATATYPE
ChatMessageDataType::ChatMessageDataType( void )
    :ChatDataType(CM_MESSAGE)
{

}

ChatMessageDataType::ChatMessageDataType( QString message )
    :ChatDataType(CM_MESSAGE),
    m_message(message)
{

}

ChatMessageDataType::~ChatMessageDataType()
{

}

void ChatMessageDataType::getData( QDataStream &dataStream )
{
    dataStream << m_cMType << m_message;
}

void ChatMessageDataType::setData( QDataStream &dataStream )
{
    dataStream >> m_message;
}


///< CHATEXITTYPE
ChatExitType::ChatExitType( void )
    :ChatDataType(CM_EXIT)
{

}

ChatExitType::~ChatExitType( void )
{

}

void ChatExitType::getData( QDataStream &dataStream )
{
    dataStream << m_cMType;
}

void ChatExitType::setData( QDataStream& /*dataStream*/ )
{

}
