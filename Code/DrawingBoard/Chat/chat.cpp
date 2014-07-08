#include "chat.h"
#include "ui_chat.h"
#include "common.h"
#include "ChatDataType.h"
#include "LogManager.h"

#include <QMessageBox>
#include <QScrollBar>
#include <QFileDialog>
#include <QColorDialog>
#include <cassert>

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

Chat::Chat(QString targAccont, QString targIp, qint32 localPort, qint32 targPort)
    : ui(new Ui::chat),
    m_localPort(localPort),
    m_targPort(targPort),
    m_targAccount(targAccont),
    m_targIp(targIp),
    m_passiveClose(false),
    m_canSend(true)
{
    ui->setupUi(this);
    ui->textEdit->setFocusPolicy(Qt::StrongFocus);
    ui->textBrowser->setFocusPolicy(Qt::NoFocus);

    ui->textEdit->setFocus();
    ui->textEdit->installEventFilter(this);

    ui->label->setText(tr("与%1聊天中   对方IP:%2").arg(m_targAccount).arg(m_targIp));
    setWindowTitle(QString("与 %1 聊天中...").arg(m_targAccount));

    //UDP部分
    m_chatSocket = new QUdpSocket(this);
    m_localPort = localPort;
    m_chatSocket->bind(m_localPort );
    connect(m_chatSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));

    connect(ui->textEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(currentFormatChanged(const QTextCharFormat)));
}

Chat::~Chat()
{
    delete ui;
}

bool Chat::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)//按下键盘某键
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);
            if(k->key() == Qt::Key_Return)//回车键
            {
                on_send_clicked();
                return true;
            }
        }
    }
    return QWidget::eventFilter(target,event);
}

QString Chat::getIP()  //获取ip地址
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        ///< 使用 IPv4地址
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
    return 0;
}

void Chat::processPendingDatagrams()   //接收数据UDP
{
    while(m_chatSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_chatSocket->pendingDatagramSize());
        m_chatSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);

        ChatMessageType chatMessageType;
        in >> (qint32&)chatMessageType;

        ChatDataType* chatDataType = m_cDTypeFactory.createChatMessageType(chatMessageType);
        chatDataType->setData(in);
        
        switch (chatMessageType)
        {
        case CM_MESSAGE:
            {
                LogManager::getSingleton().logDebug("聊天对话框收到带有聊天内容的消息, 交由聊天内容处理函数处理");
                requestMessage(chatDataType);
                break;
            }
        case CM_EXIT:
            {
                LogManager::getSingleton().logDebug("聊天对话框收到结束对话消息, 交由结束通话函数处理");
                requestExit();
                break;
            }
        case  CM_FILENAME:
            {
                LogManager::getSingleton().logDebug("聊天对话框收到文件请求消息, 交由文件请求函数处理");
                requestFileName(chatDataType);
                break;
            }
        default:
            {
                LogManager::getSingleton().logDebug("聊天对话框收到非法消息, 无法解析");
                assert(false);
                break;
            }
        }
    }
}

QString Chat::getMessage()  //获得要发送的信息
{
    QString msg = ui->textEdit->toHtml();
    ui->textEdit->clear();
    ui->textEdit->setFocus();
    return msg;
}

void Chat::sendMessage()  //发送信息
{
    if(ui->textEdit->toPlainText() == "")
    {
        QMessageBox::warning(0,tr("警告"),tr("发送内容不能为空"),QMessageBox::Ok);
        return;
    }

    if (!m_canSend)
    {
        QMessageBox::warning(NULL, tr("警告"), tr("对方已经离线, 发送消息失败"), QMessageBox::Ok);
        return;
    }

    ///< 创建消息实体
    ChatDataType* cDType = m_cDTypeFactory.createChatMessageType(CM_MESSAGE);

    ///< 将基类指针转换为消息实体指针
    ChatMessageDataType* cMDType = dynamic_cast<ChatMessageDataType*>(cDType);

    ///< 判断是否发生转换异常
    if (NULL == cMDType)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    m_message = getMessage();
    cMDType->setMessage(m_message);

    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());

    sendUDPMessage(cDType);
}

void Chat::currentFormatChanged(const QTextCharFormat &format)
{//当编辑器的字体格式改变时，我们让部件状态也随之改变
    ui->fontComboBox->setCurrentFont(format.font());

    if(format.fontPointSize()<9)  //如果字体大小出错，因为我们最小的字体为9
    {
        ui->fontsizecomboBox->setCurrentIndex(3); //即显示12
    }
    else
    {
        ui->fontsizecomboBox->setCurrentIndex(ui->fontsizecomboBox->findText(QString::number(format.fontPointSize())));
    }

    ui->textbold->setChecked(format.font().bold());
    ui->textitalic->setChecked(format.font().italic());
    ui->textUnderline->setChecked(format.font().underline());
    m_color = format.foreground().color();
}

