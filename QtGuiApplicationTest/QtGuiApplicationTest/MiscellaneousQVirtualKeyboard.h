#ifndef MISCELLANEOUS_QVIRTUALKEYBOARD_H
#define MISCELLANEOUS_QVIRTUALKEYBOARD_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousQVirtualKeyboard.h"

#include <QProcess>

class QPaintEvent;

class MiscellaneousQVirtualKeyboard : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousQVirtualKeyboard(QWidget *parent = 0);
    ~MiscellaneousQVirtualKeyboard();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

protected:
    virtual void keyPressEvent(QKeyEvent* event) override;

private slots:
    void SlotErrorOccurred(QProcess::ProcessError error);
    void SlotFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void SlotReadyReadStandardError();
    void SlotReadyReadStandardOutput();
    void SlotStarted();
    void SlotStateChanged(QProcess::ProcessState newStatus);
    void SlotTimerTimeout();

    void on_btnOSK_clicked();
    void on_btnTabtip_clicked();
    void on_btnNumbers_clicked();
    void on_btnLetters_clicked();
    void on_btnSymbols_clicked();
    void on_btnNumLock_clicked();
    void on_btnSendInput_clicked();

private:
    void ShowDesktop();
    void SimulateInput(const QString &inputString, bool englishFlag);

private:
    Ui::MiscellaneousQVirtualKeyboard ui;
    QProcess                          mProcessOSK;
    bool                              mbNumbers;
    bool                              mbLetters;
    bool                              mbSymbols;
};

#endif // MISCELLANEOUS_QVIRTUALKEYBOARD_H
