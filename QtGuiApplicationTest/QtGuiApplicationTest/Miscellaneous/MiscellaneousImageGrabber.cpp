#include "MiscellaneousImageGrabber.h"
#include "ui_MiscellaneousImageGrabber.h"

// Windows Headers
#include <d3d11_1.h>
#include <dwmapi.h>

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
    , mpDevice(nullptr)
    , mpDeviceCtx(nullptr)
    , mpDxgiOutputDuplication(nullptr)
{
    ui->setupUi(this);
    this->setMouseTracking(true);

    mpScreenShotEditer = new ScreenShotEditer();
    this->Init3D();

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
    for (auto& monitorDxgiOutputIt : mMonitorDxgiOutput)
    {
        if (monitorDxgiOutputIt.second != nullptr)
        {
            monitorDxgiOutputIt.second->Release();
            monitorDxgiOutputIt.second = nullptr;
        }
    }
    mMonitorDxgiOutput.clear();
    this->Uninit3D();
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

void MiscellaneousImageGrabber::on_btnGrabDxgi_clicked()
{
    IDXGIResource* pDesktopResource = nullptr;
    DXGI_OUTDUPL_FRAME_INFO dxgiOutduplFrameInfo = { 0 };
    mpDxgiOutputDuplication->AcquireNextFrame(0, &dxgiOutduplFrameInfo, &pDesktopResource);

    ID3D11Texture2D* pD3dTexture2D = nullptr;
    pDesktopResource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pD3dTexture2D));
    pDesktopResource->Release();

    D3D11_TEXTURE2D_DESC d3d11Texture2dDesc = { 0 };
    pD3dTexture2D->GetDesc(&d3d11Texture2dDesc);
    d3d11Texture2dDesc.Usage = D3D11_USAGE_STAGING;
    d3d11Texture2dDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    d3d11Texture2dDesc.BindFlags = 0;
    d3d11Texture2dDesc.MiscFlags = 0;
    d3d11Texture2dDesc.MipLevels = 1;
    d3d11Texture2dDesc.ArraySize = 1;
    d3d11Texture2dDesc.SampleDesc.Count = 1;

    ID3D11Texture2D* pNewDesktopImage = nullptr;
    mpDevice->CreateTexture2D(&d3d11Texture2dDesc, nullptr, &pNewDesktopImage);

    mpDeviceCtx->CopyResource(pNewDesktopImage, pD3dTexture2D);
    pD3dTexture2D->Release();
    mpDxgiOutputDuplication->ReleaseFrame();

    IDXGISurface* pDxgiSurface = nullptr;
    pNewDesktopImage->QueryInterface(__uuidof(IDXGISurface), reinterpret_cast<void**>(&pDxgiSurface));
    pNewDesktopImage->Release();

    DXGI_MAPPED_RECT mappedRect;
    pDxgiSurface->Map(&mappedRect, DXGI_MAP_READ);
    //memcpy((BYTE*)pImgData, mappedRect.pBits, dxgiOutputDesc.DesktopCoordinates.right * dxgiOutputDesc.DesktopCoordinates.bottom * 4);
    pDxgiSurface->Unmap();
    pDxgiSurface->Release();
}

