#include "MiscellaneousAlgorithmLeetCode.h"

#include <algorithm>

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
        {42, std::bind(&MiscellaneousAlgorithmLeetCode::LeetCode_42_Entry, this), QStringLiteral("面试题： 连续子数组的最大和"),
        QStringLiteral("输入一个整型数组，数组里有正数也有负数。数组中的一个或连续多个整数组成一个子数组。求所有子数组的和的最大值。"
                       "\n"
                       "\n要求时间复杂度为O(n)。"
                       "\n"
                       "\n"
                       "\n示例1 :"
                       "\n"
                       "\n输入: nums = [-2,1,-3,4,-1,2,1,-5,4]"
                       "\n输出 : 6"
                       "\n解释 : 连续子数组[4,-1,2,1] 的和最大，为 6。"
                       "\n"
                       "\n"
                       "\n提示："
                       "\n"
                       "\n1 <= arr.length <= 10 ^ 5"
                       "\n-100 <= arr[i] <= 100")},
        {121, std::bind(&MiscellaneousAlgorithmLeetCode::LeetCode_121_Entry, this), QStringLiteral("买卖股票的最佳时机 I"),
        QStringLiteral("给定一个数组，它的第 i 个元素是一支给定股票第 i 天的价格。"
                       "\n如果你最多只允许完成一笔交易（即买入和卖出一支股票一次），设计一个算法来计算你所能获取的最大利润。"
                       "\n注意：你不能在买入股票前卖出股票。"
                       "\n"
                       "\n示例 1:"
                       "\n"
                       "\n输入: [7,1,5,3,6,4]"
                       "\n输出 : 5"
                       "\n解释 : 在第 2 天（股票价格 = 1）的时候买入，在第 5 天（股票价格 = 6）的时候卖出，最大利润 = 6 - 1 = 5 。"
                       "\n       注意利润不能是 7 - 1 = 6, 因为卖出价格需要大于买入价格；同时，你不能在买入前卖出股票。"
                       "\n"
                       "\n示例 2 :"
                       "\n"
                       "\n输入 : [7,6,4,3,1]"
                       "\n输出 : 0"
                       "\n解释 : 在这种情况下, 没有交易完成, 所以最大利润为 0。")},
        {122, std::bind(&MiscellaneousAlgorithmLeetCode::LeetCode_122_Entry, this), QStringLiteral("买卖股票的最佳时机 II"),
        QStringLiteral("给定一个数组，它的第?i 个元素是一支给定股票第 i 天的价格。"
                       "\n设计一个算法来计算你所能获取的最大利润。你可以尽可能地完成更多的交易（多次买卖一支股票）。"
                       "\n注意：你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。"
                       "\n"
                       "\n示例 1:"
                       "\n"
                       "\n输入: [7,1,5,3,6,4]"
                       "\n输出 : 7"
                       "\n解释 : 在第 2 天（股票价格 = 1）的时候买入，在第 3 天（股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5 - 1 = 4 。"
                       "\n       随后，在第 4 天（股票价格 = 3）的时候买入，在第 5 天（股票价格 = 6）的时候卖出, 这笔交易所能获得利润 = 6 - 3 = 3 。"
                       "\n"
                       "\n示例 2 :"
                       "\n"
                       "\n输入 : [1,2,3,4,5]"
                       "\n输出 : 4"
                       "\n解释 : 在第 1 天（股票价格 = 1）的时候买入，在第 5 天 （股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5 - 1 = 4 。"
                       "\n       注意你不能在第 1 天和第 2 天接连购买股票，之后再将它们卖出。"
                       "\n       因为这样属于同时参与了多笔交易，你必须在再次购买前出售掉之前的股票。"
                       "\n"
                       "\n示例 3 :"
                       "\n"
                       "\n输入 : [7,6,4,3,1]"
                       "\n输出 : 0"
                       "\n解释 : 在这种情况下, 没有交易完成, 所以最大利润为 0。"
                       "\n"
                       "\n提示："
                       "\n1 <= prices.length <= 3 * 10 ^ 4"
                       "\n0 <= prices[i] <= 10 ^ 4")},
        {123, std::bind(&MiscellaneousAlgorithmLeetCode::LeetCode_123_Entry, this), QStringLiteral("买卖股票的最佳时机 III"),
        QStringLiteral("给定一个数组，它的第 i 个元素是一支给定的股票在第 i 天的价格。"
                       "\n设计一个算法来计算你所能获取的最大利润。你最多可以完成 两笔 交易。"
                       "\n注意 : 你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。"
                       "\n"
                       "\n示例 1 :"
                       "\n"
                       "\n输入 : [3,3,5,0,0,3,1,4]"
                       "\n输出 : 6"
                       "\n解释 : 在第 4 天（股票价格 = 0）的时候买入，在第 6 天（股票价格 = 3）的时候卖出，这笔交易所能获得利润 = 3 - 0 = 3 。"
                       "\n       随后，在第 7 天（股票价格 = 1）的时候买入，在第 8 天 （股票价格 = 4）的时候卖出，这笔交易所能获得利润 = 4 - 1 = 3 。"
                       "\n"
                       "\n示例 2 :"
                       "\n"
                       "\n输入 : [1,2,3,4,5]"
                       "\n输出 : 4"
                       "\n解释 : 在第 1 天（股票价格 = 1）的时候买入，在第 5 天 （股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5 - 1 = 4 。"
                       "\n       注意你不能在第 1 天和第 2 天接连购买股票，之后再将它们卖出。"
                       "\n       因为这样属于同时参与了多笔交易，你必须在再次购买前出售掉之前的股票。"
                       "\n"
                       "\n示例 3 :"
                       "\n"
                       "\n输入 : [7,6,4,3,1]"
                       "\n输出 : 0"
                       "\n解释 : 在这个情况下, 没有交易完成, 所以最大利润为 0。")},
        {188, std::bind(&MiscellaneousAlgorithmLeetCode::LeetCode_188_Entry, this), QStringLiteral("买卖股票的最佳时机 IV"),
        QStringLiteral("给定一个数组，它的第 i 个元素是一支给定的股票在第 i 天的价格。设计一个算法来计算你所能获取的最大利润。你最多可以完成 k 笔交易。"
                       "\n注意: 你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。"
                       "\n\n示例 1 :"
                       "\n\n输入 : [2,4,1], k = 2"
                       "\n输出 : 2"
                       "\n解释 : 在第 1 天(股票价格 = 2) 的时候买入，在第 2 天(股票价格 = 4) 的时候卖出，这笔交易所能获得利润 = 4 - 2 = 2 。"
                       "\n\n示例 2 :"
                       "\n\n输入 : [3,2,6,5,0,3], k = 2"
                       "\n输出 : 7"
                       "\n解释 : 在第 2 天(股票价格 = 2) 的时候买入，在第 3 天(股票价格 = 6) 的时候卖出, 这笔交易所能获得利润 = 6 - 2 = 4 。随后，在第 5 天(股票价格 = 0) 的时候买入，在第 6 天(股票价格 = 3) 的时候卖出, 这笔交易所能获得利润 = 3 - 0 = 3 。")},
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
        tempItemText = QString("%1. %2").arg(leetCodeSubjects[i].No, 6).arg(leetCodeSubjects[i].Title);
        pTempItem = new QListWidgetItem(tempItemText, ui.lwSubjects);
        pTempItem->setData(Qt::UserRole, leetCodeSubjects[i].No);
        ui.lwSubjects->addItem(pTempItem);
    }

    ui.lwSubjects->setCurrentRow(0);
    ui.teSubject->setPlainText(leetCodeSubjects[0].Description);
}


