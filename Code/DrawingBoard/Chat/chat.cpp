#include "chat.h"
#include "ui_chat.h"
#include "common.h"
#include "ChatDataType.h"
#include "LogManager.h"

#include <cassert>


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

    ui->label->setText(tr("��%1������   �Է�IP:%2").arg(m_targAccount).arg(m_targIp));
    setWindowTitle(QString("�� %1 ������...").arg(m_targAccount));

    //UDP����
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
        if(event->type() == QEvent::KeyPress)//���¼���ĳ��
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);
            if(k->key() == Qt::Key_Return)//�س���
            {
                on_send_clicked();
                return true;
            }
        }
    }
    return QWidget::eventFilter(target,event);
}

QString Chat::getIP()  //��ȡip��ַ
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
        ///< ʹ�� IPv4��ַ
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
            return address.toString();
    }
    return 0;
}

void Chat::processPendingDatagrams()   //��������UDP
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
                LogManager::getSingleton().logDebug("����Ի����յ������������ݵ���Ϣ, �����������ݴ���������");
                requestMessage(chatDataType);
                break;
            }
        case CM_EXIT:
            {
                LogManager::getSingleton().logDebug("����Ի����յ������Ի���Ϣ, ���ɽ���ͨ����������");
                requestExit();
                break;
            }
        case  CM_FILENAME:
            {
                LogManager::getSingleton().logDebug("����Ի����յ��ļ�������Ϣ, �����ļ�����������");
                requestFileName(chatDataType);
                break;
            }
        default:
            {
                LogManager::getSingleton().logDebug("����Ի����յ��Ƿ���Ϣ, �޷�����");
                assert(false);
                break;
            }
        }
    }
}

QString Chat::getMessage()  //���Ҫ���͵���Ϣ
{
    QString msg = ui->textEdit->toHtml();
    ui->textEdit->clear();
    ui->textEdit->setFocus();
    return msg;
}

void Chat::sendMessage()  //������Ϣ
{
    if(ui->textEdit->toPlainText() == "")
    {
        QMessageBox::warning(0,tr("����"),tr("�������ݲ���Ϊ��"),QMessageBox::Ok);
        return;
    }

    if (!m_canSend)
    {
        QMessageBox::warning(NULL, tr("����"), tr("�Է��Ѿ�����, ������Ϣʧ��"), QMessageBox::Ok);
        return;
    }

    ///< ������Ϣʵ��
    ChatDataType* cDType = m_cDTypeFactory.createChatMessageType(CM_MESSAGE);

    ///< ������ָ��ת��Ϊ��Ϣʵ��ָ��
    ChatMessageDataType* cMDType = dynamic_cast<ChatMessageDataType*>(cDType);

    ///< �ж��Ƿ���ת���쳣
    if (NULL == cMDType)
    {
        LogManager::getSingleton().logAlert("����ת��ʱ���ִ���, ������ָ���쳣");
        assert(false);
    }

    m_message = getMessage();
    cMDType->setMessage(m_message);

    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());

    sendUDPMessage(cDType);
}

void Chat::currentFormatChanged(const QTextCharFormat &format)
{//���༭���������ʽ�ı�ʱ�������ò���״̬Ҳ��֮�ı�
    ui->fontComboBox->setCurrentFont(format.font());

    if(format.fontPointSize()<9)  //��������С������Ϊ������С������Ϊ9
    {
        ui->fontsizecomboBox->setCurrentIndex(3); //����ʾ12
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

void Chat::on_fontComboBox_currentFontChanged(QFont f)//��������
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

void Chat::on_save_clicked()//���������¼
{
    if(ui->textBrowser->document()->isEmpty())
        QMessageBox::warning(0,tr("����"),tr("�����¼Ϊ�գ��޷����棡"),QMessageBox::Ok);
    else
    {
        //����ļ���
        QString fileName = QFileDialog::getSaveFileName(this,tr("���������¼"),tr("�����¼"),tr("�ı�(*.txt);;All File(*.*)"));
        if(!fileName.isEmpty())
            saveFile(fileName);
    }
}

void Chat::on_clear_clicked()//��������¼
{
    ui->textBrowser->clear();
}

bool Chat::saveFile(const QString &fileName)//�����ļ�
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))

    {
        QMessageBox::warning(this,tr("�����ļ�"),
            tr("�޷������ļ� %1:\n %2").arg(fileName)
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
        ui->textBrowser->append("[ " + QString("ϵͳ") +" ] "+ time);
        ui->textBrowser->append("�Է�����, ��Ϣ����ʧ��...");
    }
    else 
    {
        ui->textBrowser->setTextColor(Qt::blue);
        ui->textBrowser->append("[ " + QString("����") +" ] "+ time);
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
    LogManager::getSingleton().logDebug("���͹ر���Ϣ, ������漴���ر�");
}

void Chat::requestMessage( ChatDataType* cDType )
{

    ///< ������ָ��ת��Ϊ������Ϣʵ��ָ��
    ChatMessageDataType* chatMDType = dynamic_cast<ChatMessageDataType*>(cDType);

    ///< �ж��Ƿ���ת���쳣
    if (NULL == chatMDType)
    {
        LogManager::getSingleton().logAlert("����ת��ʱ���ִ���, ������ָ���쳣");
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
    ui->textBrowser->append("�Է��Ѿ�����!");
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
    ///< �����˳�ʵ��
    ChatDataType* cDType = m_cDTypeFactory.createChatMessageType(CM_EXIT);

    ///< ������ָ��ת��Ϊ��Ϣʵ��ָ��
    ChatExitType* cEType = dynamic_cast<ChatExitType*>(cDType);

    ///< �ж��Ƿ���ת���쳣
    if (NULL == cEType)
    {
        LogManager::getSingleton().logAlert("����ת��ʱ���ִ���, ������ָ���쳣");
        assert(false);
    }

    sendUDPMessage(cDType);
}
