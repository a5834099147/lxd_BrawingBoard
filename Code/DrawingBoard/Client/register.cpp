#include "register.h"
#include "mychitoletter.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

Register::Register(QWidget *parent)
    : QDialog(parent)
{
    Initialize();
}

Register::~Register()
{

}

void Register::Initialize()
{
    accountL_ = new QLabel(tr("�ʺ�"));
    passwordL_ = new QLabel(tr("����"));
    userNameL_ = new QLabel(tr("�ǳ�"));

    accountE_ = new QLineEdit;
    passwordE_ = new QLineEdit;	
    userNameE_ = new QLineEdit;	

    registerB_ = new QPushButton(tr("ע��"));
    cancelB_ = new QPushButton(tr("ȡ��"));

    passwordE_->setEchoMode(QLineEdit::Password);

    QFormLayout* upLayout = new QFormLayout();
    upLayout->addRow(accountL_, accountE_);
    upLayout->addRow(passwordL_, passwordE_);
    upLayout->addRow(userNameL_, userNameE_);	

    QHBoxLayout* downLayout = new QHBoxLayout();
    downLayout->addStretch();
    downLayout->addWidget(registerB_);
    downLayout->addWidget(cancelB_);

    layout_ = new QVBoxLayout(this);
    layout_->addLayout(upLayout);
    layout_->addLayout(downLayout);

    this->setLayout(layout_);
    this->setFixedHeight(this->sizeHint().height());
    this->setWindowTitle(tr("ע��"));

    createSlots();
}

void Register::createSlots()
{
    connect(registerB_, SIGNAL(clicked()),
        this, SLOT(on_registerB_clicked()));
    connect(cancelB_, SIGNAL(clicked()),
        this, SLOT(reject()));

    connect(this, SIGNAL(recive_register(bool)),
        this, SLOT(on_register_recive(bool)));
}

void Register::on_register_recive(bool result)
{
    if (result)
    {
        accept();
    }
    else 
    {
        if (QMessageBox::critical(NULL, "ע��", "��������˺ź���������ѱ�ռ�ã�����������",
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
        {
            reject();
        }
    }
}

void Register::on_registerB_clicked()
{
    std::string account = accountE_->text().toStdString();
    std::string password = passwordE_->text().toStdString();
    std::string userName = userNameE_->text().toStdString();

    if (account.empty() || password.empty() || userName.empty())
    {
        if (QMessageBox::critical(NULL, "ע�����", "�û���Ϣû����д����, ���ִ���, ������",
            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
        {
            reject();
        }
    }
    else 
    {
        MyChiToLetter chiToLetter;
        chiToLetter.m_blnFirstBig = true;
        chiToLetter.m_blnAllBiG = false;
        chiToLetter.m_blnSimaple = true;
        chiToLetter.m_LetterEnd = false;
        string userNamePinyin = chiToLetter.GetLetter(userName);	

        emit sand_register(account, password, userName, userNamePinyin);
    }
}
