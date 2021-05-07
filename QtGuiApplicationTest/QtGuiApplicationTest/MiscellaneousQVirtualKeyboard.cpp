#include "MiscellaneousQVirtualKeyboard.h"

#include <windows.h>

#include <QTextDocument>
#include <QTextBlock>
#include <QDateTime>

#include <LogUtil.h>

MiscellaneousQVirtualKeyboard::MiscellaneousQVirtualKeyboard(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousQVirtualKeyboard::~MiscellaneousQVirtualKeyboard()
{
}

QString MiscellaneousQVirtualKeyboard::GetTitle()
{
    return QObject::tr("Virtual Keyboard");
}

QString MiscellaneousQVirtualKeyboard::GetTitleTooltip()
{
    return QObject::tr("Custom virtual keyboard.");
}

MiscellaneousTestGroup MiscellaneousQVirtualKeyboard::GetGroupID()
{
    return MiscellaneousTestGroup::QT_Test;
}

MiscellaneousTestItem MiscellaneousQVirtualKeyboard::GetItemID()
{
    return MiscellaneousTestItem::QT_Virtual_Keyboard;
}

void MiscellaneousQVirtualKeyboard::on_btnOSK_clicked()
{
    int i = 0;
}

void MiscellaneousQVirtualKeyboard::on_btnTabtip_clicked()
{
    int i = 0;
    /*
#include <initguid.h>
#include <Objbase.h>
#pragma hdrstop

    // 4ce576fa-83dc-4F88-951c-9d0782b4e376
    DEFINE_GUID(CLSID_UIHostNoLaunch,
        0x4CE576FA, 0x83DC, 0x4f88, 0x95, 0x1C, 0x9D, 0x07, 0x82, 0xB4, 0xE3, 0x76);

    // 37c994e7_432b_4834_a2f7_dce1f13b834b
    DEFINE_GUID(IID_ITipInvocation,
        0x37c994e7, 0x432b, 0x4834, 0xa2, 0xf7, 0xdc, 0xe1, 0xf1, 0x3b, 0x83, 0x4b);

    struct ITipInvocation : IUnknown
    {
        virtual HRESULT STDMETHODCALLTYPE Toggle(HWND wnd) = 0;
    };

    int WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
    {
        HRESULT hr;
        hr = CoInitialize(0);

        ITipInvocation* tip;
        hr = CoCreateInstance(CLSID_UIHostNoLaunch, 0, CLSCTX_INPROC_HANDLER | CLSCTX_LOCAL_SERVER, IID_ITipInvocation, (void**)&tip);
        tip->Toggle(GetDesktopWindow());
        tip->Release();
        return 0;
    }
    */
}
