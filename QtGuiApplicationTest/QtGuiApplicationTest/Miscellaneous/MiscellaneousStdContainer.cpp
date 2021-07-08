#include "MiscellaneousStdContainer.h"
#include "ui_MiscellaneousStdContainer.h"

#include <sstream>
#include <queue>
#include <deque>
#include <utility>

#include "LogUtil.h"

template<class T>
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

void MiscellaneousStdContainer::PriorityQueueTest_BaseType()
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

void MiscellaneousStdContainer::PriorityQueueTest_CustomClass()
{
    InterObjPriorityQueue lessTest1(2, 1, 3, 3);
    InterObjPriorityQueue lessTest2(1, 2, 3, 4);
    bool resLess1 = lessTest1 < lessTest2;
    bool resLess2 = lessTest2 < lessTest1;

    InterObjPriorityQueue greatTest1(4, 3, 2, 1);
    InterObjPriorityQueue greatTest2(1, 2, 3, 4);
    bool resGreat1 = greatTest1 > greatTest2;
    bool resGreat2 = greatTest2 > greatTest1;

    InterObjPriorityQueue equalTest1(4, 3, 2, 1);
    InterObjPriorityQueue equalTest2(4, 3, 2, 1);
    InterObjPriorityQueue equalTest3(4, 3, 2, 2);
    bool resEqual1 = equalTest1 == equalTest2;
    bool resEqual2 = equalTest2 == equalTest3;

    InterObjPriorityQueue lessEqualTest1(4, 3, 2, 1);
    InterObjPriorityQueue lessEqualTest2(4, 3, 2, 2);
    InterObjPriorityQueue lessEqualTest3(4, 3, 2, 2);
    bool resLessEqual1 = lessEqualTest1 <= lessEqualTest2;
    bool resLessEqual2 = lessEqualTest2 <= lessEqualTest3;
    bool resLessEqual3 = lessEqualTest1 <= lessEqualTest3;
    bool resLessEqual4 = lessEqualTest3 <= lessEqualTest1;

    InterObjPriorityQueue greatEqualTest1(4, 3, 2, 2);
    InterObjPriorityQueue greatEqualTest2(4, 3, 2, 1);
    InterObjPriorityQueue greatEqualTest3(4, 3, 2, 1);
    bool resGreatEqual1 = greatEqualTest1 >= greatEqualTest2;
    bool resGreatEqual2 = greatEqualTest2 >= greatEqualTest3;
    bool resGreatEqual3 = greatEqualTest1 >= greatEqualTest3;
    bool resGreatEqual4 = greatEqualTest3 >= greatEqualTest1;
}

void MiscellaneousStdContainer::PriorityQueueTest_CustomClass_Sort()
{
    std::priority_queue<InterObjPriorityQueue, std::vector<InterObjPriorityQueue>, std::greater<InterObjPriorityQueue>> sortedQueue;
    sortedQueue.push(InterObjPriorityQueue(1, 2, 3, 4));
    sortedQueue.push(InterObjPriorityQueue(1, 2, 3, 5));
    sortedQueue.push(InterObjPriorityQueue(0, 2, 3, 4));
    sortedQueue.push(InterObjPriorityQueue(0, 1, 3, 4));
    sortedQueue.push(InterObjPriorityQueue(0, 3, 3, 4));
    sortedQueue.push(InterObjPriorityQueue(1, 2, 2, 4));
    sortedQueue.push(InterObjPriorityQueue(1, 2, 4, 4));
    sortedQueue.push(InterObjPriorityQueue(1, 2, 3, 5));
    print_queue(sortedQueue);
}

void MiscellaneousStdContainer::MapTest_CommonUse()
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

