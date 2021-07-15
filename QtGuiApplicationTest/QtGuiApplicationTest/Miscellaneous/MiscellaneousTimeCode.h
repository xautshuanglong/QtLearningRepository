#ifndef MISCELLANEOUS_TIME_CODE_H
#define MISCELLANEOUS_TIME_CODE_H

#include <Windows.h>

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousTimeCode;};

class QTimer;

class MiscellaneousTimeCode : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousTimeCode(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousTimeCode();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    std::string MidiTechnologyToString(WORD wTechnology);
    std::string MidiSupportToString(DWORD dwSupport);
    std::string MidiMsgTypeToString(MMRESULT errorCode);
    std::string MidiErrorCodeToString(MMRESULT errorCode);
    static void CALLBACK MidiInProcedure(HMIDIIN hMidiOut, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
    static void CALLBACK MidiOutProcedure(HMIDIOUT hMidiOut, UINT wMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

private slots:
    void TimeCodeEmiter_TimeOut();

    void on_btnTransferTest_clicked();
    void on_btnEventMapTest_clicked();
    void on_btnTimeEmiterTest_clicked();
    void on_btnEnumerateMIDI_clicked();

private:
    Ui::MiscellaneousTimeCode *ui;
    QTimer                    *mpTimeCodeEmiter;
    bool                       mbTimeCodeEnable;
    QString                    mBtnTimeEmiterText;
    long long                  mCurFrequency;
};

#endif // MISCELLANEOUS_TIME_CODE_H
