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

#include <LogUtil.h>

DebugPanel::DebugPanel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DebugPanel)
{
    ui->setupUi(this);
    // 窗口属性
    this->setWindowTitle(QStringLiteral("调试面板"));
    this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
    //this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    // 菜单栏
    mpMenuBar = new QMenuBar(this);
    QAction *pAction1 = new QAction(QStringLiteral("文件"), this);
    QAction *pAction2 = new QAction(QStringLiteral("视图"), this);
    QAction *pAction3 = new QAction(QStringLiteral("设置项1"), this);
    QAction *pAction4 = new QAction(QStringLiteral("设置项2"), this);
    QMenu *pTempMenu = new QMenu(QStringLiteral("设置"));
    pTempMenu->addAction(pAction3);
    pTempMenu->addAction(pAction4);
    mpMenuBar->addAction(pAction1);
    mpMenuBar->addAction(pAction2);
    mpMenuBar->addMenu(pTempMenu);

    // 停靠窗口部件
    //QDockWidget *pDockWidget = new QDockWidget("TestDockWindow 1", ui->centralWidget);
    //QWidget *pDockWidgetContents = new QWidget();
    //pDockWidget->setWidget(pDockWidgetContents);
    //pDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures);

    // 列表控件
    QListWidget *pListWidget = new QListWidget(this);
    pListWidget->insertItem(0, QString::fromLocal8Bit("第一項"));
    pListWidget->insertItem(1, QString::fromLocal8Bit("第二項"));
    pListWidget->insertItem(2, QString::fromLocal8Bit("第三項"));
    //pListWidget->setFocusPolicy(Qt::NoFocus);

    QLabel *label1 = new QLabel(QString::fromLocal8Bit("第一項的內容"));
    QLabel *label2 = new QLabel(QString::fromLocal8Bit("第二項的內容"));
    QLabel *label3 = new QLabel(QString::fromLocal8Bit("第三項的內容"));

    QStackedWidget *stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(label1);
    stackedWidget->addWidget(label2);
    stackedWidget->addWidget(label3);

    QHBoxLayout *mainLayout = new QHBoxLayout(ui->centralWidget);
    mainLayout->addWidget(pListWidget);
    mainLayout->addWidget(stackedWidget, 0, Qt::AlignHCenter);
    mainLayout->setStretchFactor(pListWidget, 1);
    mainLayout->setStretchFactor(stackedWidget, 4);
    connect(pListWidget, SIGNAL(currentRowChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));
}

DebugPanel::~DebugPanel()
{
    LogUtil::Debug(CODE_LOCATION, "Destruct instance");
    delete ui;
    delete mpMenuBar;
}

DebugPanel* DebugPanel::GetInstance()
{
    static DebugPanel instance;
    return &instance;
}

void DebugPanel::ListenKeyboard(QObject *pTarget)
{
    if (pTarget != Q_NULLPTR)
    {
        pTarget->installEventFilter(this);
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
    //ui->centralWidget->setGeometry(0, 0, newSize.width(), newSize.height());
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
