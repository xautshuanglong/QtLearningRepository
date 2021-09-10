#include "MiscellaneousQNativeWindow.h"
#include "ui_MiscellaneousQNativeWindow.h"

#include <d3dcompiler.h>
#include <QWindow>
#include <QTimer>

#include "Utils/TimeUtil.h"
#include "JCB_Logger/LogUtil.h"

#include "DirectX/DDSTextureLoader.h"

using namespace Shuanglong::Utils;

const D3D11_INPUT_ELEMENT_DESC MiscellaneousQNativeWindow::VertexPosColor::inputLayout[2] =
{
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

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
    , m_pVertexLayout(nullptr)
    , m_pVertexBufferTriangle(nullptr)
    , m_pVertexShaderTriangle(nullptr)
    , m_pPixelShaderTriangle(nullptr)
    , m_pVertexBufferCube(nullptr)
    , m_pIndexBufferCube(nullptr)
    , m_pConstantBuffer(nullptr)
    , m_pVertexShaderCube(nullptr)
    , m_pPixelShaderCube(nullptr)
    , m_pVertexBufferCubeTexture(nullptr)
    , m_pResourceViewWood(nullptr)
    , m_pSamplerState(nullptr)
    , m_pVertexBufferCoor(nullptr)
    , m_pVertexShaderCoor(nullptr)
    , m_pPixelShaderCoor(nullptr)
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
    this->InitializeDirectShaders();
    this->InitializeDirectVertices();
    this->ResizeBufferAndTargetView();

    QTimer* pUpdateTimer3D = new QTimer(this);
    pUpdateTimer3D->setInterval(1);
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

    m_bInitialized3D = true;
}

void MiscellaneousQNativeWindow::InitializeDirectShaders()
{
    ComPtr<ID3DBlob> blob;
    HRESULT hResult = S_OK;

    //------------------------------ triangle --------------------------------------

    hResult = CreateShaderFromFile(L"Triangle_VS.cso", L"Shaders\\Triangle_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf());
    hResult = m_pDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShaderTriangle.GetAddressOf());

    hResult = m_pDevice->CreateInputLayout(VertexPosColor::inputLayout, ARRAYSIZE(VertexPosColor::inputLayout), blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf());

    hResult = CreateShaderFromFile(L"Triangle_PS.cso", L"Shaders\\Triangle_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf());
    hResult = m_pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShaderTriangle.GetAddressOf());

    //------------------------------ coordinate system --------------------------------------

    hResult = CreateShaderFromFile(L"CoordinateSystem_VS.cso", L"Shaders\\CoordinateSystem_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf());
    hResult = m_pDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShaderCoor.GetAddressOf());

    hResult = CreateShaderFromFile(L"CoordinateSystem_PS.cso", L"Shaders\\CoordinateSystem_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf());
    hResult = m_pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShaderCoor.GetAddressOf());

    //------------------------------ cube --------------------------------------

    hResult = CreateShaderFromFile(L"Cube_VS.cso", L"Shaders\\Cube_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf());
    hResult = m_pDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShaderCube.GetAddressOf());

    hResult = CreateShaderFromFile(L"Cube_PS.cso", L"Shaders\\Cube_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf());
    hResult = m_pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShaderCube.GetAddressOf());
}

void MiscellaneousQNativeWindow::InitializeDirectTextures()
{
    HRESULT hResult = S_OK;
    // 初始化木箱纹理
    hResult = DirectX::CreateDDSTextureFromFile(m_pDevice.Get(), L"..\\Texture\\WoodCrate.dds", nullptr, m_pResourceViewWood.GetAddressOf());
    // 初始化火焰纹理
    //WCHAR strFile[40];
    //m_pFireAnims.resize(120);
    //for (int i = 1; i <= 120; ++i)
    //{
    //    wsprintf(strFile, L"..\\Texture\\FireAnim\\Fire%03d.bmp", i);
    //    HR(CreateWICTextureFromFile(m_pd3dDevice.Get(), strFile, nullptr, m_pFireAnims[static_cast<size_t>(i) - 1].GetAddressOf()));
    //}
}

void MiscellaneousQNativeWindow::InitializeDirectSamplers()
{
    HRESULT hResult = S_OK;
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hResult = m_pDevice->CreateSamplerState(&sampDesc, m_pSamplerState.GetAddressOf());
}

