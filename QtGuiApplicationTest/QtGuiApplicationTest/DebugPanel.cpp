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

#include <LogUtil.h>

QAtomicPointer<DebugPanel> DebugPanel::mInstance = Q_NULLPTR;
QMutex DebugPanel::mMutexInstance;

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

    this->InitMenu();              // 初始胡菜单栏
    this->InitDebugInfoWidgets();  // 初始化调试窗口部件

    // 窗口分栏处理
    mpSpliter = new QSplitter(Qt::Horizontal, ui->centralWidget);
    mpSpliter->addWidget(mpListWidget);
    mpSpliter->addWidget(mpStackedWidget);
    mpSpliter->setStretchFactor(0, 25);
    mpSpliter->setStretchFactor(1, 75);
    //mpSpliter->setHandleWidth(1);
}

DebugPanel::~DebugPanel()
{
    delete pMenuFile;
    delete ui;
}

void DebugPanel::InitMenu()
{
    QAction *pActionFileSave = new QAction(QStringLiteral("保存"), this);
    QAction *pActionFileSaveAs = new QAction(QStringLiteral("另存为"), this);
    pMenuFile = new QMenu(QStringLiteral("文件"), this);
    pMenuFile->addAction(pActionFileSave);
    pMenuFile->addAction(pActionFileSaveAs);

    QAction *pActionViewList = new QAction(QStringLiteral("列表"), this);
    QAction *pActionViewTab = new QAction(QStringLiteral("标签"), this);
    QMenu *pMenuView = new QMenu(QStringLiteral("视图"), this);
    pMenuView->addAction(pActionViewList);
    pMenuView->addAction(pActionViewTab);

    QAction *pActionSettingTest = new QAction(QStringLiteral("测试项"), this);
    QMenu *pMenuSetting = new QMenu(QStringLiteral("设置"), this);
    pMenuSetting->addAction(pActionSettingTest);

    mpMenuBar = new QMenuBar(this);
    mpMenuBar->addMenu(pMenuFile);
    mpMenuBar->addMenu(pMenuView);
    mpMenuBar->addMenu(pMenuSetting);
}

void DebugPanel::InitDebugInfoWidgets()
{
    // 列表控件
    mpListWidget = new QListWidget(this);
    mpListWidget->insertItem(0, QString::fromLocal8Bit("第一项"));
    mpListWidget->insertItem(1, QString::fromLocal8Bit("第二项"));
    mpListWidget->insertItem(2, QString::fromLocal8Bit("第三项"));
    //pListWidget->setFocusPolicy(Qt::NoFocus);
    QLabel *label1 = new QLabel(QString::fromLocal8Bit("第一项的內容"));
    QLabel *label2 = new QLabel(QString::fromLocal8Bit("第二项的內容"));
    QLabel *label3 = new QLabel(QString::fromLocal8Bit("第三项的內容"));
    mpStackedWidget = new QStackedWidget(this);
    mpStackedWidget->addWidget(label1);
    mpStackedWidget->addWidget(label2);
    mpStackedWidget->addWidget(label3);
    connect(mpListWidget, SIGNAL(currentRowChanged(int)), mpStackedWidget, SLOT(setCurrentIndex(int)));
}

DebugPanel* DebugPanel::GetInstance()
{
#ifdef Q_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE
    if (mInstance.testAndSetOrdered(0, 0))//第一次检测
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

void DebugPanel::ListenKeyboard(QObject *pTarget)
{
    if (pTarget != Q_NULLPTR)
    {
        pTarget->installEventFilter(this);
    }
}

void DebugPanel::AddDebugInfoWidget(QString topic, QWidget *pWidget)
{
    if (pWidget != Q_NULLPTR)
    {
        mpListWidget->addItem(topic);
        mpStackedWidget->addWidget(pWidget);
    }
}

void DebugPanel::closeEvent(QCloseEvent *event)
{
    QWidget::close();
}

void DebugPanel::keyPressEvent(QKeyEvent *event)
{
    //int keyValue = event->key();
    //Qt::KeyboardModifiers modifiers = event->modifiers();
    //QMetaEnum KeyboardModifierEnum = QMetaEnum::fromType<Qt::KeyboardModifiers>();
    //QByteArray modifierArray = KeyboardModifierEnum.valueToKeys(modifiers);
    //LogUtil::Debug(CODE_LOCATION, "%s + %d", modifierArray.toStdString().c_str(), keyValue);
    
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
    this->window();
}

void DebugPanel::resizeEvent(QResizeEvent *event)
{
    //ui->centralWidget->setStyleSheet("background-color: green;");
    QRect menuRectGeometry = mpMenuBar->geometry();
    QSize newSize = event->size();
    mpMenuBar->resize(newSize.width(), menuRectGeometry.height());
    ui->centralWidget->setGeometry(0, menuRectGeometry.height(), newSize.width(), newSize.height() - menuRectGeometry.height());
    mpSpliter->resize(newSize.width(), newSize.height() - menuRectGeometry.height());
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

void DebugPanel::HandleKeyPressEvent(QKeyEvent *event)
{
    if (QKeySequence("Ctrl+Shift+D") == QKeySequence(event->modifiers() | event->key()))
    {
        this->ShowOrHideDebugPanel();
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
