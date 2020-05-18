#ifndef MISCELLANEOUSQ_QTCPSOCKET_H
#define MISCELLANEOUSQ_QTCPSOCKET_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousQTcpSocket.h"

class MiscellaneousQTcpSocket : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousQTcpSocket(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousQTcpSocket();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    Ui::MiscellaneousQTcpSocket ui;
};

#endif // MISCELLANEOUSQ_QTCPSOCKET_H
