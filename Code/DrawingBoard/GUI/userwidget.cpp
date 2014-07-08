#include "userwidget.h"
#include "libxl.h"
#include "MyChiToLetter.h"
#include "LogManager.h"
#include <boost/lexical_cast.hpp>

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <string>
#include <QLabel>
#include <QMessageBox>

///< UTF-8编码设置, 可以用来显示中文乱码问题, 前提是文档的编码格式为UTF-8
#pragma execution_character_set("utf-8")

using namespace libxl;

UserWidget::UserWidget(QWidget *parent) :
QWidget(parent)
{
    InitInterface();
}

void UserWidget::CreateComponent()
{
    inquiryE_ = new QLineEdit;
    inquiryL_ = new QLabel(tr("查询"));

    importB_ = new QPushButton(tr("导入"));
    exportB_ = new QPushButton(tr("导出"));

    infoList_ = new QTableWidget;  

    usbState_ = new QLabel;  
}

void UserWidget::InitInterface()
{
    CreateComponent();
    CreateLoyout();
    CreateSlots();
    SettingInfoList(); 
}

void UserWidget::insertUserList(std::string account, std::string userName, 
                                std::string userPinyin, bool on_line)
{
    ///< 列表在末行增加一行
    infoList_->insertRow(infoList_->rowCount());
    
    User user_temp;
    user_temp.account = account;
    user_temp.userName = userName;
    user_temp.userPinyin = userPinyin;
    user_temp.on_line = on_line;
    user_temp.rowId_ = infoList_->rowCount() - 1;

    ///< 更改列表内容
    tableContentChange(user_temp);

    user_.push_back(user_temp);
    LogManager::getSingleton().logDebug("添加用户信息");
}

void UserWidget::CreateLoyout()
{
    QHBoxLayout *upLayout  = new QHBoxLayout;
    upLayout->addWidget(inquiryL_);
    upLayout->addWidget(inquiryE_);  

    QHBoxLayout *downLayout = new QHBoxLayout;
    downLayout->addWidget(importB_);
    downLayout->addStretch();
    downLayout->addWidget(exportB_);

    layout_ = new QVBoxLayout(this);
    layout_->addLayout(upLayout);
    layout_->addWidget(infoList_);
    layout_->addLayout(downLayout);
    layout_->addWidget(usbState_);

    setLayout(layout_);
    setWindowTitle(tr("用户名单"));
    setFixedWidth(sizeHint().width());
}

void UserWidget::SettingInfoList()
{  
    infoList_->setColumnCount(3);

    QStringList header;
    header << "帐号" << "用户名" << "在线";
    infoList_->setHorizontalHeaderLabels(header);

    infoList_->setColumnWidth(0, 70);
    infoList_->setColumnWidth(1, 70);
    infoList_->setColumnWidth(2, 60);
    infoList_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    infoList_->setSelectionBehavior(QAbstractItemView::SelectRows);

    /*list_Change(odb_user::export());*/
    LogManager::getSingleton().logDebug("创建用户列表");
}



void UserWidget::CreateSlots()
{
    connect(exportB_, SIGNAL(clicked()),
        this, SLOT(on_exportB_clicked()));
    connect(importB_, SIGNAL(clicked()),
        this, SLOT(on_importB_clicked()));
    connect(inquiryE_, SIGNAL(textEdited(const QString &)),
        this, SLOT(on_inquiryE_changed()));
    connect(infoList_, SIGNAL(cellDoubleClicked(int, int)),
            this, SLOT(on_cellDoubleClicked(int, int)));
}

void UserWidget::on_inquiryE_changed()
{
    LogManager::getSingleton().logDebug("模糊查询数据库");
    string str = inquiryE_->text().toStdString();
    /*list_Change(odb_user::fuzzySearch(str));*/
}

void UserWidget::on_exportB_clicked()
{	
    // 	vector<User> user_v = odb_user::export();
    // 	Book* book = xlCreateBook();
    // 	if (book)
    // 	{
    // 		Sheet* sheet = book->addSheet("Sheet1");
    // 		if (sheet)
    // 		{
    // 			QString fileName = QFileDialog::getSaveFileName(this, 
    // 				tr("保存需要导出文件的xls"), ".",
    // 				tr("EXCEL文件 (*.xls)"));
    // 
    // 			Log4Cpp::info((QString("导出数据库文件到EXCLE文件中 ... ") += fileName).toStdString());
    // 
    // 			sheet->writeStr(0, 0, "帐号");
    // 			sheet->writeStr(0, 1, "密码");
    // 			sheet->writeStr(0, 2, "昵称");
    // 
    // 			for (int i = 0; i < user_v.size(); ++i)
    // 			{				
    // 				sheet->writeStr(i + 1, 0, user_v.at(i).Account().c_str());
    // 				sheet->writeStr(i + 1, 1, user_v.at(i).Password().c_str());
    // 				sheet->writeStr(i + 1, 2, user_v.at(i).UserName().c_str());
    // 			}
    // 
    // 			if (book->save(fileName.toStdString().c_str()));
    // 			Log4Cpp::info((QString("导出数据库文件到EXCLE文件成功") += fileName).toStdString());
    // 		}
    // 		book->release();
    // 	}
}

