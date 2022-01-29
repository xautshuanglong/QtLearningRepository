#ifndef MISCELLANEOUS_IMAGE_GRABBER_H
#define MISCELLANEOUS_IMAGE_GRABBER_H

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousImageGrabber;};

class MiscellaneousImageGrabber : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousImageGrabber(QWidget *parent = 0);
    ~MiscellaneousImageGrabber();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnGrabWindow_clicked();
    void on_btnGrabDesktop_clicked();
    void on_btnGrabScreen_clicked();
    void on_btnGrabWidget_clicked();
    void on_btnGrabScreenAll_clicked();
    void on_btnGrabScrollArea_clicked();

private:
    Ui::MiscellaneousImageGrabber *ui;
};

#endif // MISCELLANEOUS_IMAGE_GRABBER_H
