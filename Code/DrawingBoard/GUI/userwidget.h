#ifndef UserWidget_H
#define UserWidget_H

#include <QWidget>
#include <vector>
#include <windows.h>
#include <dbt.h>
#include <QtDBus/QDBusInterface>

#define DBUS_HAL_SERVICE		"org.freedesktop.Hal"
#define DBUS_HAL_PATH              "/org/freedesktop/Hal/Manager"  
#define DBUS_HAL_INTERFACE         "org.freedesktop.Hal.Manager"  
#define DBUS_HAL_DEVICE_PATH       "/org/freedesktop/Hal/devices"  
#define DBUS_HAL_DEVICE_INTERFACE  "org.freedesktop.Hal.Device"

class QLabel;
class QLineEdit;
class QTableWidget;
class QVBoxLayout;
class QPushButton;

typedef struct _user_{
    std::string account;
    std::string userName;
    std::string userPinyin;
    bool on_line;
    int rowId_;
}User;

class UserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserWidget(QWidget *parent = 0);

signals:

public slots:
    void changeUsbState(QString usbState);
    void insertUserList(std::string account, std::string userName,
         std::string userPinyin, bool on_line);
    void contentChanges(std::string account, std::string userName,
        std::string userPinyin, bool on_line, bool isUpdate);

private slots:  
    void on_exportB_clicked();
    void on_importB_clicked();
    void on_inquiryE_changed();

private:
    void InitInterface(void);
    void CreateComponent(void);
    void CreateLoyout(void);
    void CreateSlots(void);
    void tableContentChange(User user);

    void SettingInfoList(void);
    /* void list_Change(std::vector<int> user_v); */

private:  
    QLineEdit *inquiryE_;
    QLabel* inquiryL_;
    QLabel* usbState_;

    QTableWidget *infoList_;

    QPushButton *importB_;
    QPushButton *exportB_;  

    QVBoxLayout *layout_;   
    std::vector<User> user_;
};

#endif // UserWidget_H
