#ifndef MISCELLANEOUS_REGULAR_EXPRESSION_H
#define MISCELLANEOUS_REGULAR_EXPRESSION_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousRegularExpression.h"

class MiscellaneousRegularExpression : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousRegularExpression(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousRegularExpression();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    void LogFilenameMatchTest(const QString& targetStr);
    void LogFilenameCaptureTest(const QString& targetStr);

private slots:
    void on_btnFullMatch_clicked();
    void on_btnCaptureGroup_clicked();
    void on_btnDoMatch_clicked();

private:
    Ui::MiscellaneousRegularExpression ui;
};

#endif // MISCELLANEOUS_REGULAR_EXPRESSION_H
