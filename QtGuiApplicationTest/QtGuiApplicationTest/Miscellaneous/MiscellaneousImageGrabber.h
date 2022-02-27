#ifndef MISCELLANEOUS_IMAGE_GRABBER_H
#define MISCELLANEOUS_IMAGE_GRABBER_H

#include "MiscellaneousBase.h"

#include <Windows.h>

namespace Ui {class MiscellaneousImageGrabber;};

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

private:
    void UpdateLogInfo();
    void GetWindowClassName(HWND hWnd, std::string& wndClassName);
    void EnumChildWindowRecursively(HWND parentWnd);
    BOOL EnumChildWindowProcShadow(HWND childWnd);
    BOOL EnumDesktopProcShadow(LPTSTR lpszDesktop);
    BOOL EnumWindowStationProcShadow(LPTSTR lpszWindowStation);
    static BOOL CALLBACK EnumChildWindowProc(_In_ HWND hwnd, _In_ LPARAM lParam);
    static BOOL CALLBACK EnumDesktopProc(_In_ LPTSTR lpszDesktop, _In_ LPARAM lParam);
    static BOOL CALLBACK EnumWindowStationProc(_In_ LPTSTR lpszWindowStation, _In_ LPARAM lParam);

private:
    Ui::MiscellaneousImageGrabber *ui;
    std::list<RECT>                mWinRectList;
    HWND                           mCurMouseWin;
    QPoint                         mMousePoint;
};

#endif // MISCELLANEOUS_IMAGE_GRABBER_H
