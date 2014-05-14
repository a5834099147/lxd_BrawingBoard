//********************************************************************
//    COMDATATYPE.H    文件注释
//    文件名  : COMDATATYPE.H
//    作者    : 李晓东
//    TEL     : 18628980285
//    邮箱    : a5834099147@126.com
//    创建时间: 2014/5/13 20:39
//    文件描述: 
//*********************************************************************
#ifndef __COMDATATYPE_H__
#define __COMDATATYPE_H__

#include <QString>

#include "common.h"

class ComDataType
{
public:
    ComDataType(void);
    ComDataType(MsgType msgType);

    ~ComDataType(void);

    void virtual getData(QDataStream &dataStream) = 0;
    void virtual setData(QDataStream &dataStream) = 0;

public:
    const MsgType& getMsgType(void)
    {
        return m_msgType;
    }

    void setMsgType(MsgType msgType)
    {
        m_msgType = msgType;
    }

protected:
    MsgType m_msgType;
};

class LandingDataType : public ComDataType
{
public:
    LandingDataType(void);
    LandingDataType(QString account, QString password);

    ~LandingDataType(void);

    void getData(QDataStream &dataStream);
    void setData(QDataStream &dataStream);

public:
    const QString& getAccount(void)
    {
        return m_account;
    }

    void setAccount(QString account)
    {
        m_account = account;
    }

    const QString& getPassword(void)
    {
        return m_password;
    }

    void setPassword(QString password)
    {
        m_password = password;
    }

private:
    QString m_account;
    QString m_password;
};

class LandingResultType : public ComDataType
{
public:
    LandingResultType(void);
    LandingResultType(bool result);

    ~LandingResultType(void);

    void getData(QDataStream &dataStream);
    void setData(QDataStream &dataStream);

public:
    const bool getResult(void)
    {
        return m_result;
    }

    void setResult(bool result)
    {
        m_result = result;
    }

private:
    bool m_result;
};

class RegisterDataType : public ComDataType
{
public:
    RegisterDataType(void);
    RegisterDataType(QString account, QString password, QString userName, QString spelling);

    ~RegisterDataType(void);

    void getData(QDataStream &dataStream);
    void setData(QDataStream &dataStream);

public:
    const QString& getAccount(void)
    {
        return m_account;
    }

    void setAccount(QString account)
    {
        m_account = account;
    }

    const QString& getPassword(void)
    {
        return m_password;
    }

    void setPassword(QString password)
    {
        m_password = password;
    }

    const QString& getUserName(void)
    {
        return m_userName;
    }

    void setUserName(QString userName)
    {
        m_userName = userName;
    }

    const QString& getSpelling(void)
    {
        return m_spelling;
    }

    void setSpelling(QString spelling)
    {
        m_spelling = spelling;
    }

private:
    QString m_account;
    QString m_password;
    QString m_userName;
    QString m_spelling;
};

class RegisterResultType : public ComDataType
{
public:
    RegisterResultType(void);
    RegisterResultType(bool result);

    ~RegisterResultType(void);

    void getData(QDataStream &dataStream);
    void setData(QDataStream &dataStream);

public:
    const bool getResult(void)
    {
        return m_result;
    }

    void setResult(bool result)
    {
        m_result = result;
    }

private:
    bool m_result;
};

class ReturnTheListDataType : public ComDataType
{
public:
    ReturnTheListDataType(void);
    ReturnTheListDataType(QString account, QString userName, QString spelling, bool onLine);

    ~ReturnTheListDataType(void);

    void getData(QDataStream &dataStream);
    void setData(QDataStream &dataStream);

public:
    const QString& getAccount(void)
    {
        return m_account;
    }

    void setAccount(QString account)
    {
        m_account = account;
    }

    const QString& getUserName(void)
    {
        return m_userName;
    }

    void setUserName(QString userName)
    {
        m_userName = userName;
    }

    const QString& getSpelling(void)
    {
        return m_spelling;
    }

    void setSpelling(QString spelling)
    {
        m_spelling = spelling;
    }

    const bool getOnLine(void)
    {
        return m_onLine;
    }

    void setOnLine(bool onLine)
    {
        m_onLine = onLine;
    }

private:
    QString m_account;
    QString m_userName;
    QString m_spelling;

    ///< 是否在线
    bool m_onLine;
};

class SandMessageType : public ComDataType
{
public:
    SandMessageType(void);
    SandMessageType(QString account, QString userName, QString message);

    ~SandMessageType(void);

    void getData(QDataStream &dataStream);
    void setData(QDataStream &dataStream);

public:
    const QString& getAccount(void)
    {
        return m_account;
    }

    void setAccount(QString account)
    {
        m_account = account;
    }

    const QString& getUserName(void)
    {
        return m_userName;
    }

    void setUserName(QString userName)
    {
        m_userName = userName;
    }

    const QString& getMessage(void)
    {
        return m_message;
    }

    void setMessage(QString message)
    {
        m_message = message;
    }

private:
    QString m_account;
    QString m_userName;
    QString m_message;
};



#endif  ///<__COMDATATYPE_H__
