#ifndef MISCELLANEOUS_WIN_EVENT_TRACE_H
#define MISCELLANEOUS_WIN_EVENT_TRACE_H

#include "MiscellaneousBase.h"

namespace Ui { class MiscellaneousWinEventTrace; };

class MiscellaneousWinEventTrace : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousWinEventTrace(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousWinEventTrace();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnEventSubscript_clicked();
    void on_btnEventExport_clicked();
    void on_btnEventWrite_clicked();
    void on_btnEmptyTest_clicked();

private:
    Ui::MiscellaneousWinEventTrace *ui;
};

#endif // MISCELLANEOUS_WIN_EVENT_TRACE_H
