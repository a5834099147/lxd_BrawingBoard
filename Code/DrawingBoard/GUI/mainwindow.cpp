#include "mainwindow.h"
#include "login.h"

#include <QtGui/QLabel>
#include <QtGui/QMenu>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>
#include <QtGui/QAction>
#include <QtGui/QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent), dataChange(false)
{
    QLabel *statusMsg = new QLabel;
    statusBar()->addWidget(statusMsg);

    paintWidget = new PaintWidget(this);
    LogManager::getSingleton().logDebug("创建画板视图成功");
    setCentralWidget(paintWidget);
    LogManager::getSingleton().logDebug("设置画板为中心区域成功");
    CreatToolsAction();
    LogManager::getSingleton().logDebug("创建 TOOLS 控件及 TOOLSBAR成功");
    CreatPenColourAction();
    LogManager::getSingleton().logDebug("创建 PENCOLOR 控件及 PENCOLORBAR 成功");
    CreatBrushColourAction();
    LogManager::getSingleton().logDebug("创建 BRUSHCOLR 控件及 BRUSHCOLORBAR 成功");
    CreatPenStyle();
    LogManager::getSingleton().logDebug("创建 PENSTYLE 控件成功");
    CreatBrushStyle();
    LogManager::getSingleton().logDebug("创建 BRUSHSTYPE 控件成功");
    CreatFileAction();
    LogManager::getSingleton().logDebug("创建 FILE 控件成功");
    CreatMenus();
    LogManager::getSingleton().logDebug("创建菜单成功");
    CreatUserDockWidget();
    LogManager::getSingleton().logDebug("创建 右边停靠栏成功");
    CreatConnect();
    LogManager::getSingleton().logDebug("绑定信号/槽成功");
    setCurrentFile(NULL);
    LogManager::getSingleton().logDebug("设置当前文件");
    CreatSocket();
    LoginApp();
}

MainWindow::~MainWindow()
{
    LogManager::getSingleton().logDebug("退出画板程序");
}

void MainWindow::CreatSocket()
{
    if (!socket_->connectToServer("127.0.0.1", 7733))
    {
        LogManager::getSingleton().logError("无法与服务器相连接");
        QMessageBox::critical(this, "致命错误", "程序无法与服务器连接",
                              QMessageBox::Yes, QMessageBox::Yes);
        exit(0);
    }
}

void MainWindow::LoginApp()
{	
    LogManager::getSingleton().logDebug("画板程序登录中...");
    if (login_->exec() == false)
    {
        LogManager::getSingleton().logError("程序登录失败");
        exit(0);
    }
    else
    {
        LogManager::getSingleton().logDebug("画板程序登录成功");
    }
}

void MainWindow::CreatToolsAction()
{
    Toolsbar = this->addToolBar(tr("形状"));
    this->addToolBar(Qt::LeftToolBarArea, Toolsbar);
    QActionGroup *group = new QActionGroup(Toolsbar);  

    drawLineAction = new QAction("Line", Toolsbar);
    drawLineAction->setIcon(QIcon(":/直线.png"));
    drawLineAction->setToolTip(tr("直线"));
    drawLineAction->setStatusTip(tr("绘制直线"));
    drawLineAction->setCheckable(true);
    drawLineAction->setChecked(true);
    group->addAction(drawLineAction);
    Toolsbar->addAction(drawLineAction);

    drawRectAction = new QAction("RectAngle", Toolsbar);
    drawRectAction->setIcon(QIcon(":/矩形.png"));
    drawRectAction->setToolTip(tr("矩形"));
    drawRectAction->setStatusTip(tr("绘制矩形"));
    drawRectAction->setCheckable(true);
    group->addAction(drawRectAction);
    Toolsbar->addAction(drawRectAction);

    drawRoundAction = new QAction("Round", Toolsbar);
    drawRoundAction->setIcon(QIcon(":/圆.png"));
    drawRoundAction->setToolTip(tr("圆形"));
    drawRoundAction->setStatusTip(tr("绘制圆形"));
    drawRoundAction->setCheckable(true);
    group->addAction(drawRoundAction);
    Toolsbar->addAction(drawRoundAction);

    drawRoundRectAction = new QAction("圆角矩形", Toolsbar);
    drawRoundRectAction->setIcon(QIcon(":/圆角矩形.png"));
    drawRoundRectAction->setToolTip(tr("圆角矩形"));
    drawRoundRectAction->setStatusTip(tr("绘制圆角矩形"));
    drawRoundRectAction->setCheckable(true);
    group->addAction(drawRoundRectAction);
    Toolsbar->addAction(drawRoundRectAction);

    drawPointsAction = new QAction("画笔", Toolsbar);
    drawPointsAction->setIcon(QIcon(":/pen.png"));
    drawPointsAction->setToolTip(tr("画笔"));
    drawPointsAction->setStatusTip(tr("手绘线条"));
    drawPointsAction->setCheckable(true);
    group->addAction(drawPointsAction);
    Toolsbar->addAction(drawPointsAction);
}

