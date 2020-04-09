#ifndef MISCELLANEOUS_ALGORITHM_LEETCODE_H
#define MISCELLANEOUS_ALGORITHM_LEETCODE_H

#include <vector>
#include <functional>
#include <QMap>

#include "MiscellaneousBase.h"
#include "ui_MiscellaneousAlgorithmLeetCode.h"

class MiscellaneousAlgorithmLeetCode : public MiscellaneousBase
{
private:
    struct LeetCodeSubject
    {
        int No;
        std::function<void()> entryFunction;
        QString Title;
        QString Description;
    };

    Q_OBJECT

public:
    MiscellaneousAlgorithmLeetCode(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousAlgorithmLeetCode();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private slots:
    void on_lwSubjects_itemClicked(QListWidgetItem *item);
    void on_lwSubjects_itemDoubleClicked(QListWidgetItem *item);

private:
    void InitializeUI();

    void LeetCode_200_Entry();
    int  LeetCode_200_NumberOfIsland_LowMemery(std::vector<std::vector<char>>& grid);
    int  LeetCode_200_NumberOfIsland_LowTime(std::vector<std::vector<char>>& grid);
    void LeetCode_201_Entry();
    void LeetCode_201_Unknown();

private:
    Ui::MiscellaneousAlgorithmLeetCode ui;
    QMap<int, LeetCodeSubject>         mLeetCodeSubjectInfo;
};

#endif // MISCELLANEOUS_ALGORITHM_LEETCODE_H
