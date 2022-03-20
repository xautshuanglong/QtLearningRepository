#ifndef MISCELLANEOUS_IMAGE_GRABBER_H
#define MISCELLANEOUS_IMAGE_GRABBER_H

#include "MiscellaneousBase.h"

#include <Windows.h>
#include <d3d11_1.h>

namespace Ui {class MiscellaneousImageGrabber;};

class ScreenShotEditer;

class MiscellaneousImageGrabber : public MiscellaneousBase
{
    Q_OBJECT

 public:
    MiscellaneousImageGrabber(QWidget *parent = 0);
    ~MiscellaneousImageGrabber();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

protected:
    virtual void mouseMoveEvent(QMouseEvent* mouseEvent) override;

private slots:
    void on_btnGrabWindow_clicked();
    void on_btnGrabDesktop_clicked();
    void on_btnGrabScreen_clicked();
    void on_btnGrabWidget_clicked();
    void on_btnGrabScreenAll_clicked();
    void on_btnGrabScrollArea_clicked();
    void on_btnGrabDxgi_clicked();
    void on_btnDxgiEnum_clicked();
    void on_btnEnumWindow_clicked();

private:
    void Init3D();
    void Uninit3D();
    void UpdateLogInfo();
    void GetWindowClassName(HWND hWnd, std::string& wndClassName);
    void GetWindowTitleString(HWND hWnd, std::string& wndTitle);
    void EnumTopLevelWindow();
    void EnumZOrderWindow();
    void EnumChildWindowRecursively(HWND parentWnd);
    BOOL EnumWindowProcShadow(HWND hwnd);
    BOOL EnumChildWindowProcShadow(HWND childWnd);
    BOOL EnumDesktopProcShadow(LPTSTR lpszDesktop);
    BOOL EnumWindowStationProcShadow(LPTSTR lpszWindowStation);
    static BOOL CALLBACK EnumWindowsProc(_In_ HWND   hwnd, _In_ LPARAM lParam);
    static BOOL CALLBACK EnumChildWindowProc(_In_ HWND hwnd, _In_ LPARAM lParam);
    static BOOL CALLBACK EnumDesktopProc(_In_ LPTSTR lpszDesktop, _In_ LPARAM lParam);
    static BOOL CALLBACK EnumWindowStationProc(_In_ LPTSTR lpszWindowStation, _In_ LPARAM lParam);

private:
    Ui::MiscellaneousImageGrabber *ui;
    std::list<RECT>                mWinRectList;
    HWND                           mCurMouseWin;
    QPoint                         mMousePoint;
    ScreenShotEditer              *mpScreenShotEditer;
    ID3D11Device                  *mpDevice;
    ID3D11DeviceContext           *mpDeviceCtx;
    IDXGIOutputDuplication        *mpDxgiOutputDuplication;
};


class ScreenShotEditer : public QWidget
{
    Q_OBJECT

public:
    ScreenShotEditer(QWidget* parent = nullptr);
    ~ScreenShotEditer();

    void SetScreenShotImage(const QPixmap& pixmap);
    void SetWindowRectList(const std::list<RECT>& winRectList);

protected:
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void keyReleaseEvent(QKeyEvent* event) override;

private:
    QRect                          mDesktopRect;
    QRect                          mCurWinRect;
    QPoint                         mCurMousePos;
    QPixmap                        mScreenShot;
    std::list<RECT>                mWinRectList;
};

#endif // MISCELLANEOUS_IMAGE_GRABBER_H
