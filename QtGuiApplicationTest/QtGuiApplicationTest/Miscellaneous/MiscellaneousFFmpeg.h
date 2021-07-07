#ifndef MISCELLANEOUS_FFMPEG_H
#define MISCELLANEOUS_FFMPEG_H

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousFFmpeg;};

class MiscellaneousFFmpeg : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousFFmpeg(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousFFmpeg();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnAudioTest_clicked();
    void on_btnVideoTest_clicked();
    void on_btnPictureTest_clicked();
    void on_btnEmptyTest_clicked();

private:
    Ui::MiscellaneousFFmpeg *ui;
};

#endif // MISCELLANEOUS_FFMPEG_H
