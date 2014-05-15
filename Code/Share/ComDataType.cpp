#include "ComDataType.h"

#include <QDataStream>

///< COMDATATYPE
ComDataType::ComDataType(void)
{
}

ComDataType::ComDataType( MsgType msgType )
    :m_msgType(msgType)
{
}

ComDataType::~ComDataType( void )
{

}

///< LANDINGDATATYPE
LandingDataType::LandingDataType( QString account, QString password )
    : ComDataType(MT_LANDING_DATA),
    m_account(account),
    m_password(password)
{

}

LandingDataType::LandingDataType( void )
    : ComDataType(MT_LANDING_DATA)
{

}

LandingDataType::~LandingDataType( void )
{

}

void LandingDataType::getData( QDataStream &dataStream )
{
    dataStream << (qint32)m_msgType << m_account << m_password;
}

void LandingDataType::setData( QDataStream &dataStream )
{
    dataStream >> m_account >> m_password;
}


///< LANDINGRESULTTYPE
LandingResultType::LandingResultType( bool result )
    : ComDataType(MT_LANDING_RESULT),
    m_result(result)
{

}

LandingResultType::LandingResultType( void )
    : ComDataType(MT_LANDING_RESULT)
{

}

LandingResultType::~LandingResultType( void )
{

}

void LandingResultType::getData( QDataStream &dataStream )
{
    dataStream << (qint32)m_msgType << m_result;
}

void LandingResultType::setData( QDataStream &dataStream )
{
    dataStream >> m_result;
}


///< REGISTERDATATYPE
RegisterDataType::RegisterDataType( QString account, QString password, QString userName, QString spelling )
    : ComDataType(MT_REGISTER_DATA),
    m_account(account),
    m_password(password),
    m_userName(userName),
    m_spelling(spelling)
{

}

RegisterDataType::RegisterDataType( void )
    : ComDataType(MT_REGISTER_DATA)
{

}

RegisterDataType::~RegisterDataType( void )
{

}

void RegisterDataType::getData( QDataStream &dataStream )
{
    dataStream << (qint32)m_msgType << m_account << m_password << m_userName << m_spelling;
}

void RegisterDataType::setData( QDataStream &dataStream )
{
    dataStream >> m_account >> m_password >> m_userName >> m_spelling;
}


///< REGISTERRESULTTYPE
RegisterResultType::RegisterResultType( bool result )
    : ComDataType(MT_REGISTER_RESULT),
    m_result(result)
{

}

RegisterResultType::RegisterResultType( void )
    : ComDataType(MT_REGISTER_RESULT)
{

}

RegisterResultType::~RegisterResultType( void )
{

}

void RegisterResultType::getData( QDataStream &dataStream )
{
    dataStream << (qint32)m_msgType << m_result;
}

void RegisterResultType::setData( QDataStream &dataStream )
{
    dataStream >> m_result;
}


///< RETURNTHELISTDATATYPE
ReturnTheListDataType::ReturnTheListDataType( QString account, QString userName, QString spelling, bool onLine, bool isUpdate )
    : ComDataType(MT_RETURNTHELIST_DATA),
    m_account(account),
    m_userName(userName),
    m_spelling(spelling),
    m_onLine(onLine),
    m_isUpdate(isUpdate)
{

}

ReturnTheListDataType::ReturnTheListDataType( void )
    : ComDataType(MT_RETURNTHELIST_DATA)
{

}

ReturnTheListDataType::~ReturnTheListDataType( void )
{

}

void ReturnTheListDataType::getData( QDataStream &dataStream )
{
    dataStream << (qint32)m_msgType << m_account << m_userName << m_spelling 
               << m_onLine << m_isUpdate;
}

void ReturnTheListDataType::setData( QDataStream &dataStream )
{
    dataStream >> m_account >> m_userName >> m_spelling >> m_onLine >> m_isUpdate;
}


///< SANDMESSAGETYPE
SandMessageType::SandMessageType( QString account, QString userName, QString message )
    : ComDataType(MT_SENDMESSAGE_MESSAGE),
    m_account(account),
    m_userName(userName),
    m_message(message)
{

}

SandMessageType::SandMessageType( void )
    : ComDataType(MT_SENDMESSAGE_MESSAGE)
{

}

SandMessageType::~SandMessageType( void )
{

}

void SandMessageType::getData( QDataStream &dataStream )
{
    dataStream << (qint32)m_msgType << m_account << m_userName << m_message;
}

void SandMessageType::setData( QDataStream &dataStream )
{
    dataStream >> m_account >> m_userName >> m_message;
}

///< CHATREQUESTSDATA
ChatRequestDataType::ChatRequestDataType( void )
    :ComDataType(MT_CHATREQUESTS_DATA)
{

}

ChatRequestDataType::ChatRequestDataType( QString localAccount, QString localUserName, QString otherAccount, QString otherUserName )
    :ComDataType(MT_CHATREQUESTS_DATA),
    m_localAccount(localAccount),
    m_localUserName(localUserName),
    m_otherAccount(otherAccount),
    m_otherUserName(otherUserName)
{

}

ChatRequestDataType::~ChatRequestDataType( void )
{

}


void ChatRequestDataType::getData( QDataStream &dataStream )
{
    dataStream << (qint32)m_msgType << m_localAccount << m_localUserName
               << m_otherAccount << m_otherUserName;
}

void ChatRequestDataType::setData( QDataStream &dataStream )
{
    dataStream >> m_localAccount >> m_localUserName 
               >> m_otherAccount >> m_otherUserName;
}


///< CHARTQUESTRESULT
ChatRequestResultType::ChatRequestResultType( void )
    :ComDataType(MT_CHATREQUESTS_RESULT)
{

}

ChatRequestResultType::ChatRequestResultType( QString account, QString userName, bool result )
    :ComDataType(MT_CHATREQUESTS_RESULT),
    m_account(account),
    m_userName(userName),
    m_result(result)
{

}

ChatRequestResultType::~ChatRequestResultType( void )
{

}

void ChatRequestResultType::getData( QDataStream &dataStream )
{
    dataStream << (qint32)m_msgType << m_account << m_userName << m_result;
}

void ChatRequestResultType::setData( QDataStream &dataStream )
{
    dataStream >> m_account >> m_userName >> m_result;
}



