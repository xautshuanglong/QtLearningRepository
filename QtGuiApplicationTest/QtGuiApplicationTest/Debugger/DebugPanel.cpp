#include "DebugPanel.h"
#include "ui_DebugPanel.h"

#include <QKeyEvent>
#include <QMetaEnum>
#include <QMenuBar>
#include <QDockWidget>
#include <QListWidget>
#include <QLabel>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QSplitter>
#include <QSignalMapper>
#include <QTimer>

#include <LogUtil.h>
#include "Debugger/DebugMenuEvent.h"
#include "Debugger/DebugInfoBaseWidget.h"
#include "Core/TitleBarWidget.h"

QAtomicPointer<DebugPanel> DebugPanel::mInstance = Q_NULLPTR;
QMutex DebugPanel::mMutexInstance;

enum MenuItemType
{
    MenuItemType_unknow = 0,
    MenuItemType_menu = 1,   // 作为父级菜单，点击没有动作
    MenuItemType_action = 2, // 菜单项点击会有动作触发响应的处理函数
};

/*
* 为了处理方便，先定义父级菜单，按包含关系及先后顺序定义子级菜单。
*/
typedef struct tagMenuItem
{
    int id;              // 点击了哪个菜单项（在 HDebugPanel.h 中枚举）
    int parent;          // 隶属于哪个父菜单
    MenuItemType type;   // QMenu 还是 QAction，后者需做槽函数映射处理
    bool checkable;      // 是否可标记，仅对 QAction 有效
    QString text;        // 菜单项显示的文本
}HMenuItem;

static HMenuItem gMenuItems[] =
{
    { DebugPanel::FILE,                DebugPanel::FILE,           MenuItemType_menu,   false, QObject::tr("File") },          // 文件
    { DebugPanel::FILE_SAVE,           DebugPanel::FILE,           MenuItemType_action, false, QObject::tr("Save") },          // 文件-保存
    { DebugPanel::FILE_SAVE_AS,        DebugPanel::FILE,           MenuItemType_action, false, QObject::tr("Save As") },       // 文件-另存为
    { DebugPanel::VIEW,                DebugPanel::VIEW,           MenuItemType_menu,   false, QObject::tr("View") },          // 视图
    { DebugPanel::VIEW_UPDATE,         DebugPanel::VIEW,           MenuItemType_menu,   false, QObject::tr("Update") },        // 更新频率
    { DebugPanel::VIEW_UPDATE_HIGH,    DebugPanel::VIEW_UPDATE,    MenuItemType_action, true,  QObject::tr("High") },          // 更新频率 - 高
    { DebugPanel::VIEW_UPDATE_MIDDLE,  DebugPanel::VIEW_UPDATE,    MenuItemType_action, true,  QObject::tr("Middle") },        // 更新频率 - 中
    { DebugPanel::VIEW_UPDATE_LOW,     DebugPanel::VIEW_UPDATE,    MenuItemType_action, true,  QObject::tr("Low") },           // 更新频率 - 低
    { DebugPanel::VIEW_LIST,           DebugPanel::VIEW,           MenuItemType_action, false, QObject::tr("ListView") },      // 视图-列表视图
    { DebugPanel::VIEW_TAB,            DebugPanel::VIEW,           MenuItemType_action, false, QObject::tr("TabView") },       // 视图-标前视图
    { DebugPanel::SETTING,             DebugPanel::SETTING,        MenuItemType_menu,   false, QObject::tr("Settings") },      // 设置
    { DebugPanel::SETTING_DEBUG,       DebugPanel::SETTING,        MenuItemType_menu,   false, QObject::tr("Debug") },         // 设置-调试
    { DebugPanel::SETTING_DEBUG_TEST1, DebugPanel::SETTING_DEBUG,  MenuItemType_action, false, QObject::tr("Debug_Test_1") },  // 设置-调试-测试 1
    { DebugPanel::SETTING_DEBUG_TEST2, DebugPanel::SETTING_DEBUG,  MenuItemType_action, false, QObject::tr("Debug_Test_2") },  // 设置-调试-测试 2
    { DebugPanel::SETTING_REGION,      DebugPanel::SETTING,        MenuItemType_menu,   false, QObject::tr("Region") },        // 设置-区域
    { DebugPanel::SETTING_REGION_CN,   DebugPanel::SETTING_REGION, MenuItemType_action, false, QObject::tr("Region_CN") },     // 设置-区域-中文
    { DebugPanel::SETTING_REGION_EN,   DebugPanel::SETTING_REGION, MenuItemType_action, false, QObject::tr("Region_EN") },     // 设置-区域-英文
};

