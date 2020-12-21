#ifndef MISCELLANEOUS_CUSTOM_TEXTEDIT_H
#define MISCELLANEOUS_CUSTOM_TEXTEDIT_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousCustomTextEdit.h"

class MiscellaneousCustomTextEdit : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousCustomTextEdit(QWidget *parent = 0);
    ~MiscellaneousCustomTextEdit();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnAppendText_clicked();

private:
    Ui::MiscellaneousCustomTextEdit ui;
};

#endif // MISCELLANEOUS_CUSTOM_TEXTEDIT_H
