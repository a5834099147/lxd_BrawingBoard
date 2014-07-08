#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>

class QLineEdit;
class QLabel;
class QPushButton;
class QVBoxLayout;

class Register : public QDialog
{
    Q_OBJECT

public:
    Register(QWidget *parent);
    ~Register();

signals:
    void sand_register(std::string account, std::string password, std::string userName, std::string yinYin);
    void recive_register(bool result);

    private slots:
        void on_registerB_clicked();
        void on_register_recive(bool);

private:
    QLabel* accountL_;
    QLabel* userNameL_;
    QLabel* passwordL_;

    QLineEdit* accountE_;
    QLineEdit* userNameE_;
    QLineEdit* passwordE_;	

    QPushButton* registerB_;	
    QPushButton* cancelB_;

    QVBoxLayout* layout_;	

    void Initialize();
    void createSlots();
};

#endif // REGISTER_H
