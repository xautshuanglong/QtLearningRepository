#ifndef MISCELLANEOUS_SQLCIPHER_H
#define MISCELLANEOUS_SQLCIPHER_H

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousSqlCipher.h"

class MiscellaneousSqlCipher : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousSqlCipher(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousSqlCipher();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    static int Sqlite3_Callback(void *notUsed, int argc, char **argv, char **azColName);

private slots:
    void on_btnInsert_clicked();
    void on_btnDelete_clicked();
    void on_btnUpdate_clicked();
    void on_btnSelect_clicked();

private:
    Ui::MiscellaneousSqlCipher ui;
    bool                       mShowHeaderFlag;
};

#endif MISCELLANEOUS_SQLCIPHER_H