DebugPanel::DebugPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DebugPanel)
{
    ui->setupUi(this);
    // 窗口属性
    this->setAttribute(Qt::WA_QuitOnClose, false);
    this->setWindowTitle(QStringLiteral("调试面板"));
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    //this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    //this->setFocusPolicy(Qt::StrongFocus); // 保证可接收到显示字符，否则只能接受特殊键，如：Control、Shift、Alt 等。

    mpTitleWidget = new TitleBarWidget(this);

    // 调试信息更新定时器
    mpUpdateTimer = new QTimer(this);
    mpUpdateTimer->setInterval(1000);
    this->connect(mpUpdateTimer, SIGNAL(timeout()), SLOT(SlotUpdateDebugWidget()));

    this->InitMenu();              // 初始胡菜单栏
    this->InitDebugInfoWidgets();  // 初始化调试窗口部件
}

DebugPanel::~DebugPanel()
{
    delete ui;
}

void DebugPanel::InitMenu()
{
    mpMenuBar = new QMenuBar(this);

    // 菜单响应槽定义
    QSignalMapper *pSignalMapper = new QSignalMapper(this);
    this->connect(pSignalMapper, SIGNAL(mapped(int)), SLOT(SlotMenuMapped(int)));

    // 菜单表处理
    int menuCount = sizeof(gMenuItems) / sizeof(HMenuItem);
    int menuItemID, parentID;
    bool checkableFlag = false;
    QString menuItemText;
    MenuItemType menuItemType;
    QMenu *pMenuTemp, *pMenuParent;
    QAction *pActionTemp;
    for (int i = 0; i < menuCount; ++i)
    {
        menuItemID = gMenuItems[i].id;
        menuItemType = gMenuItems[i].type;
        menuItemText = gMenuItems[i].text;
        checkableFlag = gMenuItems[i].checkable;
        parentID = gMenuItems[i].parent;

        if (menuItemID == parentID) // 顶级菜单
        {
            pMenuTemp = new QMenu(menuItemText, this);
            mMapMenuIdItem[menuItemID] = pMenuTemp;
            mpMenuBar->addMenu(pMenuTemp);
        }
        else
        {
            if (menuItemType == MenuItemType_menu) // 子菜单
            {
                pMenuTemp = new QMenu(menuItemText, this);
                mMapMenuIdItem[menuItemID] = pMenuTemp;
                pMenuParent = (QMenu*)mMapMenuIdItem[parentID];
                pMenuParent->addMenu(pMenuTemp);
            }
            else if (menuItemType == MenuItemType_action) // 具体菜单动作
            {
                pActionTemp = new QAction(menuItemText, this);
                pActionTemp->setCheckable(checkableFlag);
                mMapMenuIdItem[menuItemID] = pActionTemp;
                pMenuParent = (QMenu*)mMapMenuIdItem[parentID];
                pMenuParent->addAction(pActionTemp);
                // 信号映射
                pSignalMapper->setMapping(pActionTemp, menuItemID);
                this->connect(pActionTemp, SIGNAL(triggered()), pSignalMapper, SLOT(map()));
            }
        }
    }
    // 特殊菜单项初始化
    QAction *pUpdateDefault = (QAction*)mMapMenuIdItem[VIEW_UPDATE_MIDDLE];
    pUpdateDefault->setChecked(true);
}

