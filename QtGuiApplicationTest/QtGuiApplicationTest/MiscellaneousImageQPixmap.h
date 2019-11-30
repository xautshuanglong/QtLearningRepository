#ifndef MISCELLANEOUS_IMAGE_QPIXMAP_H
#define MISCELLANEOUS_IMAGE_QPIXMAP_H

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousImageQPixmap;};

class MiscellaneousImageQPixmap : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousImageQPixmap(QWidget *parent = 0);
    ~MiscellaneousImageQPixmap();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    Ui::MiscellaneousImageQPixmap *ui;
};

#endif // MISCELLANEOUS_IMAGE_QPIXMAP_H
