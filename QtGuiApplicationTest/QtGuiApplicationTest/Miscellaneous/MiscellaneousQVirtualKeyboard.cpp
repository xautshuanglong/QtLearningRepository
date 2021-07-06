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
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <QRandomGenerator>

#include <LogUtil.h>

MiscellaneousQVirtualKeyboard::MiscellaneousQVirtualKeyboard(QWidget *parent)
    : MiscellaneousBase(parent)
    , mbNumbers(false)
    , mbLetters(false)
    , mbSymbols(false)
{
    ui.setupUi(this);

    this->connect(&mProcessOSK, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(SlotErrorOccurred(QProcess::ProcessError)));
    this->connect(&mProcessOSK, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(SlotFinished(int, QProcess::ExitStatus)));
    this->connect(&mProcessOSK, SIGNAL(readyReadStandardError()), this, SLOT(SlotReadyReadStandardError()));
    this->connect(&mProcessOSK, SIGNAL(readyReadStandardOutput()), this, SLOT(SlotReadyReadStandardOutput()));
    this->connect(&mProcessOSK, SIGNAL(started()), this, SLOT(SlotStarted()));
    this->connect(&mProcessOSK, SIGNAL(stateChanged(QProcess::ProcessState)), this, SLOT(SlotStateChanged(QProcess::ProcessState)));

    QTimer* pKeyboardTimer = new QTimer(this);
    pKeyboardTimer->start(1000);
    this->connect(pKeyboardTimer, SIGNAL(timeout()), this, SLOT(SlotTimerTimeout()));
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

void MiscellaneousQVirtualKeyboard::keyPressEvent(QKeyEvent* event)
{
    //int key = event->key();
    //if (Qt::Key_0 <= key && key <= Qt::Key_9)
    //{
    //    ui.teRichText->append(QString('0' + key - Qt::Key_0));
    //}
    //else if (Qt::Key_A <= key && key <= Qt::Key_Z)
    //{
    //    if (event->modifiers() & Qt::ShiftModifier )
    //    {
    //        ui.teRichText->append(QString('A' + key - Qt::Key_A));
    //    }
    //    else
    //    {
    //        ui.teRichText->append(QString('a' + key - Qt::Key_A));
    //    }
    //}
    MiscellaneousBase::keyPressEvent(event);
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

void MiscellaneousQVirtualKeyboard::SlotTimerTimeout()
{
    char msgBuffer[1024] = { 0 };
    QRandomGenerator random(time(NULL));
    if (mbNumbers)
    {
        int numKeyOffset = random.bounded(0, 10);
        //QCoreApplication::postEvent(this, new QKeyEvent(QEvent::KeyPress, Qt::Key_0 + numKeyOffset, Qt::NoModifier));
        //QKeyEvent numberKey(QEvent::KeyPress, Qt::Key_0 + numKeyOffset, Qt::NoModifier);
        //QCoreApplication::sendEvent(ui.teRichText, &numberKey);

        //HWND activeWin = ::GetActiveWindow();
        HWND activeWin = ::GetForegroundWindow();
        if (activeWin != NULL)
        {
            ::PostMessageA(activeWin, WM_KEYDOWN, 0x30 + numKeyOffset, 0);
            sprintf_s(msgBuffer, sizeof(msgBuffer), "Active window is 0x%08p\n", activeWin);
            OutputDebugStringA(msgBuffer);
        } 
        else
        {
            OutputDebugStringA("Active window is NULL\r\n");
        }
    }
    if (mbLetters)
    {
        int letterKeyOffset = random.bounded(0, 26);
        //int shiftFlag = random.bounded(0, 100);
        //if (shiftFlag < 50)
        //{
        //    QKeyEvent letterKeyEvent(QEvent::KeyPress, Qt::Key_A + letterKeyOffset, Qt::NoModifier);
        //    QCoreApplication::sendEvent(this, &letterKeyEvent);
        //    //qApp->postEvent(this, new QKeyEvent(QEvent::KeyPress, Qt::Key_0 + letterKeyOffset, Qt::NoModifier));
        //}
        //else
        //{
        //    QKeyEvent letterKeyEvent(QEvent::KeyPress, Qt::Key_A + letterKeyOffset, Qt::ShiftModifier);
        //    QCoreApplication::sendEvent(this, &letterKeyEvent);
        //}
        //HWND activeWin = ::GetActiveWindow();
        HWND activeWin = ::GetForegroundWindow();
        if (activeWin != NULL)
        {
            ::PostMessageA(activeWin, WM_KEYDOWN, VK_SHIFT, 0);
            ::PostMessageA(activeWin, WM_KEYDOWN, 0x41 + letterKeyOffset, 0);
            //::PostMessageA(HWND_BROADCAST, WM_KEYUP, 0x41 + letterKeyOffset, 0);
            //::PostMessageA(HWND_BROADCAST, WM_KEYUP, VK_SHIFT, 0);
            sprintf_s(msgBuffer, sizeof(msgBuffer), "Active window is 0x%08p\n", activeWin);
            OutputDebugStringA(msgBuffer);
        } 
        else
        {
            OutputDebugStringA("Active window is NULL\r\n");
        }
    }
    if (mbSymbols)
    {
        //int letterKeyOffset = random.bounded(0, 26);
        ////qApp->postEvent(this, new QKeyEvent(QEvent::KeyPress, Qt::Key_0 + letterKeyOffset, Qt::ShiftModifier));
        //QKeyEvent letterKeyEvent(QEvent::KeyPress, Qt::Key_A + letterKeyOffset, Qt::ShiftModifier);
        //QCoreApplication::sendEvent(this, &letterKeyEvent);
    }
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

void MiscellaneousQVirtualKeyboard::on_btnNumbers_clicked()
{
    mbNumbers = !mbNumbers;
}

void MiscellaneousQVirtualKeyboard::on_btnLetters_clicked()
{
    mbLetters = !mbLetters;
}

void MiscellaneousQVirtualKeyboard::on_btnSymbols_clicked()
{
    mbSymbols = !mbSymbols;
}

void MiscellaneousQVirtualKeyboard::on_btnNumLock_clicked()
{
    static BOOL bState = FALSE;
    BYTE keyState[256];

    BOOL resFlag = ::GetKeyboardState((LPBYTE)&keyState);
    if ((bState && !(keyState[VK_NUMLOCK] & 1)) || (!bState && (keyState[VK_NUMLOCK] & 1)))
    {
        bState = !bState;
        // Simulate a key press
        keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | 0, 0);
        // Simulate a key release
        keybd_event(VK_NUMLOCK, 0x45, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
    }

    this->ShowDesktop();
}

void MiscellaneousQVirtualKeyboard::on_btnSendInput_clicked()
{
    QTimer::singleShot(2000, [&]()
        {
            this->SimulateInput("Shuanglong", true);
            this->SimulateInput("shuanglong", false);
        });
}

void MiscellaneousQVirtualKeyboard::ShowDesktop()
{
    OutputDebugStringW(L"Sending 'Win-D'\r\n");
    INPUT inputs[4] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_LWIN;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = 0x44; // VK_D

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = 0x44; // VK_D
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_LWIN;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        //OutputStringW(L"SendInput failed: 0x%x\n", HRESULT_FROM_WIN32(GetLastError()));
        OutputDebugStringW(L"SendInput failed!\n");
    }
}

void MiscellaneousQVirtualKeyboard::SimulateInput(const QString& inputString, bool englishFlag)
{
    OutputDebugStringW(L"Sending ---> SimulateInput\r\n");
    INPUT inputs[4] = {};

    int strLen = inputString.length();
    for (int i = 0; i < strLen; ++i)
    {
        ZeroMemory(inputs, sizeof(inputs));
        const QChar tempChar = inputString.at(i);
        if ('a' <= tempChar && tempChar <= 'z')
        {
            WORD charOffset = tempChar.toLatin1() - 'a';
            WORD virtualKey = 0x41 + charOffset;
            inputs[0].type = INPUT_KEYBOARD;
            inputs[0].ki.wVk = virtualKey;

            inputs[1].type = INPUT_KEYBOARD;
            inputs[1].ki.wVk = virtualKey;
            inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

            UINT uSent = SendInput(2, inputs, sizeof(INPUT));
            if (uSent != 2)
            {
                OutputDebugStringW(L"SendInput failed!\n");
            }
        }
        else if ('A' <= tempChar && tempChar <= 'Z')
        {
            WORD charOffset = tempChar.toLatin1() - 'A';
            WORD virtualKey = 0x41 + charOffset;
            inputs[0].type = INPUT_KEYBOARD;
            inputs[0].ki.wVk = VK_SHIFT;

            inputs[1].type = INPUT_KEYBOARD;
            inputs[1].ki.wVk = virtualKey;

            inputs[2].type = INPUT_KEYBOARD;
            inputs[2].ki.wVk = virtualKey;
            inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

            inputs[3].type = INPUT_KEYBOARD;
            inputs[3].ki.wVk = VK_SHIFT;
            inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

            UINT uSent = SendInput(4, inputs, sizeof(INPUT));
            if (uSent != 4)
            {
                OutputDebugStringW(L"SendInput failed!\n");
            }
        }
        Sleep(200);
    }
    if (englishFlag)
    {
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = VK_RETURN;
        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = VK_RETURN;
        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
        UINT uSent = SendInput(2, inputs, sizeof(INPUT));
        if (uSent != 2)
        {
            OutputDebugStringW(L"SendInput failed!\n");
        }
    } 
    else
    {
        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = VK_SPACE;
        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = VK_SPACE;
        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
        UINT uSent = SendInput(2, inputs, sizeof(INPUT));
        if (uSent != 2)
        {
            OutputDebugStringW(L"SendInput failed!\n");
        }
    }
}
