#include "MiscellaneousQNativeWindow.h"
#include "ui_MiscellaneousQNativeWindow.h"

#include <QWindow>
#include <QTimer>

#include "Utils/TimeUtil.h"
#include "LogUtil.h"

using namespace Shuanglong::Utils;

MiscellaneousQNativeWindow::MiscellaneousQNativeWindow(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousQNativeWindow())
    , m_pDevice(nullptr)
    , m_pDeviceContext(nullptr)
    , m_pSwapChain(nullptr)
    , m_pDevice1(nullptr)
    , m_pDeviceContext1(nullptr)
    , m_pSwapChain1(nullptr)
    , m_pDepthStencilBuffer(nullptr)
    , m_pRenderTargetView(nullptr)
    , m_pDepthStencilView(nullptr)
    , m_4xMsaaQuality(0)
    , m_4xMsaaEnabled(true)
    , m_bInitialized3D(false)
{
    ui->setupUi(this);
    ZeroMemory(&m_ScreenViewport, sizeof(D3D11_VIEWPORT));

    this->setAttribute(Qt::WA_NativeWindow, true);
    this->setAttribute(Qt::WA_DontCreateNativeAncestors, true);
    ////QGuiApplication::setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);

    this->InitializeDirect3D();

    QTimer* pUpdateTimer3D = new QTimer(this);
    pUpdateTimer3D->setInterval(17);
    pUpdateTimer3D->start();
    this->connect(pUpdateTimer3D, SIGNAL(timeout()), this, SLOT(SlotUpdateViewContent3D_TimeOut()));
}

MiscellaneousQNativeWindow::~MiscellaneousQNativeWindow()
{
    delete ui;
}

QString MiscellaneousQNativeWindow::GetTitle()
{
    return QObject::tr("Native Window Test");
}

QString MiscellaneousQNativeWindow::GetTitleTooltip()
{
    return QObject::tr("How to embed a native window in QT application.");
}

MiscellaneousTestGroup MiscellaneousQNativeWindow::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousQNativeWindow::GetItemID()
{
    return MiscellaneousTestItem::QT_Native_Window;
}

void MiscellaneousQNativeWindow::resizeEvent(QResizeEvent* event)
{
    this->ResizeBufferAndTargetView();
}

void MiscellaneousQNativeWindow::InitializeDirect3D()
{
    HRESULT hResult = S_OK;
    UINT createFlags = D3D11_CREATE_DEVICE_DEBUG;
    D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_HARDWARE;
    D3D_FEATURE_LEVEL featureLevel;
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);
    hResult = ::D3D11CreateDevice(nullptr, driverType, nullptr, createFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, m_pDevice.GetAddressOf(), &featureLevel, m_pDeviceContext.GetAddressOf());
    if (hResult == E_INVALIDARG)
    {
        hResult = ::D3D11CreateDevice(nullptr, driverType, nullptr, createFlags, &featureLevels[1], numFeatureLevels - 1, D3D11_SDK_VERSION, m_pDevice.GetAddressOf(), &featureLevel, m_pDeviceContext.GetAddressOf());
    }
    if (FAILED(hResult))
    {
        LogUtil::Error(CODE_LOCATION, "D3D11CreateDevice failed");
        return;
    }
    hResult = m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);

    ComPtr<IDXGIDevice> dxgiDevice = nullptr;
    ComPtr<IDXGIAdapter> dxgiAdapter = nullptr;
    ComPtr<IDXGIFactory1> dxgiFactory1 = nullptr;
    ComPtr<IDXGIFactory2> dxgiFactory2 = nullptr;

    hResult = m_pDevice.As(&dxgiDevice);
    hResult = dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
    hResult = dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(dxgiFactory1.GetAddressOf()));

    hResult = dxgiFactory1.As(&dxgiFactory2);
    if (dxgiFactory2 != nullptr)
    {
        hResult = m_pDevice.As(&m_pDevice1);
        hResult = m_pDeviceContext.As(&m_pDeviceContext1);
        DXGI_SWAP_CHAIN_DESC1 sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.Width = ui->nativeWindow->geometry().width();
        sd.Height = ui->nativeWindow->geometry().height();
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        if (m_4xMsaaEnabled)
        {
            sd.SampleDesc.Count = 4;
            sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
        }
        else
        {
            sd.SampleDesc.Count = 1;
            sd.SampleDesc.Quality = 0;
        }
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fd;
        fd.RefreshRate.Numerator = 60;
        fd.RefreshRate.Denominator = 1;
        fd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        fd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        fd.Windowed = TRUE;
        // 为当前窗口创建交换链
        hResult = dxgiFactory2->CreateSwapChainForHwnd(m_pDevice.Get(), (HWND)ui->nativeWindow->winId(), &sd, &fd, nullptr, m_pSwapChain1.GetAddressOf());
        hResult = m_pSwapChain1.As(&m_pSwapChain);
    }
    else
    {
        // 填充DXGI_SWAP_CHAIN_DESC用以描述交换链
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferDesc.Width = ui->nativeWindow->geometry().width();
        sd.BufferDesc.Height = ui->nativeWindow->geometry().height();
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        if (m_4xMsaaEnabled)
        {
            sd.SampleDesc.Count = 4;
            sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
        }
        else
        {
            sd.SampleDesc.Count = 1;
            sd.SampleDesc.Quality = 0;
        }
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 1;
        sd.OutputWindow = (HWND)ui->nativeWindow->winId();
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0;
        hResult = dxgiFactory1->CreateSwapChain(m_pDevice.Get(), &sd, m_pSwapChain.GetAddressOf());
    }

    dxgiFactory1->MakeWindowAssociation((HWND)ui->nativeWindow->winId(), DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);

    // 设置调试对象名
    std::string contextName = "ImmediateContext";
    m_pDeviceContext->SetPrivateData(WKPDID_D3DDebugObjectName, contextName.length(), contextName.c_str());
    std::string swapChainName = "SwapChain";
    m_pSwapChain->SetPrivateData(WKPDID_D3DDebugObjectName, swapChainName.length(), swapChainName.c_str());

    // 设置缓冲区 及 渲染目标试图的大小
    this->ResizeBufferAndTargetView();

    m_bInitialized3D = true;
}

