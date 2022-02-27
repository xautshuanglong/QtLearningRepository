#include "MiscellaneousImageGrabber.h"
#include "ui_MiscellaneousImageGrabber.h"

// QT Headers
#include <QImage>
#include <QPixmap>
#include <QDateTime>
#include <QThread>
#include <QScreen>
#include <QDesktopWidget>
#include <QMouseEvent>

#include "JCB_Logger/LogUtil.h"

MiscellaneousImageGrabber::MiscellaneousImageGrabber(QWidget* parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousImageGrabber())
{
    ui->setupUi(this);
    this->setMouseTracking(true);
}

MiscellaneousImageGrabber::~MiscellaneousImageGrabber()
{
    delete ui;
}

QString MiscellaneousImageGrabber::GetTitle()
{
    return QObject::tr("Image Grabber");
}

QString MiscellaneousImageGrabber::GetTitleTooltip()
{
    return QObject::tr("Grab window desktop screen and scroll area.");
}

MiscellaneousTestGroup MiscellaneousImageGrabber::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousImageGrabber::GetItemID()
{
    return MiscellaneousTestItem::QT_Image_Grabber;
}

void MiscellaneousImageGrabber::mouseMoveEvent(QMouseEvent* mouseEvent)
{
    QPoint tempPoint = mouseEvent->pos();
    mMousePoint = this->mapToGlobal(tempPoint);
    mCurMouseWin = ::WindowFromPoint(POINT(mMousePoint.x(), mMousePoint.y()));
    
    this->UpdateLogInfo();
}

void MiscellaneousImageGrabber::on_btnGrabWindow_clicked()
{
    char objName[128] = { 0 };
    DWORD bufLenNeed = 0;

    std::string deskClassName;
    HWND desktopWnd = ::GetDesktopWindow();
    this->mWinRectList.clear();
    this->GetWindowClassName(desktopWnd, deskClassName);
    this->EnumChildWindowRecursively(desktopWnd);
    this->UpdateLogInfo();

    //HWINSTA hWinStation = GetProcessWindowStation();
    //::GetUserObjectInformation(hWinStation, UOI_NAME, objName, 128, &bufLenNeed);
    //::EnumDesktops(hWinStation, EnumDesktopProc, reinterpret_cast<LPARAM>(this));

    //::EnumWindowStations(EnumWindowStationProc, reinterpret_cast<LPARAM>(this));
}

void MiscellaneousImageGrabber::on_btnGrabDesktop_clicked()
{
    QDesktopWidget* pDesktop = QApplication::desktop();
    QString filename = QString("E:\\Temp\\ScreenShots\\QtGrabberTest_%1.png")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz"));
    QPixmap shotImag = pDesktop->grab();
    shotImag.save(filename);

    QString filename1 = QString("E:\\Temp\\ScreenShots\\QtGrabberTest_%1.png")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz"));
    QPixmap pixmap1(pDesktop->size());
    pDesktop->render(&pixmap1);
    pixmap1.save(filename1);

    QString filename2 = QString("E:\\Temp\\ScreenShots\\QtGrabberTest_%1.png")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz"));
    QPixmap pixmap2 = QPixmap::grabWindow(pDesktop->screen()->winId());
    pixmap2.save(filename2);
}

void MiscellaneousImageGrabber::on_btnGrabScreen_clicked()
{
    QScreen *primaryScreen = QGuiApplication::primaryScreen();
    QString filename = QString("E:\\Temp\\ScreenShots\\QtGrabberTest_%1.png")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz"));
    QPixmap shotImag = primaryScreen->grabWindow(0);
    shotImag.save(filename);
}

void MiscellaneousImageGrabber::on_btnGrabWidget_clicked()
{
    QWidget* pWidget = this;
    while (pWidget != nullptr)
    {
        QString filename = QString("E:\\Temp\\ScreenShots\\QtGrabberTest_%1.png")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz"));
        QPixmap shotImag = pWidget->grab(QRect(QPoint(0, 0), pWidget->size()));
        shotImag.save(filename);
        pWidget = pWidget->parentWidget();
    }
}

