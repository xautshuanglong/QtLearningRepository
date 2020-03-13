#ifndef MISCELLANEOUS_STRING_UTIL_H
#define MISCELLANEOUS_STRING_UTIL_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousStringUtil.h"

class MiscellaneousStringUtil : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousStringUtil(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousStringUtil();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnCharacterShiftLeft_clicked();
    void on_btnCharacterShiftRight_clicked();
    void on_btnBase64ShiftEncode_clicked();
    void on_btnBase64ShiftDecode_clicked();
    void on_btnRandomPassword_clicked();

private:
    Ui::MiscellaneousStringUtil ui;
    QString                     m_strShifted;
    QString                     m_strEncoded;
};

#endif // MISCELLANEOUS_STRING_UTIL_H
