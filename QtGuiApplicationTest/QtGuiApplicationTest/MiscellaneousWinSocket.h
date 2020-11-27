#ifndef MISCELLANEOUS_WIN_SOCKET_H
#define MISCELLANEOUS_WIN_SOCKET_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousWinSocket.h"

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

private slots:
    void on_btnListen_clicked();
    void on_btnStop_clicked();

private:
    Ui::MiscellaneousWinSocket   ui;
};

#endif // MISCELLANEOUS_WIN_SOCKET_H