void MiscellaneousImageGrabber::on_btnGrabScreenAll_clicked()
{
    int i = 0;
}

void MiscellaneousImageGrabber::on_btnGrabScrollArea_clicked()
{
    int i = 0;
}

void MiscellaneousImageGrabber::UpdateLogInfo()
{
    static int i = 0;
    RECT curWinRect = { 0 };
    ::GetWindowRect(mCurMouseWin, &curWinRect);

    QString logContent = QString("LogInfo Test %1\n").arg(++i);
    logContent += QString("Cur Win : 0x%1\n").arg(QString("%1").arg(reinterpret_cast<long>(mCurMouseWin), 8, 16, QChar('0')).toUpper());;
    logContent += QString("Mouse Point : (%1, %2)\n").arg(mMousePoint.x()).arg(mMousePoint.y());
    logContent += QString("Mouse Win Rect : (%1, %2) (%3, %4)")
        .arg(curWinRect.left).arg(curWinRect.top).arg(curWinRect.right - curWinRect.left).arg(curWinRect.bottom - curWinRect.top);
    ui->teLogInfo->setText(logContent);
}

void MiscellaneousImageGrabber::GetWindowClassName(HWND hWnd, std::string& wndClassName)
{
    char className[128] = { 0 };
    ::GetClassName(hWnd, className, 128);
    wndClassName = className;
}

void MiscellaneousImageGrabber::EnumChildWindowRecursively(HWND parentWnd)
{
    ::EnumChildWindows(parentWnd, EnumChildWindowProc, reinterpret_cast<LPARAM>(this));
}

BOOL MiscellaneousImageGrabber::EnumChildWindowProcShadow(HWND childWnd)
{
    std::string wndClassName;
    this->GetWindowClassName(childWnd, wndClassName);

    if (::IsWindow(childWnd) && ::IsWindowVisible(childWnd))
    {
        RECT winRect = { 0 };
        ::GetWindowRect(childWnd, &winRect);
        mWinRectList.emplace_back(winRect);
        //LogUtil::Debug(CODE_LOCATION, "childWnd=0x%08X wndClassName=%s", childWnd, wndClassName.c_str());
    }
    //LogUtil::Debug(CODE_LOCATION, "childWnd=0x%08X wndClassName=%s", childWnd, wndClassName.c_str());

    ::EnumChildWindows(childWnd, EnumChildWindowProc, reinterpret_cast<LPARAM>(this));

    return TRUE;
}

BOOL MiscellaneousImageGrabber::EnumDesktopProcShadow(LPTSTR lpszDesktop)
{
    LogUtil::Debug(CODE_LOCATION, "lpszDesktop=%s", lpszDesktop);
    return TRUE;
}

BOOL MiscellaneousImageGrabber::EnumWindowStationProcShadow(LPTSTR lpszWindowStation)
{
    LogUtil::Debug(CODE_LOCATION, "lpszWindowStation=%s", lpszWindowStation);
    return TRUE;
}

BOOL CALLBACK MiscellaneousImageGrabber::EnumChildWindowProc(_In_ HWND hwnd, _In_ LPARAM lParam)
{
    MiscellaneousImageGrabber* pThis = reinterpret_cast<MiscellaneousImageGrabber*>(lParam);
    return pThis->EnumChildWindowProcShadow(hwnd);
}

BOOL CALLBACK MiscellaneousImageGrabber::EnumDesktopProc(_In_ LPTSTR lpszDesktop, _In_ LPARAM lParam)
{
    MiscellaneousImageGrabber *pThis = reinterpret_cast<MiscellaneousImageGrabber*>(lParam);
    return pThis->EnumDesktopProcShadow(lpszDesktop);
}

BOOL CALLBACK MiscellaneousImageGrabber::EnumWindowStationProc(_In_ LPTSTR lpszWindowStation, _In_ LPARAM lParam)
{
    MiscellaneousImageGrabber* pThis = reinterpret_cast<MiscellaneousImageGrabber*>(lParam);
    return pThis->EnumWindowStationProcShadow(lpszWindowStation);
}
