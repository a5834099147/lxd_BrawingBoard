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