#ifndef _CHATDATATYPE_H__
#define _CHATDATATYPE_H__

#include "common.h"
#include <QDataStream>

#include <QString>

class ChatDataType
{
public:
    ChatDataType(void);
    ChatDataType(ChatMessageType cMType);

    ~ChatDataType(void);

public:
    void virtual getData(QDataStream &dataStream) = 0;
    void virtual setData(QDataStream &dataStream) = 0;

protected:
    ChatMessageType m_cMType;
};

class ChatMessageDataType : public ChatDataType 
{
public:
    ChatMessageDataType(void);
    ChatMessageDataType(QString message);

    ~ChatMessageDataType();

public:
    void getData( QDataStream &dataStream );
    void setData( QDataStream &dataStream );

    QString getMessage() const
    {
        return m_message;
    }
    void setMessage(QString val)
    {
        m_message = val;
    }

private:
    QString m_message;
};

class ChatExitType : public ChatDataType
{
public:
    ChatExitType(void);

    ~ChatExitType(void);

public:
    void getData( QDataStream &dataStream );
    void setData( QDataStream &dataStream );
};


#endif  ///<_CHATDATATYPE_H__