void MiscellaneousStdContainer::MapTest_CustomClassKey()
{
    std::unordered_map<std::string, std::string> stringKeyMap;
    stringKeyMap.insert(std::make_pair<std::string, std::string>("work", "hard"));
    stringKeyMap.insert(std::make_pair<std::string, std::string>("Hello", "world"));
    stringKeyMap.insert(std::make_pair<std::string, std::string>("Hello", "world_overwrite")); // 不会造成覆盖，查看 multimap
    for (auto pairItem : stringKeyMap)
    {
        std::string keyStr = pairItem.first;
        std::string valueStr = pairItem.second;
        LogUtil::Debug(CODE_LOCATION, "std::unordered_map<std::string, std::string>   %s = %s", keyStr.c_str(), valueStr.c_str());
    }

    InterObjPriorityQueue toStrTest(5, 4, 3, 2);
    std::string testStr = std::to_string(toStrTest);

    //std::map<InterObjPriorityQueue, std::string> customKeyMap_order; // std::less
    std::map<InterObjPriorityQueue, std::string, std::greater<InterObjPriorityQueue>> customKeyMap_order; // std::great
    customKeyMap_order.insert(std::make_pair<InterObjPriorityQueue, std::string>(InterObjPriorityQueue(1, 2, 3, 4), "Hello"));
    customKeyMap_order.insert(std::make_pair<InterObjPriorityQueue, std::string>(InterObjPriorityQueue(0, 2, 3, 4), "World"));
    customKeyMap_order.insert(std::make_pair<InterObjPriorityQueue, std::string>(InterObjPriorityQueue(3, 2, 3, 4), "Test"));
    customKeyMap_order.insert(std::make_pair<InterObjPriorityQueue, std::string>(InterObjPriorityQueue(3, 2, 3, 4), "Test_overwrite"));
    customKeyMap_order.insert(std::make_pair<InterObjPriorityQueue, std::string>(InterObjPriorityQueue(2, 2, 3, 4), "ordered"));
    for (auto pairItem : customKeyMap_order)
    {
        std::string keyStr = std::to_string(pairItem.first);
        std::string valueStr = pairItem.second;
        LogUtil::Debug(CODE_LOCATION, "std::<InterObjPriorityQueue, std::string>   %s = %s", keyStr.c_str(), valueStr.c_str());
    }

    //std::unordered_map<InterObjPriorityQueue, std::string> customKeyMap;
    //customKeyMap.insert(std::make_pair<InterObjPriorityQueue, std::string>(InterObjPriorityQueue(1, 2, 3, 4), "Hello"));
    //customKeyMap.insert(std::make_pair<InterObjPriorityQueue, std::string>(InterObjPriorityQueue(0, 2, 3, 4), "World"));
    //customKeyMap.insert(std::make_pair<InterObjPriorityQueue, std::string>(InterObjPriorityQueue(3, 2, 3, 4), "Test"));
    //customKeyMap.insert(std::make_pair<InterObjPriorityQueue, std::string>(InterObjPriorityQueue(3, 2, 3, 4), "Test_overwrite"));
    //for (auto pairItem : customKeyMap)
    //{
    //    std::string keyStr = std::to_string(pairItem.first);
    //    std::string valueStr = pairItem.second;
    //    LogUtil::Debug(CODE_LOCATION, "std::unordered_map<InterObjPriorityQueue, std::string>   %s = %s", keyStr.c_str(), valueStr.c_str());
    //}
}

void MiscellaneousStdContainer::on_btnPriorityQueueTest_clicked()
{
    //this->PriorityQueueTest_BaseType();
    //this->PriorityQueueTest_CustomClass();
    this->PriorityQueueTest_CustomClass_Sort();
}

void MiscellaneousStdContainer::on_btnDequeueTest_clicked()
{
    int i = 0;
}

void MiscellaneousStdContainer::on_btnMapTest_clicked()
{
    //this->MapTest_CommonUse();
    this->MapTest_CustomClassKey();
}

void MiscellaneousStdContainer::on_btnPairTest_clicked()
{
    int i = 0;
}

//====================================== InterObjPriorityQueue ====================================
//====================================== InterObjPriorityQueue ====================================
//====================================== InterObjPriorityQueue ====================================
//====================================== InterObjPriorityQueue ====================================