void MainWindow::CreatPenColourAction()
{
    penColorBar = this->addToolBar(tr("画笔颜色"));
    QLabel* name = new QLabel(tr("线条颜色:"));
    penColorBar->addWidget(name);

    QActionGroup *group = new QActionGroup(penColorBar);

    colourPenBlack = new QAction("黑色", penColorBar);
    colourPenBlack->setToolTip(tr("黑色"));
    colourPenBlack->setStatusTip(tr("黑色的画笔"));
    colourPenBlack->setCheckable(true);
    colourPenBlack->setChecked(true);
    group->addAction(colourPenBlack);
    penColorBar->addAction(colourPenBlack);

    colourPenBlue = new QAction("蓝色", penColorBar);
    colourPenBlue->setToolTip(tr("蓝色"));
    colourPenBlue->setStatusTip(tr("蓝色的画笔"));
    colourPenBlue->setCheckable(true);
    group->addAction(colourPenBlue);
    penColorBar->addAction(colourPenBlue);

    colourPenGreen = new QAction("绿色", penColorBar);
    colourPenGreen->setToolTip(tr("绿色"));
    colourPenGreen->setStatusTip(tr("绿色的画笔"));
    colourPenGreen->setCheckable(true);
    group->addAction(colourPenGreen);
    penColorBar->addAction(colourPenGreen);

    colourPenRed = new QAction("红色", penColorBar);
    colourPenRed->setToolTip(tr("红色"));
    colourPenRed->setStatusTip(tr("红色的画笔"));
    colourPenRed->setCheckable(true);
    group->addAction(colourPenRed);
    penColorBar->addAction(colourPenRed);

    colourPenNone = new QAction("白色", penColorBar);
    colourPenNone->setToolTip(tr("白色"));
    colourPenNone->setStatusTip(tr("白色的画笔"));
    colourPenNone->setCheckable(true);
    group->addAction(colourPenNone);
    penColorBar->addAction(colourPenNone);

    colourPenCustom = new QAction("自定义颜色", penColorBar);
    colourPenCustom->setToolTip(tr("自定义颜色"));
    colourPenCustom->setStatusTip(tr("自定义颜色的画笔"));
    colourPenCustom->setCheckable(true);
    group->addAction(colourPenCustom);
    penColorBar->addAction(colourPenCustom);
}

void MainWindow::CreatFileAction()
{
    newAction = new QAction(tr("新建文件"), this);
    newAction->setStatusTip(tr("新建一个空白的画板"));
    newAction->setShortcut(tr("Ctrl+N"));

    openAction = new QAction(tr("打开文件"), this);
    openAction->setStatusTip(tr("打开一个画板文件"));
    openAction->setShortcut(tr("Ctrl+O"));

    saveAction = new QAction(tr("保存"), this);
    saveAction->setStatusTip(tr("保存画板"));
    saveAction->setShortcut(tr("Ctrl+S"));

    saveAsAction = new QAction(tr("另存为"), this);
    saveAsAction->setStatusTip(tr("将画板另存为"));
    saveAsAction->setShortcut(tr("Ctrl+Shift+S"));  	
}