void MiscellaneousAlgorithmLeetCode::LeetCode_42_Entry()
{
    std::vector<int> inputNum1 = { -2,1,-3,4,-1,2,1,-5,4 }; // 6
    int maxSum = this->LeetCode_42_MaxSubArray(inputNum1);

    std::vector<int> inputNum2 = { -2,1 }; // 1
    int maxSum2 = this->LeetCode_42_MaxSubArray(inputNum2);

    int i = 0;
}

int  MiscellaneousAlgorithmLeetCode::LeetCode_42_MaxSubArray(std::vector<int>& nums)
{
    int maxSum = 0;

    int numLen = nums.size();

    if (numLen > 0)
    {
        int newLen = 0;
        for (int i = 1; i < numLen; ++i)
        {
            if (nums[newLen] * nums[i-1] < 0)
            {
                ++newLen;
                nums[newLen] = nums[i];
            }
            else
            {
                nums[newLen] += nums[i];
            }
        }
        maxSum = nums[0];
        int index = 0;
        if (maxSum < 0 && newLen > 1)
        {
            maxSum = nums[1];
            ++index;
        }
        int tempNum = 0, tempMax = maxSum;
        for (int i = index + 1; i < newLen; i+=2)
        {
            tempNum = nums[i] * -1;
            if (i + 1 < newLen && tempNum < maxSum && tempNum < nums[i = 1])
            {
                tempMax += nums[i];
                tempMax += nums[i+1];
            }
            else
            {
                tempMax = nums[i + 1];
            }

        }
    }

    // 枚举 耗时长
    //int inputLen = nums.size();
    //if (inputLen > 0)
    //{
    //    maxSum = nums[0];
    //    int tempSum = 0;
    //    for (int step = 1; step <= inputLen; ++step)
    //    {
    //        for (int j = 0; j <= inputLen - step; ++j)
    //        {
    //            tempSum = 0;
    //            for (int k = 0; k < step; ++k)
    //            {
    //                tempSum += nums[j + k];
    //            }
    //            if (tempSum > maxSum)
    //            {
    //                maxSum = tempSum;
    //            }
    //        }
    //    }
    //}

    return maxSum;
}