void UserWidget::on_importB_clicked()
{
#if 0
    Book* book = xlCreateBook();
    if (book)
    {
        QString fileName = QFileDialog::getOpenFileName(this, 
            tr("打开需要导入文件的xls"), ".",
            tr("EXCEL文件 (*.xls)"));
        LogManager::getSingleton().logDebug((QString("读取EXCLE文件到数据库中 ... ") += fileName).toStdString());
        if (book->load(fileName.toStdString().c_str()))
        {
            Sheet* sheet = book->getSheet(0);
            if (sheet)
            {
                MyChiToLetter chiToLetter;
                chiToLetter.m_blnFirstBig = true;
                chiToLetter.m_blnAllBiG = false;
                chiToLetter.m_blnSimaple = true;
                chiToLetter.m_LetterEnd = false;

                int row = 1;
                const char* s_1 = sheet->readStr(row, 0);
                const char* s_2;
                const char* s_3;
                while (s_1)
                {
                    s_2 = sheet->readStr(row, 1);
                    s_3 = sheet->readStr(row, 2);					

                    if (!s_2 || !s_3)
                    {
                        break;
                    }

                    std::string account(s_1), password(s_2), userName(s_3);

                    string userNamePinyin = chiToLetter.GetLetter(userName);	

                    /*odb_user::registerTheUser(account, password, userName, userNamePinyin);*/
                    ++row;
                    s_1 = sheet->readStr(row, 0);
                }
            }
            LogManager::getSingleton().logDebug((QString("读取EXCLE文件到数据库成功 ") += fileName).toStdString());
        }
        book->release();
    }
    /*list_Change(odb_user::export());*/
#endif
}

void UserWidget::changeUsbState(QString usbState)
{
    usbState_->setText(usbState);
}

void UserWidget::contentChanges( std::string account, std::string userName, std::string userPinyin, bool on_line, bool isUpdate )
{
    if (isUpdate)
    {
        for (int i = 0; i < user_.size(); ++i)
        {
            if (user_.at(i).account == account)
            {
                LogManager::getSingleton().logDebug("更新用户: " + account + "的在线状态为: " + boost::lexical_cast<std::string>(on_line));
                user_.at(i).on_line = on_line;
                tableContentChange(user_.at(i));
                return;
            }
        }
    }

    insertUserList(account, userName, userPinyin, on_line);
}

void UserWidget::tableContentChange( User user )
{
    infoList_->setItem(user.rowId_, 0, new QTableWidgetItem(QString(user.account.c_str())));
    infoList_->setItem(user.rowId_, 1, new QTableWidgetItem(QString(user.userName.c_str())));
    infoList_->setItem(user.rowId_, 2, new QTableWidgetItem(QString(user.on_line ? "是" : "否")));
}

void UserWidget::on_cellDoubleClicked( int row, int colume )
{
    QTableWidgetItem* userStateItem = infoList_->item(row, 2);
    QString userState = userStateItem->text();

    if ("否" == userState)
    {
        LogManager::getSingleton().logDebug("用户选择了非在线用户期望聊天, 被系统拒绝");
        QMessageBox::information(NULL, "失败", "您所选择的用户不在线, 不能进行聊天",
                                 QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    QTableWidgetItem* userAccountItem = infoList_->item(row, 0);
    QString userAccount = userAccountItem->text();

    LogManager::getSingleton().logDebug("用户选择了与" + userAccount.toStdString() + "交谈, 弹出对话框有用户确认");
    
    if (QMessageBox::information(NULL, "请确认", "您是否需要向用户:" + userAccount + "发起交谈, 如若需要请确定",
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) == QMessageBox::No)
    {
        LogManager::getSingleton().logDebug("用户取消了同" + userAccount.toStdString() + "的交谈请求");
    }
    else 
    {
        LogManager::getSingleton().logDebug("用户确认了同" + userAccount.toStdString() + "的交谈请求");
        emit chatRequest(userAccount);
        //QMessageBox::information(NULL, "提醒", "系统已经发送了您的会话请求, 请稍后...");
    }
}
