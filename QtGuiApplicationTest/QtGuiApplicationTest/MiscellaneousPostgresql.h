#ifndef MISCELLANEOUS_POSTGRESQL_H
#define MISCELLANEOUS_POSTGRESQL_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousPostgresql.h"

class MiscellaneousPostgresql : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousPostgresql(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousPostgresql();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnLibpqInsert_clicked();
    void on_btnLibpqDelete_clicked();
    void on_btnLibpqUpdate_clicked();
    void on_btnLibpqSelect_clicked();
    void on_btnLibpqxxInsert_clicked();
    void on_btnLibpqxxDelete_clicked();
    void on_btnLibpqxxUpdate_clicked();
    void on_btnLibpqxxSelect_clicked();

private:
    Ui::MiscellaneousPostgresql ui;
};

#endif MISCELLANEOUS_POSTGRESQL_H
