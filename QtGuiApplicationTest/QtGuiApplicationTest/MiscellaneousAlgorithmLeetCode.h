#ifndef MISCELLANEOUS_ALGORITHM_LEETCODE_H
#define MISCELLANEOUS_ALGORITHM_LEETCODE_H

#include <vector>

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousAlgorithmLeetCode.h"

class MiscellaneousAlgorithmLeetCode : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousAlgorithmLeetCode(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousAlgorithmLeetCode();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_btnNumOfIsland_1_clicked();
    void on_btnTest_clicked();

private:
    int LeetCode_100_NumberOfIsland(std::vector<std::vector<char>>& grid);

private:
    Ui::MiscellaneousAlgorithmLeetCode ui;
};

#endif // MISCELLANEOUS_ALGORITHM_LEETCODE_H
