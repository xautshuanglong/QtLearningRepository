#ifndef MISCELLANEOUS_QMIMETYPE_H
#define MISCELLANEOUS_QMIMETYPE_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousQMimeType.h"

class MiscellaneousQMimeType : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousQMimeType(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousQMimeType();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnFileOpen_clicked();

private:
    Ui::MiscellaneousQMimeType ui;
};

#endif // MISCELLANEOUS_QMIMETYPE_H
