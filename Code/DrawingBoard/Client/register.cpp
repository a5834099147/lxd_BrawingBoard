#include "register.h"
#include "mychitoletter.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

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
    accountL_ = new QLabel(tr("帐号"));
    passwordL_ = new QLabel(tr("密码"));
    userNameL_ = new QLabel(tr("昵称"));

    accountE_ = new QLineEdit;
    passwordE_ = new QLineEdit;	
    userNameE_ = new QLineEdit;	

    registerB_ = new QPushButton(tr("注册"));
    cancelB_ = new QPushButton(tr("取消"));

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
    this->setWindowTitle(tr("注册"));

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
        if (QMessageBox::critical(NULL, "注册", "您输入的账号已被占用，请重新输入",
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
        if (QMessageBox::critical(NULL, "注册错误", "用户信息没有填写完整, 出现错误, 请重试",
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