void MiscellaneousImageGrabber::on_btnDxgiEnum_clicked()
{
    POINT curMousePos = { 0,0 };
    ::GetCursorPos(&curMousePos);
    HMONITOR curMonitor = ::MonitorFromPoint(curMousePos, MONITOR_DEFAULTTONEAREST);
    // ö������������
    IDXGIFactory *mpDxgiFactory = nullptr;
    HRESULT hResult = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void **>(&mpDxgiFactory));
    if (FAILED(hResult))
    {
        LogUtil::Error(CODE_LOCATION, "CreateDXGIFactory failed !");
        return;
    }
    IDXGIAdapter *pTempAdapter = nullptr;
    for (UINT i = 0; mpDxgiFactory->EnumAdapters(i, &pTempAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
    {
        if (pTempAdapter)
        {
            DXGI_ADAPTER_DESC tempDxgiAdapterDesc = { 0 };
            pTempAdapter->GetDesc(&tempDxgiAdapterDesc);
            UINT outputNum = 0;

            do 
            {
                IDXGIOutput *pDxgiOutput = nullptr;
                hResult = pTempAdapter->EnumOutputs(outputNum, &pDxgiOutput);

                if (SUCCEEDED(hResult))
                {
                    DXGI_OUTPUT_DESC dxgiOutputDesc = { 0 };
                    pDxgiOutput->GetDesc(&dxgiOutputDesc);
                    mMonitorDxgiOutput[dxgiOutputDesc.Monitor] = pDxgiOutput;

                    if (curMonitor == dxgiOutputDesc.Monitor)
                    {
                        int i = 0;
                    }
                }
                ++outputNum;
            } while (hResult != DXGI_ERROR_NOT_FOUND);

            pTempAdapter->Release();
            pTempAdapter = nullptr;

            int i = 0;
        }
    }
    if (mpDxgiFactory)
    {
        mpDxgiFactory->Release();
        mpDxgiFactory = nullptr;
    }
}

void MiscellaneousImageGrabber::on_btnEnumWindow_clicked()
{
    this->EnumTopLevelWindow();
}

void MiscellaneousImageGrabber::Init3D()
{
    HRESULT hResult = S_OK;
    UINT createFlags = D3D11_CREATE_DEVICE_DEBUG;
    D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_WARP;
    D3D_FEATURE_LEVEL featureLevel;
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);
    hResult = ::D3D11CreateDevice(nullptr, driverType, nullptr, createFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &mpDevice, &featureLevel, &mpDeviceCtx);
    if (hResult == E_INVALIDARG)
    {
        hResult = ::D3D11CreateDevice(nullptr, driverType, nullptr, createFlags, &featureLevels[1], numFeatureLevels - 1, D3D11_SDK_VERSION, &mpDevice, &featureLevel, &mpDeviceCtx);
    }
    if (FAILED(hResult))
    {
        LogUtil::Error(CODE_LOCATION, "D3D11CreateDevice failed !");
        return;
    }

    IDXGIDevice *pDxgiDevice = nullptr;
    hResult = mpDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&pDxgiDevice));
    IDXGIAdapter *pDxgiAdapter = nullptr;
    hResult = pDxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void **>(&pDxgiAdapter));
    //hResult = pDxgiDevice->GetAdapter(&pDxgiAdapter);

    DXGI_ADAPTER_DESC dxgiAdapterDesc = { 0 };
    pDxgiAdapter->GetDesc(&dxgiAdapterDesc);

    UINT outputNum = 0;
    IDXGIOutput* pDxgiOutput = nullptr;
    hResult = pDxgiAdapter->EnumOutputs(outputNum, &pDxgiOutput);
    pDxgiAdapter->Release();
    if (FAILED(hResult))
    {
        LogUtil::Error(CODE_LOCATION, "EnumOutputs failed !");
        return;
    }

    DXGI_OUTPUT_DESC dxgiOutputDesc = { 0 };
    pDxgiOutput->GetDesc(&dxgiOutputDesc);

    IDXGIOutput1 *pDxgiOuput1 = nullptr;
    pDxgiOutput->QueryInterface(__uuidof(IDXGIOutput1), reinterpret_cast<void**>(&pDxgiOuput1));
    pDxgiOutput->Release();

    if (pDxgiDevice != nullptr)
    {
        pDxgiOuput1->DuplicateOutput(pDxgiDevice, &mpDxgiOutputDuplication);
        pDxgiOuput1->Release();
        pDxgiDevice->Release();
    }

    DXGI_OUTDUPL_DESC dxgiOutduplDesc = { 0 };
    mpDxgiOutputDuplication->GetDesc(&dxgiOutduplDesc);
    if (dxgiOutduplDesc.DesktopImageInSystemMemory)
    {
    } 
    else
    {
    }
}