void MiscellaneousAlgorithmLeetCode::LeetCode_121_Entry()
{
    // 输入: [7, 1, 5, 3, 6, 4]
    // 输出 : 5
    std::vector<int> priceList1 = { 7, 1, 5, 3, 6, 4 };
    int maxProfit1 = this->LeetCode_121_MaxProfit(priceList1);

    // 输入: [7,6,4,3,1]
    // 输出: 0
    std::vector<int> priceList2 = { 7,6,4,3,1 };
    int maxProfit2 = this->LeetCode_121_MaxProfit(priceList2);

    int i = 0;
}

int MiscellaneousAlgorithmLeetCode::LeetCode_121_MaxProfit(std::vector<int>& prices)
{
    int min = 0x7FFFFFFF;
    int retValue = 0;
    int dayCount = prices.size();
    if (dayCount > 0)
    {
        min = prices[0];
        for (int i = 1; i < dayCount; ++i)
        {
            if (prices[i] < min)
            {
                min = prices[i];
                ++i;
            }
            if (prices[i] - min > retValue)
            {
                retValue = prices[i] - min;
            }
        }
    }
    return retValue;

    //int min = 0x7FFFFFFF;
    //int retValue = 0;
    //int dayCount = prices.size();
    //for (int i = 0; i < dayCount; ++i)
    //{
    //    if (prices[i] < min)
    //    {
    //        min = prices[i];
    //    }
    //    if (prices[i] - min > retValue)
    //    {
    //        retValue = prices[i] - min;
    //    }
    //}
    //return retValue;
}

void MiscellaneousAlgorithmLeetCode::LeetCode_122_Entry()
{
    // 输入: [7, 1, 5, 3, 6, 4]
    // 输出 : 7
    std::vector<int> priceList1 = { 7, 1, 5, 3, 6, 4 };
    int maxProfit1 = this->LeetCode_122_MaxProfit(priceList1);

    // 输入: [1, 2, 3, 4, 5]
    // 输出 : 4
    std::vector<int> priceList2 = { 1, 2, 3, 4, 5 };
    int maxProfit2 = this->LeetCode_122_MaxProfit(priceList2);

    // 输入: [7, 6, 4, 3, 1]
    // 输出 : 0
    std::vector<int> priceList3 = { 7, 6, 4, 3, 1 };
    int maxProfit3 = this->LeetCode_122_MaxProfit(priceList3);
}

int MiscellaneousAlgorithmLeetCode::LeetCode_122_MaxProfit(std::vector<int>& prices)
{
    int retValue = 0;
    int inputCount = prices.size();
    for (int i = 0; i < inputCount - 1; ++i)
    {
        prices[i] = prices[i + 1] - prices[i];
        if (prices[i] > 0)
        {
            retValue += prices[i];
        }
    }
    return retValue;
}

