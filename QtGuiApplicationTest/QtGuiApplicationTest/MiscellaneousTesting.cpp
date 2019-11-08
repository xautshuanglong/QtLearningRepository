#include "MiscellaneousTesting.h"

#include <windows.h>
#include <process.h>

#include "ui_MiscellaneousTesting.h"
#include "FramelessWindowHelper.h"

MiscellaneousTesting::MiscellaneousTesting(QWidget *parent /* = Q_NULLPTR */)
    : QMainWindow(parent)
    , mpFramelessHelper(Q_NULLPTR)
    , ui(new Ui::MiscellaneousTesting())
    , m_beginThreadHandle(0)
    , m_threadStopFlag(false)
{
    ui->setupUi(this);
    mpFramelessHelper = new FramelessWindowHelper(this);
}

MiscellaneousTesting::~MiscellaneousTesting()
{
    if (mpFramelessHelper != Q_NULLPTR)
    {
        delete mpFramelessHelper;
    }
}

void MiscellaneousTesting::closeEvent(QCloseEvent *event)
{
    emit SignalClosed();
}

void MiscellaneousTesting::on_btnWinApiThreadStart_clicked()
{
    m_threadStopFlag = false;
    m_beginThreadHandle = _beginthreadex(NULL, 0, MiscellaneousTesting::ThreadProc, (void*)this, 0, NULL);
}

void MiscellaneousTesting::on_btnWinApiThreadStop_clicked()
{
    m_threadStopFlag = true;
    ::WaitForSingleObject((HANDLE)m_beginThreadHandle, INFINITE);
    int i = 0;
}

void MiscellaneousTesting::run()
{
    int count = 0;
    while (count < 1000)
    {
        ::_sleep(10);
        ++count;
        if (m_threadStopFlag)
        {
            break;
        }
    }
    int i = 0;
}

unsigned int _stdcall MiscellaneousTesting::ThreadProc(void *pArg)
{
    MiscellaneousTesting *pMiscellaneous = (MiscellaneousTesting*)pArg;
    pMiscellaneous->run();
    return 0;
}
