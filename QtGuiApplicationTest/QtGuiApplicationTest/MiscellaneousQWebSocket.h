#ifndef MISCELLANEOUSQ_QWEBSOCKET_H
#define MISCELLANEOUSQ_QWEBSOCKET_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousQWebSocket.h"

class MiscellaneousQWebSocket : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousQWebSocket(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousQWebSocket();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnConnect_clicked();
    void on_btnDisconnect_clicked();

private:
    Ui::MiscellaneousQWebSocket ui;
};

#endif // MISCELLANEOUSQ_QWEBSOCKET_H
