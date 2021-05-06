#ifndef MISCELLANEOUS_QVIRTUALKEYBOARD_H
#define MISCELLANEOUS_QVIRTUALKEYBOARD_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousQVirtualKeyboard.h"

class QPaintEvent;

class MiscellaneousQVirtualKeyboard : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousQVirtualKeyboard(QWidget *parent = 0);
    ~MiscellaneousQVirtualKeyboard();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnOSK_clicked();
    void on_btnTabtip_clicked();

private:
    Ui::MiscellaneousQVirtualKeyboard ui;
};

#endif // MISCELLANEOUS_QVIRTUALKEYBOARD_H
