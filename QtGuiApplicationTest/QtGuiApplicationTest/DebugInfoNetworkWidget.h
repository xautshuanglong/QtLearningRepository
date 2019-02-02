#pragma once

#include "DebugInfoBaseWidget.h"

// For IcmpSendEcho2 and Icmp6SendEcho2
//#define PIO_APC_ROUTINE_DEFINED
//typedef struct _IO_STATUS_BLOCK
//{
//    union
//    {
//        NTSTATUS Status;
//        PVOID Pointer;
//    };
//    ULONG_PTR Information;
//} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;
//typedef void (*PIO_APC_ROUTINE) (void* ApcContext, PIO_STATUS_BLOCK IoStatusBlock, unsigned long Reserved);

namespace Ui
{
    class DebugInfoNetworkWidget;
};

class DebugInfoNetworkWidget : public DebugInfoBaseWidget
{
    Q_OBJECT

public:
    DebugInfoNetworkWidget(QWidget *parent = Q_NULLPTR);
    ~DebugInfoNetworkWidget();

private:
    void InitializeNetworkEnvironment();
    void UninitializeNetworkEnvironment();
    void PingTest(const QString &destinationAddress);
    void WinAPIGetAddrInfoTest(const QString &destinationAddress);
    void WinAPIGetHostByAddrTest(const QString &destinationAddress);
    void WinAPIGetNameInfoTest(const QString &destinationAddress, const int &destinationPort);
    void WinAPIIcmpSendEchoTest(const QString &destinationAddress);
    void WinAPIIcmpSendEcho2Test(const QString &destinationAddress);
    void WinAPIIcmp6SendEcho2Test(const QString &destinationAddress);
    ushort CaculateChecksum(uchar *InBuffer, uint BufferLen);
    void ParseHostInfo(struct hostent *pHostInfo);
    //static void IcmpSendEchoApcRoutine(void *pApcContext, PIO_STATUS_BLOCK ioStatusBlock, unsigned long reserved);

protected:
    virtual bool OnDebugMenuEvent(DebugMenuEvent *event) override;
    virtual void OnUpdateDebugInfo() override;
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_btnPingTest_clicked();

private:
    Ui::DebugInfoNetworkWidget *ui;
};
