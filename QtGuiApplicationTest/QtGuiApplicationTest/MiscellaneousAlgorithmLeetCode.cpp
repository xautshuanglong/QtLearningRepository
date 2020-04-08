#include "MiscellaneousAlgorithmLeetCode.h"

using namespace std;

MiscellaneousAlgorithmLeetCode::MiscellaneousAlgorithmLeetCode(QWidget *parent)
    : MiscellaneousBase(parent)
{
    ui.setupUi(this);
    this->InitializeUI();
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

void MiscellaneousAlgorithmLeetCode::on_lwSubjects_itemClicked(QListWidgetItem *item)
{
    int subjectNo = item->data(Qt::UserRole).toInt();
    if (mLeetCodeSubjectInfo.contains(subjectNo))
    {
        ui.teSubject->setPlainText(mLeetCodeSubjectInfo[subjectNo].Description);
    }
}

void MiscellaneousAlgorithmLeetCode::on_lwSubjects_itemDoubleClicked(QListWidgetItem *item)
{
    int subjectNo = item->data(Qt::UserRole).toInt();
    if (mLeetCodeSubjectInfo.contains(subjectNo) && mLeetCodeSubjectInfo[subjectNo].entryFunction != nullptr)
    {
        mLeetCodeSubjectInfo[subjectNo].entryFunction();
    }
}

void MiscellaneousAlgorithmLeetCode::InitializeUI()
{
    LeetCodeSubject leetCodeSubjects[] =
    {
        {200, std::bind(&MiscellaneousAlgorithmLeetCode::LeetCode_200_Entry, this), QStringLiteral("岛屿数量"),
        QStringLiteral("给定一个由 '1'（陆地）和 '0'（水）组成的的二维网格，计算岛屿的数量。一个岛被水包围，并且它是通过水平方向或垂直方向上相邻的陆地连接而成的。你可以假设网格的四个边均被水包围。"
                       "\n\n示例 1:"
                       "\n\n输入:"
                       "\n\t11110"
                       "\n\t11010"
                       "\n\t11000"
                       "\n\t00000"
                       "\n\n输出 : 1"
                       "\n\n示例 2 :"
                       "\n输入 :"
                       "\n\t11000"
                       "\n\t11000"
                       "\n\t00100"
                       "\n\t00011"
                       "\n\n输出 : 3")},
        {201, std::bind(&MiscellaneousAlgorithmLeetCode::LeetCode_201_Entry, this), "Unknown", "Description"}
    };
    int subjectCount = sizeof(leetCodeSubjects) / sizeof(LeetCodeSubject);

    QListWidgetItem *pTempItem = Q_NULLPTR;
    QString tempItemText = "";

    QFontMetrics metrics(ui.teSubject->font());
    ui.teSubject->setTabStopWidth(4 * metrics.width(' '));

    ui.lwSubjects->clear();
    mLeetCodeSubjectInfo.clear();

    for (int i = 0; i < subjectCount; ++i)
    {
        mLeetCodeSubjectInfo.insert(leetCodeSubjects[i].No, leetCodeSubjects[i]);
        tempItemText = QString("%1. %2").arg(leetCodeSubjects[i].No, 8).arg(leetCodeSubjects[i].Title);
        pTempItem = new QListWidgetItem(tempItemText, ui.lwSubjects);
        pTempItem->setData(Qt::UserRole, leetCodeSubjects[i].No);
        ui.lwSubjects->addItem(pTempItem);
    }

    ui.lwSubjects->setCurrentRow(0);
    ui.teSubject->setPlainText(leetCodeSubjects[0].Description);
}

void MiscellaneousAlgorithmLeetCode::LeetCode_200_Entry()
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

    int islandCount1 = this->LeetCode_200_NumberOfIsland(inputGrid1);

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

    int islandCount2 = this->LeetCode_200_NumberOfIsland(inputGrid2);

    int i = 0;
}

int MiscellaneousAlgorithmLeetCode::LeetCode_200_NumberOfIsland(std::vector<std::vector<char>>& grid)
{
    if (grid.size() == 0) return 0;

    // copy 同等规模矩阵
    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));

    int islandCount = 0;

    return islandCount;
}

void MiscellaneousAlgorithmLeetCode::LeetCode_201_Entry()
{
    this->LeetCode_201_Unknown();
}

void MiscellaneousAlgorithmLeetCode::LeetCode_201_Unknown()
{
    int i = 0;
}
