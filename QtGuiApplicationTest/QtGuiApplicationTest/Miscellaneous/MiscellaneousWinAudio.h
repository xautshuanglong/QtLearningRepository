#ifndef MISCELLANEOUS_WIN_AUDIO_H
#define MISCELLANEOUS_WIN_AUDIO_H

#include "MiscellaneousBase.h"

#include <string>
#include <thread>

namespace Ui { class MiscellaneousWinAudio; };

class MiscellaneousWinAudio : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousWinAudio(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousWinAudio();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnMonitorRegStart_clicked();
    void on_btnMonitorRegStop_clicked();
    void on_btnEventWrite_clicked();
    void on_btnEmptyTest_clicked();

private:

private:
    Ui::MiscellaneousWinAudio *ui;
    bool mIsMonitorRegRunning;
    std::thread* mpRegMonitorThread;
};

#endif // MISCELLANEOUS_WIN_AUDIO_H
