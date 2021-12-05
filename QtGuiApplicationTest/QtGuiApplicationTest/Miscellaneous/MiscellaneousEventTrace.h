#ifndef MISCELLANEOUS_EVENT_TRACE_H
#define MISCELLANEOUS_EVENT_TRACE_H

#include "MiscellaneousBase.h"

namespace Ui { class MiscellaneousEventTrace; };

class MiscellaneousEventTrace : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousEventTrace(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousEventTrace();

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
    Ui::MiscellaneousEventTrace *ui;
};

#endif // MISCELLANEOUS_EVENT_TRACE_H