void MainWindow::CreatBrushColourAction()
{
    brushColorBar = this->addToolBar(tr("画刷颜色"));
    QLabel* name = new QLabel(tr("画刷颜色:"));
    brushColorBar->addWidget(name);

    QActionGroup *group = new QActionGroup(brushColorBar);

    colourBrushBlack = new QAction("黑色", brushColorBar);
    colourBrushBlack->setToolTip(tr("黑色"));
    colourBrushBlack->setStatusTip(tr("黑色的画刷"));
    colourBrushBlack->setCheckable(true);
    group->addAction(colourBrushBlack);
    brushColorBar->addAction(colourBrushBlack);

    colourBrushBlue = new QAction("蓝色", brushColorBar);
    colourBrushBlue->setToolTip(tr("蓝色"));
    colourBrushBlue->setStatusTip(tr("蓝色的画刷"));
    colourBrushBlue->setCheckable(true);
    group->addAction(colourBrushBlue);
    brushColorBar->addAction(colourBrushBlue);

    colourBrushGreen = new QAction("绿色", brushColorBar);
    colourBrushGreen->setToolTip(tr("绿色"));
    colourBrushGreen->setStatusTip(tr("绿色的画刷"));
    colourBrushGreen->setCheckable(true);
    group->addAction(colourBrushGreen);
    brushColorBar->addAction(colourBrushGreen);

    colourBrushRed = new QAction("红色", brushColorBar);
    colourBrushRed->setToolTip(tr("红色"));
    colourBrushRed->setStatusTip(tr("红色的画刷"));
    colourBrushRed->setCheckable(true);
    group->addAction(colourBrushRed);
    brushColorBar->addAction(colourBrushRed);

    colourBrushNone = new QAction("白色", brushColorBar);
    colourBrushNone->setToolTip(tr("白色"));
    colourBrushNone->setStatusTip(tr("白色的画刷"));
    colourBrushNone->setCheckable(true);
    colourBrushNone->setChecked(true);
    group->addAction(colourBrushNone);
    brushColorBar->addAction(colourBrushNone);

    colourBrushCustom = new QAction("自定义颜色", brushColorBar);
    colourBrushCustom->setToolTip(tr("自定义颜色"));
    colourBrushCustom->setStatusTip(tr("自定义颜色的画刷"));
    colourBrushCustom->setCheckable(true);
    group->addAction(colourBrushCustom);
    brushColorBar->addAction(colourBrushCustom);
}

void MainWindow::CreatPenStyle()
{
    penStyle = new QComboBox();
    penRoughNess = new QSpinBox();
    QLabel* lineName = new QLabel(tr("线条:"));
    QLabel* roughnessName = new QLabel(tr("笔粗"));

    penStyle->addItem(tr("实线"));
    penStyle->addItem(tr("虚线"));
    penStyle->addItem(tr("点线"));
    penStyle->addItem(tr("单点虚线"));
    penStyle->addItem(tr("双点虚线"));
    penStyle->addItem(tr("没有线条"));

    penColorBar->addSeparator();
    penColorBar->addWidget(lineName);
    penColorBar->addWidget(penStyle);

    penRoughNess->setRange(1, 5);
    penColorBar->addSeparator();
    penColorBar->addWidget(roughnessName);
    penColorBar->addWidget(penRoughNess);
}

void MainWindow::CreatBrushStyle()
{
    brushStyle = new QComboBox();
    QLabel* name = new QLabel(tr("画刷:"));

    brushStyle->addItem(tr("纯色"));
    brushStyle->addItem(tr("点阵类型1"));
    brushStyle->addItem(tr("点阵类型2"));
    brushStyle->addItem(tr("点阵类型3"));
    brushStyle->addItem(tr("点阵类型4"));
    brushStyle->addItem(tr("点阵类型5"));
    brushStyle->addItem(tr("点阵类型6"));
    brushStyle->addItem(tr("点阵类型7"));
    brushStyle->addItem(tr("横线"));
    brushStyle->addItem(tr("纵线"));
    brushStyle->addItem(tr("正网状"));
    brushStyle->addItem(tr("左斜线"));
    brushStyle->addItem(tr("右斜线"));
    brushStyle->addItem(tr("斜网状"));
    brushStyle->addItem(tr("无画刷"));

    brushColorBar->addSeparator();
    brushColorBar->addWidget(name);
    brushColorBar->addWidget(brushStyle);
}