void MiscellaneousQNativeWindow::InitializeDirectVertices()
{
    HRESULT hResult = S_OK;

    // 常量缓冲区 世界坐标系、投影、视图（相机位置 视线朝向 相机方向）。
    D3D11_BUFFER_DESC cbd;
    ZeroMemory(&cbd, sizeof(cbd));
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.ByteWidth = sizeof(ConstantBuffer);
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hResult = m_pDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffer.GetAddressOf());
    m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pConstantBuffer.GetAddressOf());

    float ratio = ui->nativeWindow->geometry().width() * 1.0 / ui->nativeWindow->geometry().height();
    m_CBuffer.world = DirectX::XMMatrixIdentity();
    m_CBuffer.projection = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, ratio, 1.0f, 1000.0f));
    m_CBuffer.view = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(
        DirectX::XMVectorSet(0.3f, 0.5f, -2.3f, 0.0f),
        DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
        DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
    ));

    // 旋转矩阵初始化
    m_matrixWorld = DirectX::XMMatrixIdentity();

    // 采样器初始化
    this->InitializeDirectSamplers();

    // 模型定点初始化
    this->InitializeDirectVertices_Coordinate();
    this->InitializeDirectVertices_Triangle();
    this->InitializeDirectVertices_TriangleTexture();
    this->InitializeDirectVertices_Cube();
    this->InitializeDirectVertices_CubeTexture();
}

