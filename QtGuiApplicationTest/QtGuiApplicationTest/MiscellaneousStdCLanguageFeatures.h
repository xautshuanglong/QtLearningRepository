#ifndef MISCELLANEOUS_STD_C_LANGUAGE_FEATURES_H
#define MISCELLANEOUS_STD_C_LANGUAGE_FEATURES_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousStdCLanguageFeatures.h"

class MiscellaneousStdCLanguageFeatures : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousStdCLanguageFeatures(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousStdCLanguageFeatures();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnIfCondition_clicked();

private:
    Ui::MiscellaneousStdCLanguageFeatures ui;
};

#endif // MISCELLANEOUS_STD_C_LANGUAGE_FEATURES_H