void MainWindow::CreatMenus()
{
    fileMenu = menuBar()->addMenu(tr("文件"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);

    toolsMenu = menuBar()->addMenu(tr("画笔"));
    toolsMenu->addAction(drawLineAction);
    toolsMenu->addAction(drawRectAction);
    toolsMenu->addAction(drawRoundAction);
    toolsMenu->addAction(drawRoundRectAction);
    toolsMenu->addAction(drawPointsAction);

    penColourMenu = menuBar()->addMenu(tr("画笔颜色"));
    penColourMenu->addAction(colourPenBlack);
    penColourMenu->addAction(colourPenBlue);
    penColourMenu->addAction(colourPenGreen);
    penColourMenu->addAction(colourPenRed);
    penColourMenu->addAction(colourPenNone);
}

void MainWindow::CreatUserDockWidget()
{
    userWidget_ = new UserWidget(this);

    userDockWidget_ = new QDockWidget(this);
    userDockWidget_->setWidget(userWidget_);

    addDockWidget(Qt::RightDockWidgetArea, userDockWidget_);

    login_ = new Login;
    socket_ = new ClientSocket(this);
}

void MainWindow::CreatConnect()
{
    connect(drawLineAction, SIGNAL(triggered()),
        this, SLOT(drawLineActionTriggered()));
    connect(drawRectAction, SIGNAL(triggered()),
        this, SLOT(drawRectActionTriggered()));
    connect(drawRoundAction, SIGNAL(triggered()),
        this, SLOT(drawRoundActionTriggered()));
    connect(drawRoundRectAction, SIGNAL(triggered()),
        this, SLOT(drawRoundRectActionTriggered()));
    connect(drawPointsAction, SIGNAL(triggered()),
        this, SLOT(drawPointsActionTriggered()));

    connect(this, SIGNAL(changeCurrentShape(Shape::Code)),
        paintWidget, SLOT(setCurrentShape(Shape::Code)));

    connect(colourPenBlack, SIGNAL(triggered()),
        this, SLOT(colourPenBlackActionTriggered()));
    connect(colourPenBlue, SIGNAL(triggered()),
        this, SLOT(colourPenBlueActionTriggered()));
    connect(colourPenGreen, SIGNAL(triggered()),
        this, SLOT(colourPenGreenActionTriggered()));
    connect(colourPenRed, SIGNAL(triggered()),
        this, SLOT(colourPenRedActionTriggered()));
    connect(colourPenNone, SIGNAL(triggered()),
        this, SLOT(colourPenNoneActionTriggered()));
    connect(colourPenCustom, SIGNAL(triggered()),
        this, SLOT(colourPenCustomActionTriggered()));

    connect(this, SIGNAL(changePenColour(Shape::Colour)),
        paintWidget, SLOT(setPenColour(Shape::Colour)));

    connect(colourBrushBlack, SIGNAL(triggered()),
        this, SLOT(colourBrushBlackActionTriggered()));
    connect(colourBrushBlue, SIGNAL(triggered()),
        this, SLOT(colourBrushBlueActionTriggered()));
    connect(colourBrushGreen, SIGNAL(triggered()),
        this, SLOT(colourBrushGreenActionTriggered()));
    connect(colourBrushRed, SIGNAL(triggered()),
        this, SLOT(colourBrushRedActionTriggered()));
    connect(colourBrushNone, SIGNAL(triggered()),
        this, SLOT(colourBrushNoneActionTriggered()));
    connect(colourBrushCustom, SIGNAL(triggered()),
        this, SLOT(colourBrushCustomActionTriggered()));

    connect(this, SIGNAL(changeBrushColour(Shape::Colour)),
        paintWidget, SLOT(setBrushColour(Shape::Colour)));

    connect(penRoughNess, SIGNAL(valueChanged(int)),
        paintWidget, SLOT(setPenRoughNess(int)));

    connect(penStyle, SIGNAL(activated(int)),
        this, SLOT(findPenStyleChanged(int)));
    connect(this, SIGNAL(changePenStyle(Shape::PenStyle)),
        paintWidget, SLOT(setPenStyle(Shape::PenStyle)));

    connect(brushStyle, SIGNAL(activated(int)),
        this, SLOT(findBrushStyleChanged(int)));
    connect(this, SIGNAL(changeBrushStyle(Shape::BrushStyle)),
        paintWidget, SLOT(setBrushStyle(Shape::BrushStyle)));

    connect(openAction, SIGNAL(triggered()),
        this, SLOT(on_openAction_triggered()));
    connect(saveAsAction, SIGNAL(triggered()),
        this, SLOT(on_saveAsAction_triggered()));
    connect(saveAction, SIGNAL(triggered()),
        this, SLOT(on_saveAction_triggered()));
    connect(newAction, SIGNAL(triggered()),
        this, SLOT(on_newAction_triggered()));

    connect(this, SIGNAL(writeFile(const QString&)),
        paintWidget, SLOT(writeFile(const QString&)));
    connect(this, SIGNAL(readFile(const QString&)),
        paintWidget, SLOT(readFile(const QString&)));
    connect(paintWidget, SIGNAL(dateIsChanged()),
        this, SLOT(dateIsChanged()));

    connect(this, SIGNAL(changeUsbState(QString)),
        userWidget_, SLOT(changeUsbState(QString)));

    /* 以下为网络部分*/
    connect(socket_, SIGNAL(recive_login(bool)),
        login_, SIGNAL(recive_login(bool)));
    connect(socket_, SIGNAL(recive_register(bool)),
        login_, SIGNAL(recive_register(bool)));
    connect(login_, SIGNAL(sand_login(std::string, std::string)),
        this, SLOT(sand_login(std::string, std::string)));
    connect(login_, SIGNAL(sand_register(std::string, std::string, std::string, std::string)),
        this, SLOT(sand_register(std::string, std::string, std::string, std::string))); 

    /* 用户列表下放*/
    connect(socket_, SIGNAL(recive_userList(std::string, std::string, std::string, bool, bool)),
        userWidget_, SLOT(contentChanges(std::string, std::string, std::string, bool, bool)));
    connect(userWidget_, SIGNAL(chatRequest(const QString&)), socket_, SLOT(sandRequestChat(const QString&)));
}

void MainWindow::on_openAction_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, 
        tr("打开画板文件"), ".",
        tr("画板文件 (*.HB)"));
    if (!fileName.isEmpty())
    {
        if (okToContinue())
        {
            LogManager::getSingleton().logDebug((QString("打开画板文件, 位置为: ") += fileName).toStdString());
            emit readFile(fileName);
            dataChange = false;
            setCurrentFile(fileName);	
        }
    }
    else 
    {
        LogManager::getSingleton().logWarn("打开文件位置为空");
        return;
    }	
}

