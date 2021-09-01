#ifndef MISCELLANEOUS_QNATIVE_WINDOW_H
#define MISCELLANEOUS_QNATIVE_WINDOW_H

#include "MiscellaneousBase.h"

namespace Ui {class MiscellaneousQNativeWindow;};

class MiscellaneousQNativeWindow : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousQNativeWindow(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousQNativeWindow();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnCustomTimer_clicked();
    void on_btnDiffThread_clicked();
    void on_btnTemporaryThread_clicked();
    void on_btnEmptyTest_clicked();

private:
    Ui::MiscellaneousQNativeWindow*ui;
};

#endif // MISCELLANEOUS_QNATIVE_WINDOW_H