void MiscellaneousAlgorithmLeetCode::LeetCode_123_Entry()
{
    // 输入: [3,3,5,0,0,3,1,4]
    // 输出: 6
    //std::vector<int> priceList1 = { 3,3,5,0,0,3,1,4 };
    std::vector<int> priceList1 = { 1, 2, 4, 2, 5, 7, 2, 4, 9, 0 };
    int maxProfit1 = this->LeetCode_123_MaxProfit(priceList1);

    // 输入: [1,2,3,4,5]
    // 输出: 4
    std::vector<int> priceList2 = { 1,2,3,4,5 };
    int maxProfit2 = this->LeetCode_123_MaxProfit(priceList2);

    // 输入: [7, 6, 4, 3, 1]
    // 输出 : 0
    std::vector<int> priceList3 = { 7, 6, 4, 3, 1 };
    int maxProfit3 = this->LeetCode_123_MaxProfit(priceList3);
}

int MiscellaneousAlgorithmLeetCode::LeetCode_123_MaxProfit(std::vector<int>& prices)
{
    int firstBig = 0, secondBig = 0, tempBig = 0;

    int inputCount = prices.size();

    // 求差
    std::vector<int> diffVector;
    for (int i = 0; i < inputCount - 1; ++i)
    {
        diffVector.push_back(prices[i + 1] - prices[i]);
    }
    // 同号合并
    int multiple = 0;
    std::vector<int> unionVector;
    unionVector.push_back(diffVector[0]);
    int unionCount = 1;
    int diffCount = diffVector.size();
    for (int i = 1; i < diffCount; ++i)
    {
        multiple = unionVector[unionCount - 1] * diffVector[i];
        if (multiple < 0)
        {
            // 相邻异号
            unionVector.push_back(diffVector[i]);
            ++unionCount;
        }
        else
        {
            unionVector[unionCount - 1] = unionVector[unionCount - 1] + diffVector[i];
        }
    }
    // 合并最大利益
    int tempUnionThree = 0;
    for (int i = 0, j = 1; j < unionCount; ++j)
    {
        tempUnionThree = unionVector[j - 1] + unionVector[j] + unionVector[j + 1];
        if (unionVector[j] < 0 && tempUnionThree > unionVector[j - 1] && tempUnionThree > unionVector[j + 1]);
        {
        }
    }

    return firstBig + secondBig;
}

void MiscellaneousAlgorithmLeetCode::LeetCode_188_Entry()
{
    // 输入 : [2,4,1], k = 2
    // 输出 : 2
    int k1 = 2;
    std::vector<int> priceList1 = { 2, 4, 1 };
    this->LeetCode_188_MaxProfit(k1, priceList1);

    // 输入 : [3,2,6,5,0,3], k = 2
    // 输出 : 7
    int k2 = 2;
    std::vector<int> priceList2 = { 3, 2, 6, 5, 0, 3 };
    this->LeetCode_188_MaxProfit(k2, priceList2);
}

int MiscellaneousAlgorithmLeetCode::LeetCode_188_MaxProfit(int k, std::vector<int>& prices)
{
    int retValue = 0;

    int transactionCount = std::min(int(prices.size() / 2), k % 2 == 0 ? k : k - 1);
    for (int i = 0; i < transactionCount; ++i)
    {
        ; //
    }

    return retValue;
}

void MiscellaneousAlgorithmLeetCode::LeetCode_200_Entry()
{
    //11110
    //11010
    //11000
    //00000
    // 1个
    std::vector<std::vector<char>> inputGrid1;
    inputGrid1.push_back(std::vector<char>({ '1', '1', '1', '1', '0' }));
    inputGrid1.push_back(std::vector<char>({ '1', '1', '0', '1', '0' }));
    inputGrid1.push_back(std::vector<char>({ '1', '1', '0', '0', '0' }));
    inputGrid1.push_back(std::vector<char>({ '0', '0', '0', '0', '0' }));

    //11000
    //11000
    //00100
    //00011
    // 3个
    std::vector<std::vector<char>> inputGrid2;
    inputGrid2.push_back(std::vector<char>({ '1', '1', '0', '0', '0' }));
    inputGrid2.push_back(std::vector<char>({ '1', '1', '0', '0', '0' }));
    inputGrid2.push_back(std::vector<char>({ '0', '0', '1', '0', '0' }));
    inputGrid2.push_back(std::vector<char>({ '0', '0', '0', '1', '1' }));

    int islandCount1 = this->LeetCode_200_NumberOfIsland_LowMemery(inputGrid1);
    int islandCount2 = this->LeetCode_200_NumberOfIsland_LowMemery(inputGrid2);
    int islandCount3 = this->LeetCode_200_NumberOfIsland_LowTime(inputGrid1);
    int islandCount4 = this->LeetCode_200_NumberOfIsland_LowTime(inputGrid2);

    int i = 0;
}

