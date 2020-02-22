#ifndef MISCELLANEOUS_C_LANGUAGE_FEATURES_H
#define MISCELLANEOUS_C_LANGUAGE_FEATURES_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousCLanguageFeatures.h"

class MiscellaneousCLanguageFeatures : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousCLanguageFeatures(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousCLanguageFeatures();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnIfCondition_clicked();

private:
    Ui::MiscellaneousCLanguageFeatures ui;
};

#endif // MISCELLANEOUS_C_LANGUAGE_FEATURES_H
