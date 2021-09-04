#ifndef MISCELLANEOUS_QNATIVE_WINDOW_H
#define MISCELLANEOUS_QNATIVE_WINDOW_H

#include <wrl/client.h>
#include <d3d11_1.h>

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousQNativeWindow;};

class MiscellaneousQNativeWindow : public MiscellaneousBase
{
    template <class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    Q_OBJECT

public:
    MiscellaneousQNativeWindow(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousQNativeWindow();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    void InitializeDirect3D();

private slots:
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

    UINT                             m_4xMsaaQuality;
    bool                             m_4xMsaaEnabled;
};

#endif // MISCELLANEOUS_QNATIVE_WINDOW_H
