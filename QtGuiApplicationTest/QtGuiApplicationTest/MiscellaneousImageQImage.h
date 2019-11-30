#ifndef MISCELLANEOUS_IMAGE_QIMAGE_H
#define MISCELLANEOUS_IMAGE_QIMAGE_H

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousImageQImage;};

class MiscellaneousImageQImage : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousImageQImage(QWidget *parent = 0);
    ~MiscellaneousImageQImage();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnImgNoMemory_clicked();
    void on_btnImgNoMemoryCopy_clicked();
    void on_btnImgNoMemoryAssign_clicked();
    void on_btnImgNewMemory_clicked();
    void on_btnImgNewMemoryCopy_clicked();
    void on_btnImgNewMemoryAssign_clicked();

private:
    Ui::MiscellaneousImageQImage *ui;
};

#endif // MISCELLANEOUS_IMAGE_QIMAGE_H
