#ifndef MISCELLANEOUS_STRING_UTIL_H
#define MISCELLANEOUS_STRING_UTIL_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousStringUtil.h"

class QListWidget;
class QLineEdit;

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

private:
    void InitCharacterTypeComboBox();
    void GetCharacterTypeInfo(uint typeID, QString& shortText, QString& showText);

private slots:
    void stateChanged(int state);
    void on_btnCharacterShiftLeft_clicked();
    void on_btnCharacterShiftRight_clicked();
    void on_btnBase64ShiftEncode_clicked();
    void on_btnBase64ShiftDecode_clicked();
    void on_btnRandomPassword_clicked();

private:
    Ui::MiscellaneousStringUtil ui;
    bool                        m_bSelected;
    uint                        m_characterTypes;
    QString                     m_strSelectedText;
    QListWidget                *m_pListWidget;
    QLineEdit                  *m_pLineEdit;
};

#endif // MISCELLANEOUS_STRING_UTIL_H
