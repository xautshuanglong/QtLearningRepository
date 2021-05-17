#include "MiscellaneousQVirtualKeyboard.h"

#include <windows.h>

// 弹出系统虚拟键盘（平板模式）
#include <initguid.h>
#include <Objbase.h>
// #pragma hdrstop // 预编译头文件到此为止

// 4ce576fa-83dc-4F88-951c-9d0782b4e376
DEFINE_GUID(CLSID_UIHostNoLaunch, 0x4CE576FA, 0x83DC, 0x4f88, 0x95, 0x1C, 0x9D, 0x07, 0x82, 0xB4, 0xE3, 0x76);
// 37c994e7_432b_4834_a2f7_dce1f13b834b
DEFINE_GUID(IID_ITipInvocation, 0x37c994e7, 0x432b, 0x4834, 0xa2, 0xf7, 0xdc, 0xe1, 0xf1, 0x3b, 0x83, 0x4b);

#include <QTextDocument>
#include <QTextBlock>
#include <QDateTime>
#include <QMetaEnum>

#include <LogUtil.h>

MiscellaneousQVirtualKeyboard::MiscellaneousQVirtualKeyboard(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);

    this->connect(&mProcessOSK, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(SlotErrorOccurred(QProcess::ProcessError)));
    this->connect(&mProcessOSK, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(SlotFinished(int, QProcess::ExitStatus)));
    this->connect(&mProcessOSK, SIGNAL(readyReadStandardError()), this, SLOT(SlotReadyReadStandardError()));
    this->connect(&mProcessOSK, SIGNAL(readyReadStandardOutput()), this, SLOT(SlotReadyReadStandardOutput()));
    this->connect(&mProcessOSK, SIGNAL(started()), this, SLOT(SlotStarted()));
    this->connect(&mProcessOSK, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(SlotStateChanged(QProcess::ProcessState)));
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

void MiscellaneousQVirtualKeyboard::SlotErrorOccurred(QProcess::ProcessError error)
{
    QMetaEnum processErrorEnum = QMetaEnum::fromType<QProcess::ProcessError>();
    QString processErrorStr = processErrorEnum.valueToKeys(error);
    LogUtil::Error(CODE_LOCATION, "QProcess Error: %s", processErrorStr.toUtf8().data());
}

void MiscellaneousQVirtualKeyboard::SlotFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    QMetaEnum exitStatusEnum = QMetaEnum::fromType<QProcess::ExitStatus>();
    QString exitStatusStr = exitStatusEnum.valueToKeys(exitCode);
    LogUtil::Error(CODE_LOCATION, "QProcess Exit Status [%d]: %s", exitCode, exitStatusStr.toUtf8().data());
}

void MiscellaneousQVirtualKeyboard::SlotReadyReadStandardError()
{
    LogUtil::Error(CODE_LOCATION, "QProcess ready for reading standard error.");
}

void MiscellaneousQVirtualKeyboard::SlotReadyReadStandardOutput()
{
    LogUtil::Error(CODE_LOCATION, "QProcess ready for reading standard output.");
}

void MiscellaneousQVirtualKeyboard::SlotStarted()
{
    LogUtil::Error(CODE_LOCATION, "QProcess has started.");
}

void MiscellaneousQVirtualKeyboard::SlotStateChanged(QProcess::ProcessState newStatus)
{
    QMetaEnum processStateEnum = QMetaEnum::fromType<QProcess::ProcessState>();
    QString processStateStr = processStateEnum.valueToKeys(newStatus);
    LogUtil::Error(CODE_LOCATION, "QProcess new Status: %s", processStateStr.toUtf8().data());
}

void MiscellaneousQVirtualKeyboard::on_btnOSK_clicked()
{
    QString filename = "C:\\Windows\\System32\\osk.exe";
    if (QFile::exists(filename))
    {
        mProcessOSK.setProgram(filename);
        mProcessOSK.setWorkingDirectory("C:\\Windows\\System32");
        mProcessOSK.startDetached();
        //mProcessOSK.start();
    }
}

void MiscellaneousQVirtualKeyboard::on_btnTabtip_clicked()
{
    struct ITipInvocation : IUnknown
    {
        virtual HRESULT STDMETHODCALLTYPE Toggle(HWND wnd) = 0;
    };

    HRESULT hr;
    ITipInvocation* tip;
    hr = CoInitialize(0);
    hr = CoCreateInstance(CLSID_UIHostNoLaunch, 0, CLSCTX_INPROC_HANDLER | CLSCTX_LOCAL_SERVER, IID_ITipInvocation, (void**)&tip);
    if (hr == S_OK)
    {
        tip->Toggle(GetDesktopWindow());
        tip->Release();
    }
    CoUninitialize();
}
