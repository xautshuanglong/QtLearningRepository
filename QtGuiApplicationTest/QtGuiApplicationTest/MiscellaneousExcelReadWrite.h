#ifndef MISCELLANEOUS_EXCEL_READWRITE_H
#define MISCELLANEOUS_EXCEL_READWRITE_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousExcelReadWrite.h"

class MiscellaneousExcelReadWrite : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousExcelReadWrite(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousExcelReadWrite();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnOne_clicked();
    void on_btnTwo_clicked();
    void on_btnThree_clicked();
    void on_btnFour_clicked();

private:
    Ui::MiscellaneousExcelReadWrite ui;
};

#endif // MISCELLANEOUS_EXCEL_READWRITE_H
