#ifndef MISCELLANEOUS_WIN_SOCKET_H
#define MISCELLANEOUS_WIN_SOCKET_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousWinSocket.h"

namespace std
{
    class thread;
}

enum class EnumServerSocketMode
{
    CStyleSelect,
    Win32Select,
    Win32Asynchronize,
    CompletionPort,
};

class MiscellaneousWinSocket : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousWinSocket(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousWinSocket();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    void CStyleSelectStart();
    void CStyleSelectStop();
    void Win32SelectStart();
    void Win32SelectStop();
    void Win32AsynchronizeStart();
    void Win32AsynchronizeStop();
    void CompletionPortStart();
    void CompletionPortStop();
    void ThreadFunction();

private slots:
    void on_cbServerMode_currentIndexChanged(int index);
    void on_btnListen_clicked();
    void on_btnStop_clicked();

private:
    Ui::MiscellaneousWinSocket   ui;
    EnumServerSocketMode         mServerMode;
    std::thread                 *mThreadListen;
    std::list<std::thread*>      mListThreads;
};

#endif // MISCELLANEOUS_WIN_SOCKET_H
