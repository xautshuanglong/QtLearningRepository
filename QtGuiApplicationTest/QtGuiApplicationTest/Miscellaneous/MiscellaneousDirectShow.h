#ifndef MISCELLANEOUS_DIRECT_SHOW_H
#define MISCELLANEOUS_DIRECT_SHOW_H

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousDirectShow;};

class MiscellaneousDirectShow : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousDirectShow(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousDirectShow();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnEnumDeviceTest_clicked();
    void on_btnCaptureDeviceTest_clicked();
    void on_btnEmptyTest_1_clicked();
    void on_btnEmptyTest_2_clicked();

private:
    Ui::MiscellaneousDirectShow *ui;
};

#endif // MISCELLANEOUS_DIRECT_SHOW_H