int MiscellaneousAlgorithmLeetCode::LeetCode_200_NumberOfIsland_LowMemery(std::vector<std::vector<char>>& grid)
{
    if (grid.size() == 0) return 0;

    struct GridCell 
    {
        int row;
        int col;
    };
    // copy 同等规模矩阵
    std::vector<std::vector<bool>> visited(grid.size(), std::vector<bool>(grid[0].size(), false));
    std::vector<GridCell> islandVec;
    int islandCount = 0;
    int maxRow = grid.size();
    int maxCol = grid[0].size();

    GridCell nextCell = { 0, 0 };
    GridCell tempCell = { 0, 0 };
    int tempRow = 0, tempCol = 0;

    for (int row = 0; row < maxRow; ++row)
    {
        for (int col = 0; col < maxCol; ++col)
        {
            if (false == visited[row][col])
            {
                visited[row][col] = true;
                if ('1' == grid[row][col])
                {
                    ++islandCount;

                    tempCell.row = row;
                    tempCell.col = col;
                    islandVec.push_back(tempCell);

                    while (islandVec.size() > 0)
                    {
                        tempCell = islandVec.front();
                        islandVec.erase(islandVec.begin());
                        // 上侧
                        tempRow = tempCell.row - 1;
                        tempCol = tempCell.col;
                        if (tempRow >= 0 && false == visited[tempRow][tempCol] && '1' == grid[tempRow][tempCol])
                        {
                            nextCell.row = tempRow;
                            nextCell.col = tempCol;
                            islandVec.push_back(nextCell);
                            visited[tempRow][tempCol] = true;
                        }
                        // 右侧
                        tempRow = tempCell.row;
                        tempCol = tempCell.col + 1;
                        if (tempCol < maxCol && false == visited[tempRow][tempCol] && '1' == grid[tempRow][tempCol])
                        {
                            nextCell.row = tempRow;
                            nextCell.col = tempCol;
                            islandVec.push_back(nextCell);
                            visited[tempRow][tempCol] = true;
                        }
                        // 下侧
                        tempRow = tempCell.row + 1;
                        tempCol = tempCell.col;
                        if (tempRow < maxRow && false == visited[tempRow][tempCol] && '1' == grid[tempRow][tempCol])
                        {
                            nextCell.row = tempRow;
                            nextCell.col = tempCol;
                            islandVec.push_back(nextCell);
                            visited[tempRow][tempCol] = true;
                        }
                        // 左侧
                        tempRow = tempCell.row;
                        tempCol = tempCell.col - 1;
                        if (tempCol >= 0 && false == visited[tempRow][tempCol] && '1' == grid[tempRow][tempCol])
                        {
                            nextCell.row = tempRow;
                            nextCell.col = tempCol;
                            islandVec.push_back(nextCell);
                            visited[tempRow][tempCol] = true;
                        }
                    }
                }
            }
        }
    }

    return islandCount;
}

// breadth first search
// depth first search
void dfs(std::vector<std::vector<char>>& grid, int i, int j)
{
    if (i < 0 || i >= grid.size() ||
        j < 0 || j >= grid[0].size() ||
        grid[i][j] != '1')
        return;
    grid[i][j] = '2';
    dfs(grid, i + 1, j);
    dfs(grid, i - 1, j);
    dfs(grid, i, j - 1);
    dfs(grid, i, j + 1);
}

int MiscellaneousAlgorithmLeetCode::LeetCode_200_NumberOfIsland_LowTime(std::vector<std::vector<char>>& grid)
{
    int res = 0;
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++)
        {
            if (grid[i][j] == '1')
            {
                res++;
                dfs(grid, i, j);
            }
        }
    }
    return res;
}

void MiscellaneousAlgorithmLeetCode::LeetCode_201_Entry()
{
    this->LeetCode_201_Unknown();
}

void MiscellaneousAlgorithmLeetCode::LeetCode_201_Unknown()
{
    int i = 0;
}