void MainWindow::on_newAction_triggered()
{
    if (okToContinue())
    {		
        LogManager::getSingleton().logDebug("新建画板文件");
        paintWidget->cleanWidget();
        dataChange = false;
        curFile = nullptr;
        setCurrentFile(NULL);
    }
    else
    {
        LogManager::getSingleton().logDebug("新建画板取消");
        return;
    }
}

void MainWindow::on_saveAsAction_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("保存画板文件"), ".",
        tr("画板文件(*.HB)"));
    if (!fileName.isEmpty())
    {
        LogManager::getSingleton().logDebug((QString("保存画板文件成功, 文件名为: ") += fileName).toStdString());
        emit writeFile(fileName);
        dataChange = false;
        setCurrentFile(fileName);
    }
}

void MainWindow::dateIsChanged()
{
    dataChange = true;
    setCurrentFile(NULL);
}

QString MainWindow::strippendName(const QString& fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    if (!fileName.isEmpty())
    {
        curFile = fileName;
    }

    QString showName = tr("未命名");
    if (!curFile.isEmpty())
    {
        showName = strippendName(curFile);
    }

    setWindowTitleState(showName);
}

void MainWindow::setWindowTitleState(QString showName)
{
    setWindowTitle(tr("%1%2 - %3").arg(showName)
        .arg(dataChange ? QString("[ * ]") : QString(" "))
        .arg(tr("画板程序")));
}

void MainWindow::on_saveAction_triggered()
{
    if (curFile.isEmpty())
    {
        LogManager::getSingleton().logDebug("文件另存为工序开始");
        on_saveAsAction_triggered();
    }
    else 
    {
        LogManager::getSingleton().logDebug("文件保存成功");
        emit writeFile(curFile);
        dataChange = false;		
        setCurrentFile(NULL);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue())
    {
        event->accept();
    }
    else 
    {
        event->ignore();
    }
}

