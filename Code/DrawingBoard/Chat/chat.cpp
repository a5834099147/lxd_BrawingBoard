#include "chat.h"
#include "ui_chat.h"
#include <cassert>


chat::chat(QString targAccont, QString targIp, qint32 localPort, qint32 targPort)
    : ui(new Ui::chat),
    m_localPort(localPort),
    m_targPort(targPort),
    m_targAccount(targAccont),
    m_targIp(targIp)
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

    this->show();
}

chat::~chat()
{
    delete ui;
}

bool chat::eventFilter(QObject *target, QEvent *event)
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

QString chat::getIP()  //��ȡip��ַ
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

void chat::processPendingDatagrams()   //��������UDP
{
    while(m_chatSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_chatSocket->pendingDatagramSize());
        m_chatSocket->readDatagram(datagram.data(),datagram.size());
        QDataStream in(&datagram,QIODevice::ReadOnly);

        ///< �յ�����Ϣ
        QString message;
        ///< ����Ϣ�������Ϣ�ַ�����
        in >> message;

        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        ui->textBrowser->setTextColor(Qt::blue);
        ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
        ui->textBrowser->append("[ " + m_targAccount +" ] "+ time);
        ui->textBrowser->append(message);
    }
}

QString chat::getMessage()  //���Ҫ���͵���Ϣ
{
    QString msg = ui->textEdit->toHtml();
    qDebug()<<msg;
    ui->textEdit->clear();
    ui->textEdit->setFocus();
    return msg;
}

void chat::sendMessage()  //������Ϣ
{
    QByteArray data;
    QDataStream out(&data,QIODevice::WriteOnly);

    if(ui->textEdit->toPlainText() == "")
    {
        QMessageBox::warning(0,tr("����"),tr("�������ݲ���Ϊ��"),QMessageBox::Ok);
        return;
    }
    m_message = getMessage();
    out << m_message;

    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
    m_chatSocket->writeDatagram(data,data.length(),QHostAddress::QHostAddress(m_targIp), m_targPort);
}

void chat::currentFormatChanged(const QTextCharFormat &format)
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
    color = format.foreground().color();
}

void chat::on_fontComboBox_currentFontChanged(QFont f)//��������
{
    ui->textEdit->setCurrentFont(f);
    ui->textEdit->setFocus();
}

void chat::on_fontsizecomboBox_currentIndexChanged(QString size)
{
    ui->textEdit->setFontPointSize(size.toDouble());
    ui->textEdit->setFocus();
}

void chat::on_textbold_clicked(bool checked)
{
    if(checked)
        ui->textEdit->setFontWeight(QFont::Bold);
    else
        ui->textEdit->setFontWeight(QFont::Normal);
    ui->textEdit->setFocus();
}

void chat::on_textitalic_clicked(bool checked)
{
    ui->textEdit->setFontItalic(checked);
    ui->textEdit->setFocus();
}

void chat::on_save_clicked()//���������¼
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

void chat::on_clear_clicked()//��������¼
{
    ui->textBrowser->clear();
}

bool chat::saveFile(const QString &fileName)//�����ļ�
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

void chat::on_textUnderline_clicked(bool checked)
{
    ui->textEdit->setFontUnderline(checked);
    ui->textEdit->setFocus();
}

void chat::on_textcolor_clicked()
{
    color = QColorDialog::getColor(color,this);
    if(color.isValid())
    {
        ui->textEdit->setTextColor(color);
        ui->textEdit->setFocus();
    }
}

void chat::on_send_clicked()
{
    sendMessage();
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    ui->textBrowser->setTextColor(Qt::blue);
    ui->textBrowser->setCurrentFont(QFont("Times New Roman",12));
    ui->textBrowser->append("[ " + QString("����") +" ] "+ time);
    ui->textBrowser->append(m_message);
}
