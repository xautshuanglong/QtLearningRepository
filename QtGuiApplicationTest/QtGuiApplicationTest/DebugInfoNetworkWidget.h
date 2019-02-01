#pragma once

#include "DebugInfoBaseWidget.h"

// Windows Headers
#include <WinSock2.h>

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
    UINT16 CaculateChecksum(UINT8 *InBuffer, INT32 BufferLen);
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
