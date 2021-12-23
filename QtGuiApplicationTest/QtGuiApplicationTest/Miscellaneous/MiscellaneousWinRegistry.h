#ifndef MISCELLANEOUS_WIN_REGISTRY_H
#define MISCELLANEOUS_WIN_REGISTRY_H

#include "MiscellaneousBase.h"

namespace Ui { class MiscellaneousWinRegistry; };

class MiscellaneousWinRegistry : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousWinRegistry(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousWinRegistry();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnRegNotifyChange_clicked();
    void on_btnEventExport_clicked();
    void on_btnEventWrite_clicked();
    void on_btnEmptyTest_clicked();

private:
    Ui::MiscellaneousWinRegistry *ui;
};

#endif // MISCELLANEOUS_WIN_REGISTRY_H
