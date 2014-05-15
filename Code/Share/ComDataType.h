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

///< 基类
class ComDataType
{
public:
    ComDataType(void);
    ComDataType(MsgType msgType);

    ~ComDataType(void);

    void virtual getData(QDataStream &dataStream) = 0;
    void virtual setData(QDataStream &dataStream) = 0;

protected:
    MsgType m_msgType;
};

///< 登陆请求数据
class LandingDataType : public ComDataType
{
public:
    LandingDataType(void);
    LandingDataType(QString account, QString password);

    ~LandingDataType(void);

    void getData(QDataStream &dataStream);
    void setData(QDataStream &dataStream);

public:
    const QString& getAccount(void) const
    {
        return m_account;
    }

    void setAccount(QString account)
    {
        m_account = account;
    }

    const QString& getPassword(void) const
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

///< 登陆请求结果
class LandingResultType : public ComDataType
{
public:
    LandingResultType(void);
    LandingResultType(bool result);

    ~LandingResultType(void);

    void getData(QDataStream &dataStream);
    void setData(QDataStream &dataStream);

public:
    const bool getResult(void) const
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

///< 注册请求数据
class RegisterDataType : public ComDataType
{
public:
    RegisterDataType(void);
    RegisterDataType(QString account, QString password, QString userName, QString spelling);

    ~RegisterDataType(void);

    void getData(QDataStream &dataStream);
    void setData(QDataStream &dataStream);

public:
    const QString& getAccount(void) const
    {
        return m_account;
    }

    void setAccount(QString account)
    {
        m_account = account;
    }

    const QString& getPassword(void) const
    {
        return m_password;
    }

    void setPassword(QString password)
    {
        m_password = password;
    }

    const QString& getUserName(void) const
    {
        return m_userName;
    }

    void setUserName(QString userName)
    {
        m_userName = userName;
    }

    const QString& getSpelling(void) const
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

///< 注册请求结果
class RegisterResultType : public ComDataType
{
public:
    RegisterResultType(void);
    RegisterResultType(bool result);

    ~RegisterResultType(void);

    void getData(QDataStream &dataStream);
    void setData(QDataStream &dataStream);

public:
    const bool getResult(void) const
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

///< 列表返回
class ReturnTheListDataType : public ComDataType
{
public:
    ReturnTheListDataType(void);
    ReturnTheListDataType(QString account, QString userName, QString spelling,
                          bool onLine, bool isUpdate);

    ~ReturnTheListDataType(void);

    void getData(QDataStream &dataStream);
    void setData(QDataStream &dataStream);

public:
    const QString& getAccount(void) const
    {
        return m_account;
    }

    void setAccount(QString account)
    {
        m_account = account;
    }

    const QString& getUserName(void) const
    {
        return m_userName;
    }

    void setUserName(QString userName)
    {
        m_userName = userName;
    }

    const QString& getSpelling(void) const
    {
        return m_spelling;
    }

    void setSpelling(QString spelling)
    {
        m_spelling = spelling;
    }

    const bool getOnLine(void) const
    {
        return m_onLine;
    }

    void setOnLine(bool onLine)
    {
        m_onLine = onLine;
    }

    const bool getIsUpdate(void) const
    {
        return m_isUpdate;
    }

    void setIsUpdate(bool isUpdate)
    {
        m_isUpdate = isUpdate;
    }

private:
    QString m_account;
    QString m_userName;
    QString m_spelling;

    ///< 是否在线
    bool m_onLine;
    ///< 是否为更新
    bool m_isUpdate;
};

///< 发送消息
class SandMessageType : public ComDataType
{
public:
    SandMessageType(void);
    SandMessageType(QString account, QString userName, QString message);

    ~SandMessageType(void);

    void getData(QDataStream &dataStream);
    void setData(QDataStream &dataStream);

public:
    const QString& getAccount(void) const
    {
        return m_account;
    }

    void setAccount(QString account)
    {
        m_account = account;
    }

    const QString& getUserName(void) const
    {
        return m_userName;
    }

    void setUserName(QString userName)
    {
        m_userName = userName;
    }

    const QString& getMessage(void) const
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

///< 聊天请求
class ChatRequestDataType : public ComDataType
{
public:
    ChatRequestDataType(void);
    ChatRequestDataType(QString localAccount, QString localUserName, 
                        QString otherAccount, QString otherUserName);

     ~ChatRequestDataType(void);

    void getData(QDataStream &dataStream);
    void setData(QDataStream &dataStream);

public:
    const QString& getLocalAccount() const
    {
        return m_localAccount;
    }
    void setLocalAccount(QString val)
    {
        m_localAccount = val;
    }

    const QString& getLocalUserName() const
    {
        return m_localUserName;
    }
    void setLocalUserName(QString val)
    {
        m_localUserName = val;
    }

    const QString& getOtherAccount() const
    {
        return m_otherAccount;
    }
    void setOtherAccount(QString val)
    {
        m_otherAccount = val;
    }

    const QString& getOtherUserName() const
    {
        return m_otherUserName;
    }
    void setOtherUserName(QString val)
    {
        m_otherUserName = val;
    }

private:
    QString m_localAccount;
    QString m_localUserName;
    QString m_otherAccount;
    QString m_otherUserName;
};

///< 聊天请求结果
class ChatRequestResultType : public ComDataType
{
public:
    ChatRequestResultType(void);
    ChatRequestResultType(QString account, QString userName, bool result);

    ~ChatRequestResultType(void);

    void virtual getData( QDataStream &dataStream );
    void virtual setData( QDataStream &dataStream );

public:
    bool getResult() const
    {
        return m_result; 
    }
    void setResult(bool val) {
        m_result = val;
    }

    QString getAccount() const
    {
        return m_account;
    }
    void setAccount(QString val)
    {
        m_account = val;
    }

    QString getUserName() const
    {
        return m_userName;
    }
    void setUserName(QString val)
    {
        m_userName = val;
    }

private:
    bool m_result;
    QString m_account;
    QString m_userName;
};


#endif  ///<__COMDATATYPE_H__
