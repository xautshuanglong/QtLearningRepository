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
    void InitializeDirectResource();
    void ResizeBufferAndTargetView();
    void UpdateViewContent3D();
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
    D3D11_VIEWPORT                   m_ScreenViewport;

    ComPtr<ID3D11InputLayout>        m_pVertexLayout;
    ComPtr<ID3D11Buffer>             m_pVertexBuffer;
    ComPtr<ID3D11VertexShader>       m_pVertexShader;
    ComPtr<ID3D11PixelShader>        m_pPixelShader;

    ComPtr<ID3D11InputLayout>        m_pVertexLayoutCube;
    ComPtr<ID3D11Buffer>             m_pVertexBufferCube;
    ComPtr<ID3D11Buffer>             m_pIndexBuffer;
    ComPtr<ID3D11Buffer>             m_pConstantBuffer;

    ComPtr<ID3D11VertexShader>       m_pVertexShaderCube;
    ComPtr<ID3D11PixelShader>        m_pPixelShaderCube;
    ConstantBuffer                   m_CBuffer;

    UINT                             m_4xMsaaQuality;
    bool                             m_4xMsaaEnabled;
    bool                             m_bInitialized3D;
};

#endif // MISCELLANEOUS_QNATIVE_WINDOW_H