void MiscellaneousImageGrabber::Uninit3D()
{
    if (mpDxgiOutputDuplication)
    {
        mpDxgiOutputDuplication->Release();
        mpDxgiOutputDuplication = nullptr;
    }
    if (mpDeviceCtx)
    {
        mpDeviceCtx->Release();
        mpDeviceCtx = nullptr;
    }
    if (mpDevice)
    {
        mpDevice->Release();
        mpDevice;
    }
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
            std::string className, winTitle;
            this->GetWindowClassName(winTopLevel, className);
            this->GetWindowTitleString(winTopLevel, winTitle);
            CaptureWindowInfo winInfo;
            winInfo.winHandle = winTopLevel;
            winInfo.winRect = winRect;
            winInfo.className = className;
            winInfo.winTitle = winTitle;
            if (className != "popupshadow")
            {
                mWinRectList.push_back(winInfo);
            }
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
        RECT clientRect = { 0 };
        WINDOWINFO windowInfo = { 0 };
        RECT dwmRect = { 0 };
        HRESULT dwmWniRes = DwmGetWindowAttribute(hwnd, DWMWA_EXTENDED_FRAME_BOUNDS, &dwmRect, sizeof(RECT));

        if (::GetClientRect(hwnd, &clientRect) && ::GetWindowRect(hwnd, &winRect) && ::GetWindowInfo(hwnd, &windowInfo)
            && winRect.right > winRect.left && winRect.bottom > winRect.top)
        {
            RECT realRect = { 0 };
            realRect.left = winRect.left + clientRect.left;
            realRect.top = winRect.top;
            realRect.right = winRect.left + clientRect.right;
            realRect.bottom = winRect.top + clientRect.bottom;

            std::string className, winTitle;
            this->GetWindowClassName(hwnd, className);
            this->GetWindowTitleString(hwnd, winTitle);
            CaptureWindowInfo winInfo;
            winInfo.winHandle = hwnd;
            winInfo.winRect = winRect;
            winInfo.clientRect = clientRect;
            winInfo.realRect = realRect;
            winInfo.rcWindow = windowInfo.rcWindow;
            winInfo.rcClient = windowInfo.rcClient;
            winInfo.dwmRect = dwmRect;
            winInfo.className = className;
            winInfo.winTitle = winTitle;
            if (className.find("shadow") == std::string::npos && className.find("Shadow") == std::string::npos)
            {
                mWinRectList.push_back(winInfo);
            }
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
        std::string className, winTitle;
        this->GetWindowClassName(childWnd, className);
        this->GetWindowTitleString(childWnd, winTitle);
        CaptureWindowInfo winInfo;
        winInfo.winHandle = childWnd;
        winInfo.winRect = winRect;
        winInfo.className = className;
        winInfo.winTitle = winTitle;
        if (className != "popupshadow")
        {
            mWinRectList.push_back(winInfo);
        }
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
    this->setWindowFlags(this->windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    this->setGeometry(0, 0, pDesktop->width(), pDesktop->height());
    this->setAutoFillBackground(true);
    this->setPalette(palette1);
    this->setMouseTracking(true);
    //this->installEventFilter(this);

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

void ScreenShotEditer::SetWindowRectList(const std::list<CaptureWindowInfo>& winRectList)
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
        if (winRectIt->winRect.left <= x && x <= winRectIt->winRect.right && winRectIt->winRect.top <= y && y <= winRectIt->winRect.bottom)
        {
            mCurWinRect = *winRectIt;
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
    rectPen.setWidth(2);
    QPainter painter(this);
    painter.drawPixmap(this->rect(), mScreenShot);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(rectPen);

    //QRect winRect = QRect(QPoint(mCurWinRect.winRect.left, mCurWinRect.winRect.top), QPoint(mCurWinRect.winRect.right - 2, mCurWinRect.winRect.bottom - 2)); // ��ȥ�߿����
    QRect winRect = QRect(QPoint(mCurWinRect.dwmRect.left, mCurWinRect.dwmRect.top), QPoint(mCurWinRect.dwmRect.right - 2, mCurWinRect.dwmRect.bottom - 2)); // ��ȥ�߿����
    painter.drawRect(winRect);

    QPen textPen(qRgb(255, 0, 255));
    textPen.setWidth(1);
    painter.setPen(textPen);

    QFont textFont;
    textFont.setBold(true);
    textFont.setPointSize(16);
    painter.setFont(textFont);
    QString testStr = QString::fromLocal8Bit(mCurWinRect.winTitle.c_str());

    QPoint textPoint(winRect.left() + 10, winRect.top() + 20);
    painter.drawText(textPoint, QString("ClassName: %1").arg(QString::fromStdString(mCurWinRect.className)));
    textPoint += QPoint(0, 20);
    painter.drawText(textPoint, QString("WinTitle: %1").arg(QString::fromLocal8Bit(mCurWinRect.winTitle.c_str())));
    textPoint += QPoint(0, 20);
    painter.drawText(textPoint, QString("HWND: 0x%1").arg((ulong)mCurWinRect.winHandle, 8, 16, QChar('0')));
    textPoint += QPoint(0, 20);
    painter.drawText(textPoint, QString("WinRect: (%1, %2) (%3, %4)").arg(mCurWinRect.winRect.left).arg(mCurWinRect.winRect.top)
        .arg(mCurWinRect.winRect.right - mCurWinRect.winRect.left).arg(mCurWinRect.winRect.bottom - mCurWinRect.winRect.top));
    textPoint += QPoint(0, 20);
    painter.drawText(textPoint, QString("ClientRect: (%1, %2) (%3, %4)").arg(mCurWinRect.clientRect.left).arg(mCurWinRect.clientRect.top)
        .arg(mCurWinRect.clientRect.right - mCurWinRect.clientRect.left).arg(mCurWinRect.clientRect.bottom - mCurWinRect.clientRect.top));
    textPoint += QPoint(0, 20);
    painter.drawText(textPoint, QString("RealRect: (%1, %2) (%3, %4)").arg(mCurWinRect.realRect.left).arg(mCurWinRect.realRect.top)
        .arg(mCurWinRect.realRect.right - mCurWinRect.realRect.left).arg(mCurWinRect.realRect.bottom - mCurWinRect.realRect.top));
    textPoint += QPoint(0, 20);
    painter.drawText(textPoint, QString("RcWindow: (%1, %2) (%3, %4)").arg(mCurWinRect.rcWindow.left).arg(mCurWinRect.rcWindow.top)
        .arg(mCurWinRect.rcWindow.right - mCurWinRect.rcWindow.left).arg(mCurWinRect.rcWindow.bottom - mCurWinRect.rcWindow.top));
    textPoint += QPoint(0, 20);
    painter.drawText(textPoint, QString("RcClient: (%1, %2) (%3, %4)").arg(mCurWinRect.rcClient.left).arg(mCurWinRect.rcClient.top)
        .arg(mCurWinRect.rcClient.right - mCurWinRect.rcClient.left).arg(mCurWinRect.rcClient.bottom - mCurWinRect.rcClient.top));
    textPoint += QPoint(0, 20);
    painter.drawText(textPoint, QString("DwmRect: (%1, %2) (%3, %4)").arg(mCurWinRect.dwmRect.left).arg(mCurWinRect.dwmRect.top)
        .arg(mCurWinRect.dwmRect.right - mCurWinRect.dwmRect.left).arg(mCurWinRect.dwmRect.bottom - mCurWinRect.dwmRect.top));

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

bool ScreenShotEditer::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() == QEvent::KeyRelease)
    {
        QKeyEvent* pKeyEvent = static_cast<QKeyEvent*>(event);
        if (pKeyEvent->key() == Qt::Key_Escape)
        {
            this->hide();
            return true;
        }
        else if (pKeyEvent->key() == Qt::Key_Menu)
        {
            int i = 0;
        }
        else if (pKeyEvent->key() == Qt::Key_SysReq)
        {
            int i = 0;
        }
    }
    return false;
}