InterObjPriorityQueue::InterObjPriorityQueue() noexcept
    : m_iHour(0)
    , m_iMinute(0)
    , m_iSecond(0)
    , m_iFrame(0)
{}

InterObjPriorityQueue::InterObjPriorityQueue(int hour, int minute, int second, int frame) noexcept
    : m_iHour(hour)
    , m_iMinute(minute)
    , m_iSecond(second)
    , m_iFrame(frame)
{}

InterObjPriorityQueue::InterObjPriorityQueue(const InterObjPriorityQueue& other) noexcept
{
    m_iHour = other.m_iHour;
    m_iMinute = other.m_iMinute;
    m_iSecond = other.m_iSecond;
    m_iFrame = other.m_iFrame;
}

InterObjPriorityQueue::InterObjPriorityQueue(InterObjPriorityQueue&& other) noexcept
{
    m_iHour = std::move(other.m_iHour);
    m_iMinute = std::move(other.m_iMinute);
    m_iSecond = std::move(other.m_iSecond);
    m_iFrame = std::move(other.m_iFrame);
}

InterObjPriorityQueue::~InterObjPriorityQueue()
{}

InterObjPriorityQueue& InterObjPriorityQueue::operator=(const InterObjPriorityQueue &other)
{
    m_iHour = other.m_iHour;
    m_iMinute = other.m_iMinute;
    m_iSecond = other.m_iSecond;
    m_iFrame = other.m_iFrame;
    return *this;
}

bool InterObjPriorityQueue::operator <(const InterObjPriorityQueue& other) const
{
    if (m_iHour < other.m_iHour)
    {
        return true;
    }
    else if (m_iHour > other.m_iHour)
    {
        return false;
    }
    if (m_iMinute < other.m_iMinute)
    {
        return true;
    }
    else if (m_iMinute > other.m_iMinute)
    {
        return false;
    }
    if (m_iSecond < other.m_iSecond)
    {
        return true;
    }
    else if (m_iSecond > other.m_iSecond)
    {
        return false;
    }
    if (m_iFrame < other.m_iFrame)
    {
        return true;
    }
    return false;
}

bool InterObjPriorityQueue::operator >(const InterObjPriorityQueue& other) const
{
    if (m_iHour > other.m_iHour)
    {
        return true;
    }
    else if (m_iHour < other.m_iHour)
    {
        return false;
    }
    if (m_iMinute > other.m_iMinute)
    {
        return true;
    }
    else if (m_iMinute < other.m_iMinute)
    {
        return false;
    }
    if (m_iSecond > other.m_iSecond)
    {
        return true;
    }
    else if (m_iSecond < other.m_iSecond)
    {
        return false;
    }
    if (m_iFrame > other.m_iFrame)
    {
        return true;
    }
    return false;
}

bool InterObjPriorityQueue::operator ==(const InterObjPriorityQueue& other) const
{
    if (m_iHour == other.m_iHour &&
        m_iMinute == other.m_iMinute &&
        m_iSecond == other.m_iSecond &&
        m_iFrame == other.m_iFrame)
    {
        return true;
    }
    return false;
}

bool InterObjPriorityQueue::operator <=(const InterObjPriorityQueue& other) const
{
    if (*this > other)
    {
        return false;
    }
    return true;
}

bool InterObjPriorityQueue::operator >=(const InterObjPriorityQueue& other) const
{
    if (*this < other)
    {
        return false;
    }
    return true;
}

std::ostream& operator<<(std::ostream& output, const InterObjPriorityQueue& obj)
{
    output << obj.m_iHour << ":" << obj.m_iMinute << ":" << obj.m_iSecond << ":" << obj.m_iFrame;
    return output;
}

_STD_BEGIN
string to_string(const InterObjPriorityQueue& obj)
{
    std::string retValue = "";
    retValue += to_string(obj.GetHour());
    retValue += ":" + to_string(obj.GetMinute());
    retValue += ":" + to_string(obj.GetSecond());
    retValue += ":" + to_string(obj.GetFrame());
    return retValue;
}
_STD_END