HRESULT MiscellaneousQNativeWindow::InitializeDirectVertices_Coordinate()
{
    HRESULT hResult = S_OK;
    VertexPosColor verticesCoor[] =
    {
        { DirectX::XMFLOAT3(-1.0f,  0.0f,  0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(1.0f,  0.0f,  0.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.0f, -1.0f,  0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.0f,  1.0f,  0.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.0f,  0.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.0f,  0.0f,  1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
    };

    D3D11_BUFFER_DESC vbd_coor;
    ZeroMemory(&vbd_coor, sizeof(vbd_coor));
    vbd_coor.Usage = D3D11_USAGE_IMMUTABLE;
    vbd_coor.ByteWidth = sizeof(verticesCoor);
    vbd_coor.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd_coor.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = verticesCoor;
    hResult = m_pDevice->CreateBuffer(&vbd_coor, &InitData, m_pVertexBufferCoor.GetAddressOf());

    return hResult;
}

HRESULT MiscellaneousQNativeWindow::InitializeDirectVertices_Triangle()
{
    HRESULT hResult = S_OK;
    //         0           y     z
    //        /\           |    /
    //       /  \          |   /
    //      /    \         | /
    //     /______\        |/_________
    //    2        1       O          x
    VertexPosColor vertices[] =
    {
        { DirectX::XMFLOAT3(-0.50f, 0.75f, 0.5f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(-0.25f, 0.25f, 0.5f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { DirectX::XMFLOAT3(-0.75f, 0.25f, 0.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(-0.50f, 0.75f, 0.5f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(-0.75f, 0.25f, 0.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(-0.25f, 0.25f, 0.5f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) }
    };

    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(vertices);
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = vertices;
    hResult = m_pDevice->CreateBuffer(&vbd, &InitData, m_pVertexBufferTriangle.GetAddressOf());

    return hResult;
}

HRESULT MiscellaneousQNativeWindow::InitializeDirectVertices_TriangleTexture()
{
    HRESULT hResult = S_OK;

    return hResult;
}

HRESULT MiscellaneousQNativeWindow::InitializeDirectVertices_Cube()
{
    HRESULT hResult = S_OK;
    //    5________ 6      y     z
    //    /|      /|       |    /
    //   /_|_____/ |       |   /
    //  1|4|_ _ 2|_|7      |  /
    //   | /     | /       | /
    //   |/______|/        |/_________
    //  0       3          O          x
    VertexPosColor verticesCube[] =
    {
        { DirectX::XMFLOAT3(0.25f, 0.25f, 0.25f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.25f, 0.75f, 0.25f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.75f, 0.75f, 0.25f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.75f, 0.25f, 0.25f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.25f, 0.25f, 0.75f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.25f, 0.75f, 0.75f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.75f, 0.75f, 0.75f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.75f, 0.25f, 0.75f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
    };

    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(verticesCube);
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = verticesCube;
    hResult = m_pDevice->CreateBuffer(&vbd, &InitData, m_pVertexBufferCube.GetAddressOf());

    DWORD indices[] =
    {
        // 正面
        0, 1, 2,
        2, 3, 0,
        // 左面
        4, 5, 1,
        1, 0, 4,
        // 顶面
        1, 5, 6,
        6, 2, 1,
        // 背面
        7, 6, 5,
        5, 4, 7,
        // 右面
        3, 2, 6,
        6, 7, 3,
        // 底面
        4, 0, 3,
        3, 7, 4
    };

    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd, sizeof(ibd));
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(indices);
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;

    InitData.pSysMem = indices;
    hResult = m_pDevice->CreateBuffer(&ibd, &InitData, m_pIndexBufferCube.GetAddressOf());

    return hResult;
}

HRESULT MiscellaneousQNativeWindow::InitializeDirectVertices_CubeTexture()
{
    HRESULT hResult = S_OK;
    //    5________ 6      y     z
    //    /|      /|       |    /
    //   /_|_____/ |       |   /
    //  1|4|_ _ 2|_|7      |  /
    //   | /     | /       | /
    //   |/______|/        |/_________
    //  0       3          O          x
    VertexPosColor verticesCube[] =
    {
        { DirectX::XMFLOAT3(0.25f, -0.75f, -0.75f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.25f, -0.25f, -0.75f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.75f, -0.25f, -0.75f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.75f, -0.75f, -0.75f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.25f, -0.75f, -0.25f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.25f, -0.25f, -0.25f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.75f, -0.25f, -0.25f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { DirectX::XMFLOAT3(0.75f, -0.75f, -0.25f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
    };

    D3D11_BUFFER_DESC vbd;
    ZeroMemory(&vbd, sizeof(vbd));
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(verticesCube);
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    InitData.pSysMem = verticesCube;
    hResult = m_pDevice->CreateBuffer(&vbd, &InitData, m_pVertexBufferCubeTexture.GetAddressOf());

    DWORD indices[] =
    {
        // 正面
        0, 1, 2,
        2, 3, 0,
        // 左面
        4, 5, 1,
        1, 0, 4,
        // 顶面
        1, 5, 6,
        6, 2, 1,
        // 背面
        7, 6, 5,
        5, 4, 7,
        // 右面
        3, 2, 6,
        6, 7, 3,
        // 底面
        4, 0, 3,
        3, 7, 4
    };

    D3D11_BUFFER_DESC ibd;
    ZeroMemory(&ibd, sizeof(ibd));
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(indices);
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;

    InitData.pSysMem = indices;
    hResult = m_pDevice->CreateBuffer(&ibd, &InitData, m_pIndexBufferCube.GetAddressOf());

    return hResult;
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

void MiscellaneousQNativeWindow::UpdateViewContent3D(const DirectX::XMMATRIX& modelMatrix)
{
    m_CBuffer.world = DirectX::XMMatrixTranspose(modelMatrix * m_matrixWorld);
    float ratio = ui->nativeWindow->geometry().width() * 1.0 / ui->nativeWindow->geometry().height();
    m_CBuffer.projection = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2, ratio, 1.0f, 1000.0f));
    
    HRESULT hResult = S_OK;
    D3D11_MAPPED_SUBRESOURCE mappedData;
    hResult = m_pDeviceContext->Map(m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
    memcpy_s(mappedData.pData, sizeof(m_CBuffer), &m_CBuffer, sizeof(m_CBuffer));
    m_pDeviceContext->Unmap(m_pConstantBuffer.Get(), 0);
}

void MiscellaneousQNativeWindow::DrawViewContent3D()
{
    assert(m_pDeviceContext);
    static float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), black);
    m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    this->DrawViewContent3D_Coordinate();
    this->DrawViewContent3D_Triangle();
    this->DrawViewContent3D_TriangleTexture();
    this->DrawViewContent3D_Cube();
    this->DrawViewContent3D_CubeTexture();
}

void MiscellaneousQNativeWindow::DrawViewContent3D_Coordinate()
{
    // ------------------ Update Coordinates model matrix --------------------
    this->UpdateViewContent3D(DirectX::XMMatrixIdentity());
    // ------------------ Draw Coordinates --------------------
    UINT stride = sizeof(VertexPosColor);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0, 1, m_pVertexBufferCoor.GetAddressOf(), &stride, &offset);
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    m_pDeviceContext->IASetInputLayout(m_pVertexLayout.Get());
    m_pDeviceContext->VSSetShader(m_pVertexShaderCoor.Get(), nullptr, 0);
    m_pDeviceContext->PSSetShader(m_pPixelShaderCoor.Get(), nullptr, 0);
    m_pDeviceContext->Draw(6, 0);
}

void MiscellaneousQNativeWindow::DrawViewContent3D_Triangle()
{
    // ------------------ Update Triangle model matrix --------------------
    this->UpdateViewContent3D(DirectX::XMMatrixIdentity());
    // ------------------ Draw Triangle --------------------
    UINT stride = sizeof(VertexPosColor);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0, 1, m_pVertexBufferTriangle.GetAddressOf(), &stride, &offset);
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pDeviceContext->IASetInputLayout(m_pVertexLayout.Get());
    m_pDeviceContext->VSSetShader(m_pVertexShaderTriangle.Get(), nullptr, 0);
    m_pDeviceContext->PSSetShader(m_pPixelShaderTriangle.Get(), nullptr, 0);
    m_pDeviceContext->Draw(3, 0);
    m_pDeviceContext->Draw(3, 3);
}

void MiscellaneousQNativeWindow::DrawViewContent3D_TriangleTexture()
{}

void MiscellaneousQNativeWindow::DrawViewContent3D_Cube()
{
    // ------------------ Update Cube model matrix --------------------
    static float phi = 0.0f, theta = 0.0f;
    phi += 0.001f, theta += 0.0015f;
    DirectX::XMMATRIX translateBefore = DirectX::XMMatrixTranslation(-0.5f, -0.5f, -0.5f);
    DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationX(phi) * DirectX::XMMatrixRotationY(theta);
    DirectX::XMMATRIX translateAfter = DirectX::XMMatrixTranslation(0.5f, 0.5f, 0.5f);
    this->UpdateViewContent3D(translateBefore * rotation * translateAfter);
    // ------------------ Draw Cube --------------------
    UINT stride = sizeof(VertexPosColor);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0, 1, m_pVertexBufferCube.GetAddressOf(), &stride, &offset);
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pDeviceContext->IASetInputLayout(m_pVertexLayout.Get());
    m_pDeviceContext->VSSetShader(m_pVertexShaderCube.Get(), nullptr, 0);
    m_pDeviceContext->PSSetShader(m_pPixelShaderCube.Get(), nullptr, 0);
    m_pDeviceContext->IASetIndexBuffer(m_pIndexBufferCube.Get(), DXGI_FORMAT_R32_UINT, 0);
    m_pDeviceContext->DrawIndexed(36, 0, 0);
}

void MiscellaneousQNativeWindow::DrawViewContent3D_CubeTexture()
{
    // ------------------ Update Cube model matrix --------------------
    static float phi = 0.0f, theta = 0.0f;
    phi += 0.001f, theta += 0.0015f;
    DirectX::XMMATRIX translateBefore = DirectX::XMMatrixTranslation(-0.5f, 0.5f, 0.5f);
    DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationX(phi) * DirectX::XMMatrixRotationY(theta);
    DirectX::XMMATRIX translateAfter = DirectX::XMMatrixTranslation(0.5f, -0.5f, -0.5f);
    this->UpdateViewContent3D(translateBefore * rotation * translateAfter);
    // ------------------ Draw Cube --------------------
    UINT stride = sizeof(VertexPosColor);
    UINT offset = 0;
    m_pDeviceContext->IASetVertexBuffers(0, 1, m_pVertexBufferCubeTexture.GetAddressOf(), &stride, &offset);
    m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_pDeviceContext->IASetInputLayout(m_pVertexLayout.Get());
    m_pDeviceContext->VSSetShader(m_pVertexShaderCube.Get(), nullptr, 0);
    m_pDeviceContext->PSSetShader(m_pPixelShaderCube.Get(), nullptr, 0);
    m_pDeviceContext->IASetIndexBuffer(m_pIndexBufferCube.Get(), DXGI_FORMAT_R32_UINT, 0);
    m_pDeviceContext->DrawIndexed(36, 0, 0);
}

void MiscellaneousQNativeWindow::PresentViewContent3D()
{
    assert(m_pSwapChain);
    m_pSwapChain->Present(0, 0);
}

HRESULT MiscellaneousQNativeWindow::CreateShaderFromFile(const WCHAR* csoFileNameInOut, const WCHAR* hlslFileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut)
{
    HRESULT hResult = S_OK;

    if (csoFileNameInOut && D3DReadFileToBlob(csoFileNameInOut, ppBlobOut) == S_OK)
    {
        return hResult;
    }

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    dwShaderFlags |= D3DCOMPILE_DEBUG;
    dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION; // 禁用优化以避免出现一些不合理的情况
#endif
    ID3DBlob* errorBlob = nullptr;
    hResult = D3DCompileFromFile(hlslFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel, dwShaderFlags, 0, ppBlobOut, &errorBlob);
    if (FAILED(hResult))
    {
        if (errorBlob != nullptr)
        {
            OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
        }
        if (errorBlob)
        {
            errorBlob->Release();
            errorBlob = nullptr;
        }
        return hResult;
    }

    if (csoFileNameInOut)
    {
        return D3DWriteBlobToFile(*ppBlobOut, csoFileNameInOut, FALSE);
    }

    return hResult;
}

void MiscellaneousQNativeWindow::SlotUpdateViewContent3D_TimeOut()
{
    if (m_bInitialized3D)
    {
        this->DrawViewContent3D();
        this->PresentViewContent3D();
    }
}

void MiscellaneousQNativeWindow::on_btnEmptyTest1_clicked()
{
    // 更新世界坐标系
    static float phi = 0.0f;
    phi += 0.05f;
    m_matrixWorld = DirectX::XMMatrixRotationY(phi);
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
