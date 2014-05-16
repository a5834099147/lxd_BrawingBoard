#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QWidget>

#include "shape.h"
#include "paintwidget.h"
#include "userwidget.h"
#include "clientsocket.h"

#define true 1
#define false 0

class QMenu;
class QAction;
class QComboBox;
class QSpinBox;
class Login;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void changeCurrentShape(Shape::Code newShape);
    void changePenColour(Shape::Colour newPenColour);
    void changePenStyle(Shape::PenStyle newPenStyle);
    void changeBrushStyle(Shape::BrushStyle newBrushStyle);
    void changeBrushColour(Shape::Colour newBurshColour);
    void writeFile(const QString& fileName);
    void readFile(const QString& fileName);
    void changeUsbState(QString usbState);

private slots:
    void drawLineActionTriggered();
    void drawRectActionTriggered();
    void drawRoundActionTriggered();
    void drawRoundRectActionTriggered();
    void drawPointsActionTriggered();

    void colourPenRedActionTriggered();
    void colourPenGreenActionTriggered();
    void colourPenBlueActionTriggered();
    void colourPenBlackActionTriggered();
    void colourPenNoneActionTriggered();
    void colourPenCustomActionTriggered();

    void colourBrushRedActionTriggered();
    void colourBrushGreenActionTriggered();
    void colourBrushBlueActionTriggered();
    void colourBrushBlackActionTriggered();
    void colourBrushNoneActionTriggered();
    void colourBrushCustomActionTriggered();

    void findPenStyleChanged(int style);
    void findBrushStyleChanged(int style);

    void on_openAction_triggered();
    void on_saveAsAction_triggered();
    void on_saveAction_triggered();
    void on_newAction_triggered();
    void dateIsChanged();

    void sand_login(std::string account, std::string password);
    void sand_register(std::string account, std::string password, std::string userName, std::string pinYin);

protected:
    bool winEvent(MSG *message, long *result);

private:
    void CreatToolsAction();
    void CreatFileAction();
    void CreatPenColourAction();
    void CreatBrushColourAction();
    void CreatPenStyle();
    void CreatBrushStyle();
    void CreatConnect();
    void CreatMenus();  
    void CreatUserDockWidget();
    void CreatSocket();
    void LoginApp();
    void closeEvent(QCloseEvent *event);
    QString strippendName(const QString& fullFileName);
    void setCurrentFile(const QString &fileName);
    void setWindowTitleState(QString showName = NULL);
    bool okToContinue();
    char FirstDriveFromMask(ULONG unitmask);

private:
    /* Tools Actions*/
    QAction* drawLineAction;
    QAction* drawRectAction;
    QAction* drawRoundAction;
    QAction* drawRoundRectAction;
    QAction* drawPointsAction;

    /* PenColour Actions*/
    QAction* colourPenRed;
    QAction* colourPenGreen;
    QAction* colourPenBlue;
    QAction* colourPenBlack;
    QAction* colourPenNone;
    QAction* colourPenCustom;

    /* BrushColour Actions*/
    QAction* colourBrushRed;
    QAction* colourBrushGreen;
    QAction* colourBrushBlue;
    QAction* colourBrushBlack;
    QAction* colourBrushNone;
    QAction* colourBrushCustom;

    /* 文件操作*/
    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;

    QComboBox* penStyle;
    QComboBox* brushStyle;
    QSpinBox* penRoughNess;

    QMenu* toolsMenu;
    QMenu* penColourMenu;
    QMenu* fileMenu;

    QToolBar *penColorBar;
    QToolBar *brushColorBar;
    QToolBar *Toolsbar;

    PaintWidget *paintWidget;
    QString curFile;

    UserWidget *userWidget_;
    QDockWidget* userDockWidget_;

    bool dataChange;
    ClientSocket* socket_;
    Login* login_;
};

#endif // MAINWINDOW_H
