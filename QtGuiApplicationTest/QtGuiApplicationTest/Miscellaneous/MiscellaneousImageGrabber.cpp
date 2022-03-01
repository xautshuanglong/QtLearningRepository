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
#include <QPainter>

#include "JCB_Logger/LogUtil.h"

MiscellaneousImageGrabber::MiscellaneousImageGrabber(QWidget* parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousImageGrabber())
    , mCurMouseWin(nullptr)
{
    ui->setupUi(this);
    this->setMouseTracking(true);

    mpScreenShotEditer = new ScreenShotEditer();

    //QWidget *pWidget = new QWidget(this);
    //QPalette palette2 = pWidget->palette();
    //pWidget->setGeometry(0, 0, 300, 200);
    //palette2.setColor(QPalette::Background, Qt::green);
    //pWidget->setAutoFillBackground(true);
    //pWidget->setPalette(palette2);
}

MiscellaneousImageGrabber::~MiscellaneousImageGrabber()
{
    if (mpScreenShotEditer)
    {
        delete mpScreenShotEditer;
        mpScreenShotEditer = nullptr;
    }
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
    //this->EnumChildWindowRecursively(desktopWnd);
    //this->EnumZOrderWindow();
    this->EnumTopLevelWindow();
    this->UpdateLogInfo();

    QScreen* primaryScreen = QGuiApplication::primaryScreen();
    QString filename = QString("E:\\Temp\\ScreenShots\\QtGrabberTest_%1.png")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz"));
    QPixmap shotImag = primaryScreen->grabWindow(0);

    mpScreenShotEditer->SetScreenShotImage(shotImag);
    mpScreenShotEditer->SetWindowRectList(mWinRectList);
    mpScreenShotEditer->show();

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

void MiscellaneousImageGrabber::on_btnEnumWindow_clicked()
{
    this->EnumTopLevelWindow();
}

void MiscellaneousImageGrabber::UpdateLogInfo()
{
    static int i = 0;
    RECT curWinRect = { 0 };
    ::GetWindowRect(mCurMouseWin, &curWinRect);

    QString logContent = QString("LogInfo Test %1\n").arg(++i);
    logContent += QString("Cur Win : 0x%1\n").arg(QString("%1").arg(reinterpret_cast<qulonglong>(mCurMouseWin), 8, 16, QChar('0')).toUpper());;
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

void MiscellaneousImageGrabber::GetWindowTitleString(HWND hWnd, std::string& wndTitle)
{
    char titleStr[256] = { 0 };
    ::GetWindowText(hWnd, titleStr, 256);
    wndTitle = titleStr;
}

void MiscellaneousImageGrabber::EnumTopLevelWindow()
{
    mWinRectList.clear();
    ::EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(this));
}

void MiscellaneousImageGrabber::EnumZOrderWindow()
{
    HWND winTopLevel = ::GetForegroundWindow();
    do 
    {
        RECT winRect = { 0 };
        if (::GetWindowRect(winTopLevel, &winRect) && winRect.right > winRect.left && winRect.bottom > winRect.top)
        {
            mWinRectList.push_back(winRect);
        }
        winTopLevel = ::GetNextWindow(winTopLevel, GW_HWNDNEXT);
    } while (winTopLevel != nullptr);
}

void MiscellaneousImageGrabber::EnumChildWindowRecursively(HWND parentWnd)
{
    ::EnumChildWindows(parentWnd, EnumChildWindowProc, reinterpret_cast<LPARAM>(this));
}

BOOL MiscellaneousImageGrabber::EnumWindowProcShadow(HWND hwnd)
{
    std::string wndClassName;
    this->GetWindowClassName(hwnd, wndClassName);

    std::string winTitle;
    this->GetWindowTitleString(hwnd, winTitle);

    if (::IsWindow(hwnd) && ::IsWindowVisible(hwnd))
    {
        RECT winRect = { 0 };
        if (::GetWindowRect(hwnd, &winRect) && winRect.right > winRect.left && winRect.bottom > winRect.top)
        {
            mWinRectList.push_back(winRect);
        }
    }

    LogUtil::Debug(CODE_LOCATION, "hwnd=0x%08X wndClassName=%s wndTitle=%s", hwnd, wndClassName.c_str(), winTitle.c_str());

    return TRUE;
}

BOOL MiscellaneousImageGrabber::EnumChildWindowProcShadow(HWND childWnd)
{
    std::string wndClassName;
    this->GetWindowClassName(childWnd, wndClassName);
    //LogUtil::Debug(CODE_LOCATION, "childWnd=0x%08X wndClassName=%s", childWnd, wndClassName.c_str());

    ::EnumChildWindows(childWnd, EnumChildWindowProc, reinterpret_cast<LPARAM>(this));

    if (::IsWindow(childWnd) && ::IsWindowVisible(childWnd))
    {
        RECT winRect = { 0 };
        ::GetWindowRect(childWnd, &winRect);
        mWinRectList.emplace_back(winRect);
        //LogUtil::Debug(CODE_LOCATION, "childWnd=0x%08X wndClassName=%s", childWnd, wndClassName.c_str());
    }

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

BOOL CALLBACK MiscellaneousImageGrabber::EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM lParam)
{
    MiscellaneousImageGrabber* pThis = reinterpret_cast<MiscellaneousImageGrabber*>(lParam);
    return pThis->EnumWindowProcShadow(hwnd);
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

ScreenShotEditer::ScreenShotEditer(QWidget* parent /* = nullptr */)
    : QWidget(parent)
{
    if (this->objectName().isEmpty())
    {
        this->setObjectName("ScreenShotEditer-Testing");
    }
    QDesktopWidget* pDesktop = QApplication::desktop();
    QPalette palette1 = this->palette();
    palette1.setColor(QPalette::Background, Qt::transparent);
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint);
    this->setGeometry(0, 0, pDesktop->width(), pDesktop->height());
    this->setAutoFillBackground(true);
    this->setPalette(palette1);
    this->setMouseTracking(true);

    mDesktopRect = pDesktop->rect();
}

ScreenShotEditer::~ScreenShotEditer()
{
    ;
}

void ScreenShotEditer::SetScreenShotImage(const QPixmap& pixmap)
{
    mScreenShot = pixmap;
}

void ScreenShotEditer::SetWindowRectList(const std::list<RECT>& winRectList)
{
    mWinRectList = winRectList;
}

void ScreenShotEditer::mouseMoveEvent(QMouseEvent* event)
{
    mCurMousePos = event->pos();
    int x = mCurMousePos.x();
    int y = mCurMousePos.y();

    auto winRectIt = mWinRectList.begin();
    auto winRectEnd = mWinRectList.end();
    while (winRectIt != winRectEnd)
    {
        if (winRectIt->left <= x && x <= winRectIt->right && winRectIt->top <= y && y <= winRectIt->bottom)
        {
            mCurWinRect = QRect(QPoint(winRectIt->left, winRectIt->top), QPoint(winRectIt->right - 1, winRectIt->bottom - 1)); // ¼õÈ¥±ß¿ò¿í¶È
            this->update();
            break;
        }
        winRectIt++;
    }

    //auto winRectIt = mWinRectList.rbegin();
    //auto winRectEnd = mWinRectList.rend();
    //mCurWinRect = mDesktopRect;
    //while (winRectIt != winRectEnd)
    //{
    //    if (winRectIt->left <= x && x <= winRectIt->right && winRectIt->top <= y && y <= winRectIt->bottom)
    //    {
    //        QRect winRect(QPoint(winRectIt->left, winRectIt->top), QPoint(winRectIt->right, winRectIt->bottom));
    //        if (mCurWinRect != winRect && mCurWinRect.contains(winRect.topLeft()) && mCurWinRect.contains(winRect.bottomRight()))
    //        {
    //            mCurWinRect = winRect;
    //            this->update();
    //        }
    //    }
    //    winRectIt++;
    //}
}

void ScreenShotEditer::paintEvent(QPaintEvent* event)
{
    QPen rectPen(Qt::red);
    rectPen.setWidth(1);
    QPainter painter(this);
    painter.drawPixmap(this->rect(), mScreenShot);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(rectPen);
    painter.drawRect(mCurWinRect);

    //rectPen.setColor(Qt::blue);
    //painter.setPen(rectPen);
    //painter.drawRect(QRect(100, 100, 300, 200));
}

void ScreenShotEditer::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        this->hide();
    }
}