void MiscellaneousQNativeWindow::ResizeBufferAndTargetView()
{
    assert(m_pDevice);
    assert(m_pDeviceContext);
    assert(m_pSwapChain);

    if (m_pDevice1 != nullptr)
    {
        assert(m_pDeviceContext1);
        assert(m_pDevice1);
        assert(m_pSwapChain1);
    }

    // 释放渲染管线输出用到的相关资源
    m_pRenderTargetView.Reset();
    m_pDepthStencilView.Reset();
    m_pDepthStencilBuffer.Reset();

    // 重设交换链并且重新创建渲染目标视图
    HRESULT hResult = S_OK;
    ComPtr<ID3D11Texture2D> backBuffer;
    hResult = m_pSwapChain->ResizeBuffers(1, ui->nativeWindow->geometry().width(), ui->nativeWindow->geometry().height(), DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    hResult = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
    hResult = m_pDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_pRenderTargetView.GetAddressOf());

    // 设置调试对象名
    //D3D11SetDebugObjectName(backBuffer.Get(), "BackBuffer[0]");

    backBuffer.Reset();


    D3D11_TEXTURE2D_DESC depthStencilDesc;

    depthStencilDesc.Width = ui->nativeWindow->geometry().width();
    depthStencilDesc.Height = ui->nativeWindow->geometry().height();
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    // 要使用 4X MSAA? --需要给交换链设置MASS参数
    if (m_4xMsaaEnabled)
    {
        depthStencilDesc.SampleDesc.Count = 4;
        depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
    }
    else
    {
        depthStencilDesc.SampleDesc.Count = 1;
        depthStencilDesc.SampleDesc.Quality = 0;
    }

    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    // 创建深度缓冲区以及深度模板视图
    hResult = m_pDevice->CreateTexture2D(&depthStencilDesc, nullptr, m_pDepthStencilBuffer.GetAddressOf());
    hResult = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, m_pDepthStencilView.GetAddressOf());


    // 将渲染目标视图和深度/模板缓冲区结合到管线
    m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

    // 设置视口变换
    m_ScreenViewport.TopLeftX = 0;
    m_ScreenViewport.TopLeftY = 0;
    m_ScreenViewport.Width = static_cast<float>(ui->nativeWindow->geometry().width());
    m_ScreenViewport.Height = static_cast<float>(ui->nativeWindow->geometry().height());
    m_ScreenViewport.MinDepth = 0.0f;
    m_ScreenViewport.MaxDepth = 1.0f;

    m_pDeviceContext->RSSetViewports(1, &m_ScreenViewport);

    // 设置调试对象名
    //D3D11SetDebugObjectName(m_pDepthStencilBuffer.Get(), "DepthStencilBuffer");
    //D3D11SetDebugObjectName(m_pDepthStencilView.Get(), "DepthStencilView");
    //D3D11SetDebugObjectName(m_pRenderTargetView.Get(), "BackBufferRTV[0]");
}

void MiscellaneousQNativeWindow::UpdateViewContent3D()
{
    assert(m_pDeviceContext);
    assert(m_pSwapChain);
    static float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };	// RGBA = (0,0,255,255)
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), blue);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_pSwapChain->Present(0, 0);
}

void MiscellaneousQNativeWindow::SlotUpdateViewContent3D_TimeOut()
{
    if (m_bInitialized3D)
    {
        this->UpdateViewContent3D();
    }
}

void MiscellaneousQNativeWindow::on_btnEmptyTest1_clicked()
{
    // 模拟重绘
    //this->UpdateViewContent3D();
}

void MiscellaneousQNativeWindow::on_btnEmptyTest2_clicked()
{
}

void MiscellaneousQNativeWindow::on_btnEmptyTest3_clicked()
{
    QWindowList windowList = QGuiApplication::allWindows();
    int windowCount = windowList.count();
    for (int i = 0; i < windowCount; ++i)
    {
        QWindow *pWindow = windowList[i];
    }
    int i = 0;
}

void MiscellaneousQNativeWindow::on_btnEmptyTest4_clicked()
{
    int i = 0;
}
