#ifndef CHAT_H
#define CHAT_H

#include <QDialog>
#include <QtNetwork/QtNetwork>
#include <QtGui>

namespace Ui {
    class chat;
}

class chat : public QDialog
{
    Q_OBJECT

public:
    ~chat();
    chat(QString pasvusername, QString pasvuserip, qint32 localPort, qint32 targPort);

private:
    Ui::chat *ui;
    QColor color;//��ɫ
    QString m_message;
    QString m_targIp;
    QString m_targAccount;
    QUdpSocket *m_chatSocket;
    qint32 m_localPort;
    qint32 m_targPort;

private:
    void sendMessage();
    bool saveFile(const QString& fileName);//���������¼
    QString getMessage();
    QString getIP();

protected:
    bool eventFilter(QObject *target, QEvent *event); //�¼�������

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
};

#endif // CHAT_H