void DebugPanel::InitDebugInfoWidgets()
{
    // 列表控件
    mpListWidget = new QListWidget(ui->centralWidget);
    mpListWidget->setGeometry(0, 0, 100, 50);
    mpStackedWidget = new QStackedWidget(ui->centralWidget);
    mpStackedWidget->setGeometry(100, 0, 100, 50);

    // 窗口分栏处理
    mpSpliter = new QSplitter(Qt::Horizontal, ui->centralWidget);
    mpSpliter->addWidget(mpListWidget);
    mpSpliter->addWidget(mpStackedWidget);
    mpSpliter->setStretchFactor(0, 1);
    mpSpliter->setStretchFactor(1, 9);
    QSplitterHandle *pSplitterHandle = mpSpliter->handle(1);
    if (pSplitterHandle)
    {
        pSplitterHandle->setFixedWidth(1);
    }

    connect(mpListWidget, SIGNAL(currentRowChanged(int)), mpStackedWidget, SLOT(setCurrentIndex(int)));
}

DebugPanel* DebugPanel::GetInstance()
{
#ifdef Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
    if (mInstance.testAndSetOrdered(0, 0))
    {
        QMutexLocker locker(&mMutexInstance);
        mInstance.testAndSetOrdered(0, new DebugPanel());
    }
#else
    if (mInstance == Q_NULLPTR)
    {
        QMutexLocker locker(&mMutexInstance);
        if (mInstance == Q_NULLPTR)
        {
            mInstance = new DebugPanel();
        }
    }
#endif
    return mInstance;
}

void DebugPanel::ClearInstance()
{
#ifdef Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE

    if (!mInstance.testAndSetOrdered(0, 0))
    {
        QMutexLocker locker(&mMutexInstance);
        DebugPanel *pTempPanel = mInstance.fetchAndStoreOrdered(Q_NULLPTR);
        delete pTempPanel;
    }
#else
    if (mInstance != Q_NULLPTR)
    {
        QMutexLocker locker(&mMutexInstance);
        if (mInstance != Q_NULLPTR)
        {
            delete mInstance;
            mInstance = Q_NULLPTR;
        }
    }
#endif
}

void DebugPanel::ListenKeyboard(QObject *pTarget)
{
    if (pTarget != Q_NULLPTR)
    {
        pTarget->installEventFilter(this);
    }
}

void DebugPanel::AddDebugInfoWidget(QString topic, DebugInfoBaseWidget *pWidget)
{
    if (pWidget != Q_NULLPTR)
    {
        mpListWidget->addItem(topic);
        mpStackedWidget->addWidget(pWidget);
        LogUtil::Debug(CODE_LOCATION, "Class: %s  HashCode: %u  TypeIndex: %d",
                       typeid(*pWidget).name(), typeid(*pWidget).hash_code(), std::type_index(typeid(*pWidget)));
        mMapTypeWidget[typeid(*pWidget).hash_code()] = pWidget;
    }
}

void DebugPanel::SlotMenuMapped(int menuID)
{
    //    LogUtil::Debug(CODE_LOCATION, "menu item clicked : %d", menuID);
    this->HandleUpdateRate(menuID); // 处理更新频率菜单事件

    QWidget *pCurWidget = mpStackedWidget->currentWidget();
    DebugInfoBaseWidget *pDebugBaseWidget = dynamic_cast<DebugInfoBaseWidget*>(pCurWidget);
    if (pDebugBaseWidget != Q_NULLPTR)
    {
        DebugMenuEvent *pDebugEnvet = new DebugMenuEvent();
        pDebugEnvet->SetMenuID(menuID);
        pDebugBaseWidget->OnDebugMenuEvent(pDebugEnvet);
        delete pDebugEnvet;
    }
}

void DebugPanel::SlotUpdateDebugWidget()
{
    QWidget *pCurWidget = mpStackedWidget->currentWidget();
    DebugInfoBaseWidget *pDebugBaseWidget = dynamic_cast<DebugInfoBaseWidget*>(pCurWidget);
    if (pDebugBaseWidget != Q_NULLPTR)
    {
        pDebugBaseWidget->OnUpdateDebugInfo();
    }
}

void DebugPanel::closeEvent(QCloseEvent *event)
{
    QWidget::close();
}

void DebugPanel::keyPressEvent(QKeyEvent *event)
{
    int keyValue = event->key();
    Qt::KeyboardModifiers modifiers = event->modifiers();
    QMetaEnum KeyboardModifierEnum = QMetaEnum::fromType<Qt::KeyboardModifiers>();
    QByteArray modifierArray = KeyboardModifierEnum.valueToKeys(modifiers);
    LogUtil::Debug(CODE_LOCATION, "%s + %d", modifierArray.toStdString().c_str(), keyValue);
    
    if (event->matches(QKeySequence::Cancel) || QKeySequence("Ctrl+Shift+D")== QKeySequence(event->modifiers() | event->key()))
    {
        this->ShowOrHideDebugPanel();
    }
}