void Chat::on_fontComboBox_currentFontChanged(QFont f)//字体设置
{
    ui->textEdit->setCurrentFont(f);
    ui->textEdit->setFocus();
}

void Chat::on_fontsizecomboBox_currentIndexChanged(QString size)
{
    ui->textEdit->setFontPointSize(size.toDouble());
    ui->textEdit->setFocus();
}

void Chat::on_textbold_clicked(bool checked)
{
    if(checked)
        ui->textEdit->setFontWeight(QFont::Bold);
    else
        ui->textEdit->setFontWeight(QFont::Normal);
    ui->textEdit->setFocus();
}

void Chat::on_textitalic_clicked(bool checked)
{
    ui->textEdit->setFontItalic(checked);
    ui->textEdit->setFocus();
}

void Chat::on_save_clicked()//保存聊天记录
{
    if(ui->textBrowser->document()->isEmpty())
        QMessageBox::warning(0,tr("警告"),tr("聊天记录为空，无法保存！"),QMessageBox::Ok);
    else
    {
        //获得文件名
        QString fileName = QFileDialog::getSaveFileName(this,tr("保存聊天记录"),tr("聊天记录"),tr("文本(*.txt);;All File(*.*)"));
        if(!fileName.isEmpty())
            saveFile(fileName);
    }
}

void Chat::on_clear_clicked()//清空聊天记录
{
    ui->textBrowser->clear();
}

bool Chat::saveFile(const QString &fileName)//保存文件
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))

    {
        QMessageBox::warning(this,tr("保存文件"),
            tr("无法保存文件 %1:\n %2").arg(fileName)
            .arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    out << ui->textBrowser->toPlainText();

    return true;
}

void Chat::on_textUnderline_clicked(bool checked)
{
    ui->textEdit->setFontUnderline(checked);
    ui->textEdit->setFocus();
}

void Chat::on_textcolor_clicked()
{
    m_color = QColorDialog::getColor(m_color,this);
    if(m_color.isValid())
    {
        ui->textEdit->setTextColor(m_color);
        ui->textEdit->setFocus();
    }
}

void Chat::on_send_clicked()
{
    sendMessage();

    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));

    if (!m_canSend)
    {
        ui->textBrowser->setTextColor(Qt::red);
        ui->textBrowser->append("[ " + QString("系统") +" ] "+ time);
        ui->textBrowser->append("对方离线, 消息发送失败...");
    }
    else 
    {
        ui->textBrowser->setTextColor(Qt::blue);
        ui->textBrowser->append("[ " + QString("本人") +" ] "+ time);
        ui->textBrowser->append(m_message);
    }
}

QString Chat::getUserAccount() const
{
    return m_targAccount;
}

void Chat::closeEvent( QCloseEvent * )
{
    if (!m_passiveClose)
    {
        sendExit();
    }

    emit exitTheChat();
    LogManager::getSingleton().logDebug("发送关闭消息, 聊天界面即将关闭");
}

void Chat::requestMessage( ChatDataType* cDType )
{

    ///< 将基类指针转换为聊天信息实体指针
    ChatMessageDataType* chatMDType = dynamic_cast<ChatMessageDataType*>(cDType);

    ///< 判断是否发生转换异常
    if (NULL == chatMDType)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->textBrowser->setTextColor(Qt::blue);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
    ui->textBrowser->append("[ " + m_targAccount +" ] "+ time);
    ui->textBrowser->append(chatMDType->getMessage());

    delete cDType;
    cDType = NULL;
}

void Chat::requestFileName( ChatDataType* cDType )
{

}

void Chat::requestExit()
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->textBrowser->setTextColor(Qt::red);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
    ui->textBrowser->append("[ " + m_targAccount +" ] "+ time);
    ui->textBrowser->append("对方已经离线!");
    m_canSend = false;
}

void Chat::sendUDPMessage( ChatDataType* cDType )
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);

    cDType->getData(out);

    m_chatSocket->writeDatagram(data,data.length(),QHostAddress::QHostAddress(m_targIp), m_targPort);

    delete cDType;
    cDType = NULL;
}

void Chat::sendExit()
{
    ///< 创建退出实体
    ChatDataType* cDType = m_cDTypeFactory.createChatMessageType(CM_EXIT);

    ///< 将基类指针转换为消息实体指针
    ChatExitType* cEType = dynamic_cast<ChatExitType*>(cDType);

    ///< 判断是否发生转换异常
    if (NULL == cEType)
    {
        LogManager::getSingleton().logAlert("类型转换时出现错误, 产生空指针异常");
        assert(false);
    }

    sendUDPMessage(cDType);
}
