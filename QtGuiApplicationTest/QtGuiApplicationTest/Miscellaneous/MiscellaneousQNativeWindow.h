#ifndef MISCELLANEOUS_QNATIVE_WINDOW_H
#define MISCELLANEOUS_QNATIVE_WINDOW_H

#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousQNativeWindow;};

class MiscellaneousQNativeWindow : public MiscellaneousBase
{
    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    struct VertexPosColor
    {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT4 color;
        static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
    };

    struct ConstantBuffer
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };

    Q_OBJECT

public:
    MiscellaneousQNativeWindow(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousQNativeWindow();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

protected:
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    void InitializeDirect3D();
    void InitializeDirectShaders();
    void InitializeDirectTextures();
    void InitializeDirectSamplers();
    void InitializeDirectVertices();
    HRESULT InitializeDirectVertices_Coordinate();
    HRESULT InitializeDirectVertices_Triangle();
    HRESULT InitializeDirectVertices_TriangleTexture();
    HRESULT InitializeDirectVertices_Cube();
    HRESULT InitializeDirectVertices_CubeTexture();
    void ResizeBufferAndTargetView();
    void UpdateViewContent3D(const DirectX::XMMATRIX& modelMatrix);
    void DrawViewContent3D();
    void DrawViewContent3D_Coordinate();
    void DrawViewContent3D_Triangle();
    void DrawViewContent3D_TriangleTexture();
    void DrawViewContent3D_Cube();
    void DrawViewContent3D_CubeTexture();
    void PresentViewContent3D();
    HRESULT CreateShaderFromFile(const WCHAR* csoFileNameInOut, const WCHAR* hlslFileName, LPCSTR entryPoint, LPCSTR shaderModel, ID3DBlob** ppBlobOut);

private slots:
    void SlotUpdateViewContent3D_TimeOut();

    void on_btnEmptyTest1_clicked();
    void on_btnEmptyTest2_clicked();
    void on_btnEmptyTest3_clicked();
    void on_btnEmptyTest4_clicked();

private:
    Ui::MiscellaneousQNativeWindow   *ui;
    ComPtr<ID3D11Device>             m_pDevice;
    ComPtr<ID3D11DeviceContext>      m_pDeviceContext;
    ComPtr<IDXGISwapChain>           m_pSwapChain;

    ComPtr<ID3D11Device1>            m_pDevice1;
    ComPtr<ID3D11DeviceContext1>     m_pDeviceContext1;
    ComPtr<IDXGISwapChain1>          m_pSwapChain1;

    ComPtr<ID3D11Texture2D>          m_pDepthStencilBuffer;
    ComPtr<ID3D11RenderTargetView>   m_pRenderTargetView;
    ComPtr<ID3D11DepthStencilView>   m_pDepthStencilView;

    // 定点布局，通用
    ComPtr<ID3D11InputLayout>        m_pVertexLayout;

    // 三角形绘制
    ComPtr<ID3D11Buffer>             m_pVertexBufferTriangle;
    ComPtr<ID3D11VertexShader>       m_pVertexShaderTriangle;
    ComPtr<ID3D11PixelShader>        m_pPixelShaderTriangle;

    // 立方体绘制
    ComPtr<ID3D11Buffer>             m_pVertexBufferCube;
    ComPtr<ID3D11Buffer>             m_pIndexBufferCube;
    ComPtr<ID3D11Buffer>             m_pConstantBuffer;
    ComPtr<ID3D11VertexShader>       m_pVertexShaderCube;
    ComPtr<ID3D11PixelShader>        m_pPixelShaderCube;

    // 立方体绘制（带木箱纹理）
    ComPtr<ID3D11Buffer>             m_pVertexBufferCubeTexture;
    ComPtr<ID3D11ShaderResourceView> m_pResourceViewWood;
    ComPtr<ID3D11SamplerState>       m_pSamplerState;

    // 坐标系绘制
    ComPtr<ID3D11Buffer>             m_pVertexBufferCoor;
    ComPtr<ID3D11VertexShader>       m_pVertexShaderCoor;
    ComPtr<ID3D11PixelShader>        m_pPixelShaderCoor;

    ConstantBuffer                   m_CBuffer;
    DirectX::XMMATRIX                m_matrixWorld;
    D3D11_VIEWPORT                   m_ScreenViewport;

    UINT                             m_4xMsaaQuality;
    bool                             m_4xMsaaEnabled;
    bool                             m_bInitialized3D;
};

#endif // MISCELLANEOUS_QNATIVE_WINDOW_H
