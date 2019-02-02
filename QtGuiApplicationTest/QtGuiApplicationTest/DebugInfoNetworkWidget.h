#pragma once

#include "DebugInfoBaseWidget.h"

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
    ushort CaculateChecksum(uchar *InBuffer, uint BufferLen);
    void ParseHostInfo(struct hostent *pHostInfo);

protected:
    virtual bool OnDebugMenuEvent(DebugMenuEvent *event) override;
    virtual void OnUpdateDebugInfo() override;
    virtual void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_btnPingTest_clicked();

private:
    Ui::DebugInfoNetworkWidget *ui;
};
