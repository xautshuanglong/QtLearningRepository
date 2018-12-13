#include "MyMainWindow.h"

#include <QDebug>
#include <QMouseEvent>
#include <QImageReader>
#include <QImageWriter>
#include <QGridLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QMenu>
#include <QMetaEnum>
#include <QAction>
#include <QKeySequence>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <windowsx.h>
#endif

#include <LogUtil.h>

#include "FramelessWindowHelper.h"
#include "MainTabPageFirst.h"
#include "MainTabPageSetting.h"
#include "MainTabPageDicom.h"
#include "DebugPanel.h"
#include "DebugInfoHardwareWidget.h"

MyMainWindow::MyMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mpFramelessWindow(Q_NULLPTR)
{
    ui.setupUi(this);

    //setAttribute(Qt::WA_TranslucentBackground, true);
    //Qt::WindowFlags oldFlags = windowFlags();
    //setWindowFlags(oldFlags | Qt::FramelessWindowHint);
    mpFramelessWindow = new FramelessWindowHelper(this);

    //DebugPanel *pDebugPanel = new DebugPanel();
    //pDebugPanel->ListenKeyboard(this);

    DebugPanel::GetInstance()->ListenKeyboard(this);
    mpCpuUsageWidget = new DebugInfoHardwareWidget();
    DebugPanel::GetInstance()->AddDebugInfoWidget(QString::fromLocal8Bit("硬件信息"), mpCpuUsageWidget);

    ui.mainTabWidget->tabBar()->setObjectName("mainTabWidget_TabBar");

    mpPageFirst = new MainTabPageFirst(this);
    ui.mainTabWidget->addTab(mpPageFirst, "First");

    mpPageDicom = new MainTabPageDicom(this);
    ui.mainTabWidget->addTab(mpPageDicom, "Dicom");

    mpPageSetting = new MainTabPageSetting(this);
    ui.mainTabWidget->addTab(mpPageSetting, "Setting");

    // 托盘右键菜单栏
    mpSystemTrayMenu = new QMenu(this);
    mpTrayActionShow = new QAction(QStringLiteral("显示"), this);
    mpTrayActionExit = new QAction(QStringLiteral("退出"), this);
    mpSystemTrayMenu->addAction(mpTrayActionShow);
    mpSystemTrayMenu->addAction(mpTrayActionExit);
    this->connect(mpTrayActionShow, SIGNAL(triggered(bool)), SLOT(on_trayActionShow_triggered(bool)));
    this->connect(mpTrayActionExit, SIGNAL(triggered(bool)), SLOT(on_trayActionExit_triggered(bool)));
    // 系统托盘
    mpSystemTray = new QSystemTrayIcon(this);
    mpSystemTray->setIcon(QIcon(":/AppImages/Resources/images/app.ico"));
    mpSystemTray->setContextMenu(mpSystemTrayMenu);
    mpSystemTray->setToolTip("TrayIcon Testing");
    mpSystemTray->show();
    bool test = this->connect(mpSystemTray,
                              SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                              SLOT(on_systemTrayIconActivated(QSystemTrayIcon::ActivationReason)));
}

MyMainWindow::~MyMainWindow()
{
    if (mpFramelessWindow != Q_NULLPTR)
    {
        delete mpFramelessWindow;
        mpFramelessWindow = Q_NULLPTR;
    }
    delete mpCpuUsageWidget;
    DebugPanel::ClearInstance();
}

bool MyMainWindow::event(QEvent *event)
{
    //LogUtil::Info(CODE_LOCATION, "Type=%d", event->type());
    return QMainWindow::event(event);
}

void MyMainWindow::mousePressEvent(QMouseEvent *event)
{
    Qt::MouseEventFlags flags = event->flags();
    QEvent::Type eventType = event->type();
    QPoint mousePos = event->pos();
    QPointF localPos = event->localPos();
    QPointF windowPos = event->windowPos();
    QPointF screenPos = event->screenPos();
    //LogUtil::Info(CODE_LOCATION, "MousePos(%d,%d) LocalPos(%d,%d) WindowPos(%d,%d) ScreenPos(%d,%d)"
    //              " MouseEventFlags=%d Type=%d",
    //              mousePos.x(), mousePos.y(),
    //              localPos.x(), localPos.y(),
    //              windowPos.x(), windowPos.y(),
    //              screenPos.x(), screenPos.y(),
    //              flags, eventType);

    //  Geometry Testing
    //QRect geometry = this->geometry();
    //QRect frame = this->frameGeometry();

    //LogUtil::Debug(CODE_LOCATION, "XY(%d,=%d) Geometry(%d,%d,%d,%d) Frame(%d,%d,%d,%d)",
    //               this->x(), this->y(),
    //               geometry.x(), geometry.y(), geometry.width(), geometry.height(),
    //               frame.x(), frame.y(), frame.width(), frame.height());
}

void MyMainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Qt::MouseEventFlags flags = event->flags();
    QEvent::Type eventType = event->type();
    QPoint mousePos = event->pos();
    QPointF localPos = event->localPos();
    QPointF windowPos = event->windowPos();
    QPointF screenPos = event->screenPos();
    //LogUtil::Info(CODE_LOCATION, "MousePos(%d,%d) LocalPos(%d,%d) WindowPos(%d,%d) ScreenPos(%d,%d)"
    //              " MouseEventFlags=%d Type=%d",
    //              mousePos.x(), mousePos.y(),
    //              localPos.x(), localPos.y(),
    //              windowPos.x(), windowPos.y(),
    //              screenPos.x(), screenPos.y(),
    //              flags, eventType);
}

void MyMainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    Qt::MouseEventFlags flags = event->flags();
    QEvent::Type eventType = event->type();
    QPoint mousePos = event->pos();
    QPointF localPos = event->localPos();
    QPointF windowPos = event->windowPos();
    QPointF screenPos = event->screenPos();
    LogUtil::Info(CODE_LOCATION, "MousePos(%d,%d) LocalPos(%d,%d) WindowPos(%d,%d) ScreenPos(%d,%d)"
                  " MouseEventFlags=%d Type=%d",
                  mousePos.x(), mousePos.y(),
                  localPos.x(), localPos.y(),
                  windowPos.x(), windowPos.y(),
                  screenPos.x(), screenPos.y(),
                  flags, eventType);

    QByteArrayList formatList = QImageWriter::supportedImageFormats();
    LogUtil::Info(CODE_LOCATION, "ImageWriter supported formats:");
    for (int i = 0; i < formatList.size(); ++i)
    {
        LogUtil::Info(CODE_LOCATION, "WriterFormat[%d] = %s", i, formatList[i].constData());
    }

    formatList = QImageReader::supportedImageFormats();
    LogUtil::Info(CODE_LOCATION, "ImageReader supported formats:");
    for (int i = 0; i < formatList.size(); ++i)
    {
        LogUtil::Info(CODE_LOCATION, "ReaderFormat[%d] = %s", i, formatList[i].constData());
    }
}

void MyMainWindow::mouseMoveEvent(QMouseEvent *event)
{
    Qt::MouseEventFlags flags = event->flags();
    QEvent::Type eventType = event->type();
    QPoint mousePos = event->pos();
    QPointF localPos = event->localPos();
    QPointF windowPos = event->windowPos();
    QPointF screenPos = event->screenPos();
    //LogUtil::Info(CODE_LOCATION, "MousePos(%d,%d) LocalPos(%d,%d) WindowPos(%d,%d) ScreenPos(%d,%d)"
    //              " MouseEventFlags=%d Type=%d",
    //              mousePos.x(), mousePos.y(),
    //              localPos.x(), localPos.y(),
    //              windowPos.x(), windowPos.y(),
    //              screenPos.x(), screenPos.y(),
    //              flags, eventType);
    //LogUtil::CollectData(CODE_LOCATION, LOG_COLLECTION_TYPE_0, "MousePos %d %d", mousePos.x(), mousePos.y());
}


void MyMainWindow::wheelEvent(QWheelEvent *event)
{
    QEvent::Type eventType = event->type();
    QPoint mousePos = event->pos();
    //LogUtil::Debug(CODE_LOCATION, "MousePos(%d,%d)"
    //              " Type=%d",
    //              mousePos.x(), mousePos.y(),
    //              eventType);
}

void MyMainWindow::keyPressEvent(QKeyEvent *event)
{
    QMainWindow::keyPressEvent(event);
}

void MyMainWindow::keyReleaseEvent(QKeyEvent *event)
{
    QMainWindow::keyReleaseEvent(event);
}


void MyMainWindow::focusInEvent(QFocusEvent *event)
{
    QMainWindow::focusInEvent(event);
}

void MyMainWindow::focusOutEvent(QFocusEvent *event)
{
    QMainWindow::focusOutEvent(event);
}

void MyMainWindow::enterEvent(QEvent *event)
{
    QMainWindow::enterEvent(event);
}

void MyMainWindow::leaveEvent(QEvent *event)
{
    QMainWindow::leaveEvent(event);
}

void MyMainWindow::paintEvent(QPaintEvent *event)
{
    QMainWindow::paintEvent(event);
}

void MyMainWindow::moveEvent(QMoveEvent *event)
{
    QMainWindow::moveEvent(event);
}

void MyMainWindow::closeEvent(QCloseEvent *event)
{
    LogUtil::Info(CODE_LOCATION, "Type=%d Enter close event...", event->type());
    //DebugPanel::GetInstance()->close(); // DebugPanel 会随着应用退出关闭。
    QMainWindow::closeEvent(event);
}

void MyMainWindow::resizeEvent(QResizeEvent *event)
{
    QRect winRect = this->rect();
    QRect titleBarRect = mpFramelessWindow->GetTitleBarRect();
    QRect mainTabRect(5, 0, winRect.width() - 10, winRect.height() - titleBarRect.height() - 5);
    ui.mainTabWidget->setGeometry(mainTabRect);

    QMainWindow::resizeEvent(event);
}

void MyMainWindow::ShowAndActivateWindow()
{
    if (!this->isVisible())
    {
        this->show();
    }
    if (!this->isActiveWindow())
    {
        this->activateWindow();
    }
}

void MyMainWindow::on_systemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Unknown:
        LogUtil::Debug(CODE_LOCATION, "QSystemTrayIcon::ActivationReason Unknown");
        break;
    case QSystemTrayIcon::Context:
        LogUtil::Debug(CODE_LOCATION, "QSystemTrayIcon::ActivationReason Context");
        break;
    case QSystemTrayIcon::DoubleClick:
        //LogUtil::Debug(CODE_LOCATION, "QSystemTrayIcon::ActivationReason DoubleClick");
        this->ShowAndActivateWindow();
        break;
    case QSystemTrayIcon::Trigger:
        LogUtil::Debug(CODE_LOCATION, "QSystemTrayIcon::ActivationReason Trigger");
        break;
    case QSystemTrayIcon::MiddleClick:
        LogUtil::Debug(CODE_LOCATION, "QSystemTrayIcon::ActivationReason MiddleClick");
        break;
    default:
        break;
    }
}

void MyMainWindow::on_trayActionShow_triggered(bool checked)
{
    this->ShowAndActivateWindow();
}

void MyMainWindow::on_trayActionExit_triggered(bool checked)
{
    this->close();
}
