#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "clientsocket.h"

class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class Register;

class Login : public QDialog
{
	Q_OBJECT

public:
	Login(/*QWidget *parent*/);
	~Login();

signals:
	void sand_login(std::string account, std::string password);
	void sand_register(std::string account, std::string password, std::string userName, std::string pinYin);

	void recive_login(bool result);
	void recive_register(bool result);

private slots:
	void on_register_clicked();
	void on_loginB_clicked();
	void on_login_recive(bool result);

private:
	QLabel* accountL_;
	QLabel* passwordL_;

	QLineEdit* accountE_;
	QLineEdit* passwordE_;

	QPushButton* loginB_;
	QPushButton* registerB_;
	QPushButton* cancelB_;

	QVBoxLayout* layout_;
	Register* register_;

private:
	void Initialize();
	void createSlots();

};

#endif // LOGIN_H