void DebugPanel::keyReleaseEvent(QKeyEvent *event)
{
    int keyValue = event->key();
    Qt::KeyboardModifiers modifiers = event->modifiers();
    QMetaEnum KeyboardModifierEnum = QMetaEnum::fromType<Qt::KeyboardModifiers>();
    QByteArray modifierArray = KeyboardModifierEnum.valueToKeys(modifiers);
    //LogUtil::Debug(CODE_LOCATION, "%s + %d", modifierArray.toStdString().c_str(), keyValue);
}

void DebugPanel::resizeEvent(QResizeEvent *event)
{
    //ui->centralWidget->setStyleSheet("background-color: green;");
    int borderSize = 5;
    QSize newSize = event->size();

    int x = borderSize;
    int w = newSize.width() - borderSize * 2;

    int titleBarHeight = mpTitleWidget->height();
    int y = borderSize;
    mpTitleWidget->setGeometry(x, y, w, titleBarHeight);

    int menuHeight = mpMenuBar->height();
    y += titleBarHeight;
    mpMenuBar->setGeometry(x, y, w, menuHeight);

    int centralHeight = newSize.height() - titleBarHeight - menuHeight - borderSize * 2;
    y += menuHeight;
    ui->centralWidget->setGeometry(x, y, w, centralHeight);
    mpSpliter->resize(w, centralHeight);
}

bool DebugPanel::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
    case QEvent::KeyPress:
        this->HandleKeyPressEvent(static_cast<QKeyEvent*>(event));
        break;
    default:
        break;
    }

    return QWidget::eventFilter(obj, event);
}

void DebugPanel::showEvent(QShowEvent *event)
{
    mpUpdateTimer->start();
}

void DebugPanel::hideEvent(QHideEvent *event)
{
    mpUpdateTimer->stop();
}

void DebugPanel::HandleKeyPressEvent(QKeyEvent *event)
{
    int keyValue = event->key();
    Qt::KeyboardModifiers modifiers = event->modifiers();
    QMetaEnum KeyboardModifierEnum = QMetaEnum::fromType<Qt::KeyboardModifiers>();
    QByteArray modifierArray = KeyboardModifierEnum.valueToKeys(modifiers);
    LogUtil::Debug(CODE_LOCATION, "%s + %d", modifierArray.toStdString().c_str(), keyValue);

    if (QKeySequence("Ctrl+Shift+D") == QKeySequence(event->modifiers() | event->key()))
    {
        this->ShowOrHideDebugPanel();
    }
}

void DebugPanel::HandleUpdateRate(int menuID)
{
    QVector<int> vecMenuID = { VIEW_UPDATE_HIGH, VIEW_UPDATE_MIDDLE, VIEW_UPDATE_LOW };
    if (vecMenuID.contains(menuID))
    {
        switch (menuID)
        {
        case VIEW_UPDATE_HIGH:
            mpUpdateTimer->setInterval(300);
            break;
        case VIEW_UPDATE_MIDDLE:
            mpUpdateTimer->setInterval(1000);
            break;
        case VIEW_UPDATE_LOW:
            mpUpdateTimer->setInterval(2000);
            break;
        }

        QAction *pCurAction = Q_NULLPTR;
        for (int i = 0; i < vecMenuID.size(); ++i)
        {
            int menuIdIndex = vecMenuID[i];
            pCurAction = (QAction*)mMapMenuIdItem[menuIdIndex];
            pCurAction->setChecked(menuIdIndex == menuID);
        }
    }
}

void DebugPanel::ShowOrHideDebugPanel()
{
    if (this->isVisible())
    {
        if (this->isMinimized())
        {
            this->showNormal();
        }
        else
        {
            this->hide();
        }
    }
    else
    {
        if (this->isMinimized())
        {
            this->showNormal();
        }
        else
        {
            this->show();
        }
    }
}
