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

void MiscellaneousStdContainer::PriorityQueueTest_1()
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
    InterObjPriorityQueue lessTest1(1, 2, 3, 3);
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

void MiscellaneousStdContainer::on_btnPriorityQueueTest_clicked()
{
    //this->PriorityQueueTest_1();
    this->PriorityQueueTest_CustomClass();
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

InterObjPriorityQueue::InterObjPriorityQueue(InterObjPriorityQueue& other) noexcept
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

bool InterObjPriorityQueue::operator <(const InterObjPriorityQueue& other)
{
    if (m_iHour < other.m_iHour)
    {
        return true;
    }
    if (m_iMinute < other.m_iMinute)
    {
        return true;
    }
    if (m_iSecond < other.m_iSecond)
    {
        return true;
    }
    if (m_iFrame < other.m_iFrame)
    {
        return true;
    }
    return false;
}

bool InterObjPriorityQueue::operator >(const InterObjPriorityQueue& other)
{
    if (m_iHour > other.m_iHour)
    {
        return true;
    }
    if (m_iMinute > other.m_iMinute)
    {
        return true;
    }
    if (m_iSecond > other.m_iSecond)
    {
        return true;
    }
    if (m_iFrame > other.m_iFrame)
    {
        return true;
    }
    return false;
}

bool InterObjPriorityQueue::operator ==(InterObjPriorityQueue& other)
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

bool InterObjPriorityQueue::operator ==(const InterObjPriorityQueue& other)
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

bool InterObjPriorityQueue::operator <=(const InterObjPriorityQueue& other)
{
    if (*this > other)
    {
        return false;
    }
    return true;
}

bool InterObjPriorityQueue::operator >=(const InterObjPriorityQueue& other)
{
    if (*this < other)
    {
        return false;
    }
    return true;
}

std::ostream& InterObjPriorityQueue::operator<<(std::ostream& output)
{
    output << m_iHour << ":" << m_iMinute << ":" << m_iSecond << ":" << m_iFrame;
    return output;
}
