#include "MiscellaneousAlgorithmLeetCode.h"

using namespace std;

MiscellaneousAlgorithmLeetCode::MiscellaneousAlgorithmLeetCode(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
}

MiscellaneousAlgorithmLeetCode::~MiscellaneousAlgorithmLeetCode()
{
}

QString MiscellaneousAlgorithmLeetCode::GetTitle()
{
    return QObject::tr("LeetCode");
}

QString MiscellaneousAlgorithmLeetCode::GetTitleTooltip()
{
    return QObject::tr("LeetCode exercise.");
}

MiscellaneousTestGroup MiscellaneousAlgorithmLeetCode::GetGroupID()
{
    return MiscellaneousTestGroup::Algorithm;
}

MiscellaneousTestItem MiscellaneousAlgorithmLeetCode::GetItemID()
{
    return MiscellaneousTestItem::Algorithm_LeetCode;
}

void MiscellaneousAlgorithmLeetCode::on_btnNumOfIsland_1_clicked()
{
    //11110
    //11010
    //11000
    //00000
    // 1个
    vector<vector<char>> inputGrid1;
    inputGrid1.push_back(vector<char>({ '1', '1', '1', '1', '0' }));
    inputGrid1.push_back(vector<char>({ '1', '1', '0', '1', '0' }));
    inputGrid1.push_back(vector<char>({ '1', '1', '0', '0', '0' }));
    inputGrid1.push_back(vector<char>({ '0', '0', '0', '0', '0' }));

    int islandCount1 = this->LeetCode_100_NumberOfIsland(inputGrid1);

    //11000
    //11000
    //00100
    //00011
    // 3个
    vector<vector<char>> inputGrid2;
    inputGrid2.push_back(vector<char>({ '1', '1', '0', '0', '0' }));
    inputGrid2.push_back(vector<char>({ '1', '1', '0', '0', '0' }));
    inputGrid2.push_back(vector<char>({ '0', '0', '1', '0', '0' }));
    inputGrid2.push_back(vector<char>({ '0', '0', '0', '1', '1' }));

    int islandCount2 = this->LeetCode_100_NumberOfIsland(inputGrid2);

    int i = 0;
}

void MiscellaneousAlgorithmLeetCode::on_btnTest_clicked()
{
    int i = 0;
}

int MiscellaneousAlgorithmLeetCode::LeetCode_100_NumberOfIsland(std::vector<std::vector<char>>& grid)
{
    if (grid.size() == 0) return 0;

    // copy 同等规模矩阵
    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));

    int islandCount = 0;

    return islandCount;
}
