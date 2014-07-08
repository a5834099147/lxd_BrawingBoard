#ifndef CHAT_H
#define CHAT_H

#include <QDialog>
#include <QtNetwork/QtNetwork>
#include <QtGui>

#include "ChatDataTypeFactory.h"

class ChatDataType;

namespace Ui {
    class chat;
}

class Chat : public QDialog
{
    Q_OBJECT

public:
    ~Chat();
    Chat(QString pasvusername, QString pasvuserip, qint32 localPort, qint32 targPort);

public:
    QString getUserAccount() const;

signals:
    void exitTheChat();

private:
    bool saveFile(const QString& fileName);//保存聊天记录
    QString getMessage();
    QString getIP();
    
    void requestMessage(ChatDataType* cDType);
    void requestFileName(ChatDataType* cDType);
    void requestExit();
    void sendExit();
    void sendMessage();

    void sendUDPMessage(ChatDataType* cDType);

protected:
    bool eventFilter(QObject *target, QEvent *event); //事件过滤器
    void closeEvent(QCloseEvent *);

private slots:
    void processPendingDatagrams();
    void on_send_clicked();
    void on_clear_clicked();
    void on_save_clicked();
    void on_textcolor_clicked();
    void on_textUnderline_clicked(bool checked);
    void on_textitalic_clicked(bool checked);
    void on_textbold_clicked(bool checked);
    void on_fontComboBox_currentFontChanged(QFont f);
    void on_fontsizecomboBox_currentIndexChanged(QString );
    void currentFormatChanged(const QTextCharFormat &format);

private:
    Ui::chat *ui;
    QColor m_color;//颜色
    QString m_message;
    QString m_targIp;
    QString m_targAccount;
    qint32 m_localPort;
    qint32 m_targPort;
    bool m_passiveClose;
    bool m_canSend;

    QUdpSocket *m_chatSocket;

    ChatDataTypeFactory m_cDTypeFactory;
};

#endif // CHAT_H
