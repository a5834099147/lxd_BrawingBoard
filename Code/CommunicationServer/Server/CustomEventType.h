#include <QEvent>
#include <QString>

class CustomEventType : public QEvent
{
public:
    CustomEventType(unsigned int id)
        :QEvent((QEvent::Type)(QEvent::User + id))
    {
    }

    ~CustomEventType(){}
};

class ChangeTheList : public CustomEventType
{
public:
    ChangeTheList(const QString& name, bool onLine);
    ~ChangeTheList(){};

    QString getUserName() const
    {
        return m_userName;
    }
    void setUserName(QString val)
    {
        m_userName = val;
    }

    bool getOnLine() const 
    {
        return m_isOnLine; 
    }
    void setOnLine(bool val) 
    {
        m_isOnLine = val;
    }

private:
    QString m_userName;
    bool m_isOnLine;
};

class ChatRequest : public CustomEventType
{
public:
    ChatRequest(const QString& name);
    ~ChatRequest(){};

    QString getUserName() const
    {
        return m_userName;
    }
    void setUserName(QString val)
    {
        m_userName = val;
    }

private:
    QString m_userName;
};

class ChatRequestResult : public CustomEventType
{
public:
    ChatRequestResult(const QString& name, bool result);
    ~ChatRequestResult(){};

    QString getUserName() const
    {
        return m_userName;
    }
    void setUserName(QString val)
    {
        m_userName = val;
    }

    bool getResult() const 
    {
        return m_result; 
    }
    void setResult(bool val) 
    {
        m_result = val;
    }

private:
    QString m_userName;
    bool m_result;
};

class OpenChatPort : public CustomEventType
{
public:
    OpenChatPort(const QString& ipAddress, const QString& userName, quint32 localPort, quint32 targetPort);
    ~OpenChatPort(){};

    QString getAddress() const
    {
        return m_ipAddress;
    }
    void setAddress(QString val)
    {
        m_ipAddress = val;
    }

    QString getUserName() const
    {
        return m_userName;
    }
    void setUserName(QString val) 
    {
        m_userName = val;
    }

    quint32 getLocalPort() const
    {
        return m_localPort;
    }
    void setLocalPort(quint32 val)
    {
        m_localPort = val;
    }

    quint32 getTargetPort() const
    {
        return m_targetPort;
    }
    void setTargetPort(quint32 val)
    {
        m_targetPort = val;
    }

private:
    QString m_ipAddress;
    QString m_userName;
    quint32 m_localPort;
    quint32 m_targetPort;
};


