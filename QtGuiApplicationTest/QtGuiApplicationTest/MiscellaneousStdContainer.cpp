#include "MiscellaneousStdContainer.h"
#include "ui_MiscellaneousStdContainer.h"

#include <sstream>
#include <queue>
#include <deque>

#include "LogUtil.h"

template<typename T>
void print_queue(T q)
{
    std::stringstream ss;
    while (!q.empty())
    {
        ss << q.top() << ' ';
        q.pop();
    }
    LogUtil::Debug(CODE_LOCATION, "PriorityQueue : %s", ss.str().c_str());
}

MiscellaneousStdContainer::MiscellaneousStdContainer(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousStdContainer())
{
    ui->setupUi(this);
}

MiscellaneousStdContainer::~MiscellaneousStdContainer()
{
    delete ui;
}

QString MiscellaneousStdContainer::GetTitle()
{
    return QObject::tr("STL Container");
}

QString MiscellaneousStdContainer::GetTitleTooltip()
{
    return QObject::tr("How to us standard template library container.");
}

MiscellaneousTestGroup MiscellaneousStdContainer::GetGroupID()
{
    return MiscellaneousTestGroup::Standard_Cpp;
}

MiscellaneousTestItem MiscellaneousStdContainer::GetItemID()
{
    return MiscellaneousTestItem::StdCpp_STL_Container;
}

void MiscellaneousStdContainer::on_btnVectorTest_clicked()
{
    int i = 0;
}

void MiscellaneousStdContainer::on_btnListTest_clicked()
{
    int i = 0;
}

void MiscellaneousStdContainer::on_btnStackTest_clicked()
{
    int i = 0;
}

void MiscellaneousStdContainer::on_btnQueueTest_clicked()
{
    int i = 0;
}

void MiscellaneousStdContainer::on_btnPriorityQueueTest_clicked()
{
    std::priority_queue<int> q;
    const auto data = { 1,8,5,6,3,4,0,9,7,2 };
    for (int n : data)
    {
        q.push(n);
    }
    print_queue(q);

    std::priority_queue<int, std::vector<int>, std::greater<int>> q2(data.begin(), data.end());
    print_queue(q2);

    auto cmp = [](int left, int right)
    {
        return (left ^ 1) < (right ^ 1);
    };
    std::priority_queue<int, std::vector<int>, decltype(cmp)> q3(cmp);
    for (int n : data)
    {
        q3.push(n);
    }
    print_queue(q3);
}

void MiscellaneousStdContainer::on_btnDequeueTest_clicked()
{
    int i = 0;
}

void MiscellaneousStdContainer::on_btnMapTest_clicked()
{
    std::pair<int, int> inputItem = std::make_pair(1, 2);
    std::pair<int, int> findItem = std::make_pair(1, 2);
    if (inputItem == findItem)
    {
        int i = 0;
    }
    else
    {
        int i = 0;
    }

    std::map<std::pair<int, int>, int> testMap;
    testMap.insert(std::make_pair(inputItem, 100));
    auto target = testMap.find(findItem);
    if (target == testMap.end())
    {
        int i = 0;
    } 
    else
    {
        int i = 0;
    }
}

void MiscellaneousStdContainer::on_btnPairTest_clicked()
{
    int i = 0;
}