bool MainWindow::okToContinue()
{
    if (dataChange)
    {
        int r = QMessageBox::warning(this, tr("画板"),
            tr("这个画板文件已经被修改.\n"
            "是否需要保存?"),
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (QMessageBox::Yes == r)
        {
            on_saveAction_triggered();
        } 
        else if (QMessageBox::Cancel == r)
        {
            return false;
        }
    }
    return true;
}

void MainWindow::drawLineActionTriggered()
{
    emit changeCurrentShape(Shape::Line);
}

void MainWindow::drawRectActionTriggered()
{
    emit changeCurrentShape(Shape::Rect);
}

void MainWindow::drawRoundActionTriggered()
{
    emit changeCurrentShape(Shape::Round);
}

void MainWindow::drawRoundRectActionTriggered()
{
    emit changeCurrentShape(Shape::RoundRect);
}

void MainWindow::drawPointsActionTriggered()
{
    emit changeCurrentShape(Shape::Points);
}

void MainWindow::colourPenBlackActionTriggered()
{
    emit changePenColour(Shape::Black);
}

void MainWindow::colourPenBlueActionTriggered()
{
    emit changePenColour(Shape::Blue);
}

void MainWindow::colourPenGreenActionTriggered()
{
    emit changePenColour(Shape::Green);
}

void MainWindow::colourPenRedActionTriggered()
{
    emit changePenColour(Shape::Red);
}

void MainWindow::colourPenNoneActionTriggered()
{
    emit changePenColour(Shape::None);
}

void MainWindow::colourPenCustomActionTriggered()
{
    emit changePenColour(Shape::Custom);
}

void MainWindow::colourBrushBlackActionTriggered()
{
    emit changeBrushColour(Shape::Black);
}

void MainWindow::colourBrushBlueActionTriggered()
{
    emit changeBrushColour(Shape::Blue);
}

void MainWindow::colourBrushGreenActionTriggered()
{
    emit changeBrushColour(Shape::Green);
}

void MainWindow::colourBrushRedActionTriggered()
{
    emit changeBrushColour(Shape::Red);
}

void MainWindow::colourBrushNoneActionTriggered()
{
    emit changeBrushColour(Shape::None);
}

void MainWindow::colourBrushCustomActionTriggered()
{
    emit changeBrushColour(Shape::Custom);
}

void MainWindow::findPenStyleChanged(int style)
{
    switch(style)
    {
    case 0: emit changePenStyle(Shape::SolidLine); break;
    case 1: emit changePenStyle(Shape::DashLine); break;
    case 2: emit changePenStyle(Shape::DotLine); break;
    case 3: emit changePenStyle(Shape::DashDotLine); break;
    case 4: emit changePenStyle(Shape::DashDotLine); break;
    case 5: emit changePenStyle(Shape::NoPen); break;
    }
}

void MainWindow::findBrushStyleChanged(int style)
{
    switch(style)
    {
    case 0:   emit changeBrushStyle(Shape::SolidPattern); break;
    case 1:   emit changeBrushStyle(Shape::Dense1Pattern); break;
    case 2:   emit changeBrushStyle(Shape::Dense2Pattern); break;
    case 3:   emit changeBrushStyle(Shape::Dense3Pattern); break;
    case 4:   emit changeBrushStyle(Shape::Dense4Pattern); break;
    case 5:   emit changeBrushStyle(Shape::Dense5Pattern); break;
    case 6:   emit changeBrushStyle(Shape::Dense6Pattern); break;
    case 7:   emit changeBrushStyle(Shape::Dense7Pattern); break;
    case 8:   emit changeBrushStyle(Shape::HorPattern); break;
    case 9:   emit changeBrushStyle(Shape::VerPattern); break;
    case 10:  emit changeBrushStyle(Shape::CrossPattern); break;
    case 11:  emit changeBrushStyle(Shape::BDiagPattern); break;
    case 12:  emit changeBrushStyle(Shape::FDiagPattern); break;
    case 13:  emit changeBrushStyle(Shape::DiagCrossPat); break;
    case 14:  emit changeBrushStyle(Shape::NoBrush); break;
    }
}

bool MainWindow::winEvent(MSG *message, long *result)
{
    int msgType = message->message;
    if (msgType == WM_DEVICECHANGE)
    {
        /*emit changeUsbState("检测到 WM_DEVICECHANGE 消息");*/
        LogManager::getSingleton().logDebug("系统检测到 WM_DEVICECHANGE　消息");
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)message->lParam;
        switch (message->wParam)
        {
        case DBT_DEVICEARRIVAL:
            if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if (lpdbv->dbcv_flags == 0)
                {
                    QString USBDisk = QString(this->FirstDriveFromMask(lpdbv->dbcv_unitmask));
                    emit changeUsbState(QString("找到USB存储设备: ") += USBDisk);
                    LogManager::getSingleton().logDebug((QString("系统找到USB设备") += USBDisk).toStdString());
                }
            }
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            if (lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if (lpdbv->dbcv_flags == 0)
                {
                    emit changeUsbState("USB设备移除");
                    LogManager::getSingleton().logDebug("USB设备被移除");
                }
            }
            break;				
        }
    }
    return false;
}

char MainWindow::FirstDriveFromMask(ULONG unitmask)
{
    char i;
    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)
            break;
        unitmask = unitmask >> 1;
    }
    return (i + 'A');
}

void MainWindow::sand_login(std::string account, std::string password)
{
    socket_->landing_Data(account, password);
}

void MainWindow::sand_register(std::string account, std::string password, std::string userName, std::string pinYin)
{
    socket_->sandRegister(account, password, userName, pinYin);
}
