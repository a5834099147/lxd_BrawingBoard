#include "login.h"
#include "register.h"

#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

Login::Login(/*QWidget *parent*/)
	: QDialog(/*parent*/)
{
	Initialize();
}

Login::~Login()
{
}

void Login::Initialize()
{
	accountL_ = new QLabel(tr("用户名:"));
	passwordL_ = new QLabel(tr("密码"));

	accountE_ = new QLineEdit;
	passwordE_ = new QLineEdit;

	loginB_ = new QPushButton(tr("登录"));
	registerB_ = new QPushButton(tr("注册"));
	cancelB_ = new QPushButton(tr("退出"));

	QFormLayout* upLayout = new QFormLayout;
	upLayout->addRow(accountL_, accountE_);
	upLayout->addRow(passwordL_, passwordE_);

	QHBoxLayout* downLayout = new QHBoxLayout;
	downLayout->addStretch();
	downLayout->addWidget(loginB_);
	downLayout->addWidget(registerB_);
	downLayout->addWidget(cancelB_);

	layout_ = new QVBoxLayout;
	layout_->addLayout(upLayout);
	layout_->addLayout(downLayout);	

	register_ = new Register(this);

	setLayout(layout_);
	this->setFixedHeight(this->sizeHint().height());
	this->setWindowTitle(tr("登录"));

	createSlots();
}

void Login::createSlots()
{
	connect(registerB_, SIGNAL(clicked()),
		this, SLOT(on_register_clicked()));
	connect(cancelB_, SIGNAL(clicked()),
		this, SLOT(reject()));
	connect(loginB_, SIGNAL(clicked()),
		this, SLOT(on_loginB_clicked()));

	connect(this, SIGNAL(recive_login(bool)),
		this, SLOT(on_login_recive(bool)));
	connect(register_, SIGNAL(sand_register(std::string, std::string, std::string, std::string)),
		this, SIGNAL(sand_register(std::string, std::string, std::string, std::string)));
	connect(this, SIGNAL(recive_register(bool)),
		register_, SIGNAL(recive_register(bool)));
}

void Login::on_register_clicked()
{
	register_->exec();	
}

void Login::on_login_recive(bool result)
{
	if (result)
	{
		accept();
	}
	else 
	{
		if (QMessageBox::critical(NULL, "登陆错误", "您输入的账号过密码无法通过验证，请重新输入",
											QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
		{
			reject();
		}
	}
}

void Login::on_loginB_clicked()
{
	std::string account = accountE_->text().toStdString();
	std::string password = passwordE_->text().toStdString();
	if (account.empty() || password.empty())
	{
		if (QMessageBox::critical(NULL, "登录", "登录信息填写不完整, 请重试",
			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
		{
			reject();
		}
	}
	else
	{
		emit sand_login(account, password);
	}
}


