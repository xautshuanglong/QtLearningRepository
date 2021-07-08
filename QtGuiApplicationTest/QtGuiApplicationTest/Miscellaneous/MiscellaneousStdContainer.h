#ifndef MISCELLANEOUS_STD_CONTAINER_H
#define MISCELLANEOUS_STD_CONTAINER_H

#include "MiscellaneousBase.h"

namespace Ui
{
    class MiscellaneousStdContainer;
};

class MiscellaneousStdContainer : public MiscellaneousBase
{
    Q_OBJECT

public:
    MiscellaneousStdContainer(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousStdContainer();

    virtual QString GetTitle() override;
    virtual QString GetTitleTooltip() override;
    virtual MiscellaneousTestGroup GetGroupID() override;
    virtual MiscellaneousTestItem GetItemID() override;

private:
    void PriorityQueueTest_BaseType();
    void PriorityQueueTest_CustomClass();
    void PriorityQueueTest_CustomClass_Sort();
    void MapTest_CommonUse();
    void MapTest_CustomClassKey();

private slots:
    void on_btnVectorTest_clicked();
    void on_btnListTest_clicked();
    void on_btnStackTest_clicked();
    void on_btnQueueTest_clicked();
    void on_btnPriorityQueueTest_clicked();
    void on_btnDequeueTest_clicked();
    void on_btnMapTest_clicked();
    void on_btnPairTest_clicked();

private:
    Ui::MiscellaneousStdContainer*ui;
};

class InterObjPriorityQueue
{
public:
    InterObjPriorityQueue() noexcept;
    InterObjPriorityQueue(int hour, int minute, int second, int frame) noexcept;
    InterObjPriorityQueue(const InterObjPriorityQueue& other) noexcept;
    InterObjPriorityQueue(InterObjPriorityQueue&& other) noexcept;
    ~InterObjPriorityQueue();

    InterObjPriorityQueue& operator=(const InterObjPriorityQueue& other);

    bool operator <(const InterObjPriorityQueue& other) const;
    bool operator >(const InterObjPriorityQueue& other) const;
    bool operator ==(const InterObjPriorityQueue& other) const;
    bool operator <=(const InterObjPriorityQueue& other) const;
    bool operator >=(const InterObjPriorityQueue& other) const;

    friend std::ostream &operator<<(std::ostream &output, const InterObjPriorityQueue &obj);
    //friend std::string std::to_string(const InterObjPriorityQueue& obj);

    int SetHour(int hour) { m_iHour = hour; }
    int SetMinute(int minute) { m_iMinute = minute; }
    int SetSecond(int second) { m_iSecond = second; }
    int SetFrame(int frame) { m_iFrame = frame; }

    int GetHour() const { return m_iHour; }
    int GetMinute() const { return m_iMinute; }
    int GetSecond() const { return m_iSecond; }
    int GetFrame() const { return m_iFrame; }

private:
    int m_iHour;
    int m_iMinute;
    int m_iSecond;
    int m_iFrame;
};

_STD_BEGIN
string to_string(const InterObjPriorityQueue& obj);
_STD_END

// https://blog.csdn.net/y109y/article/details/82669620
// 方案一：使用仿函数包装 hash 计算函数（为了达到 operator() 的效果）
size_t InterObj_Hash(const InterObjPriorityQueue& obj);
// 方案二: 自定义 hash 就算方法
class InterObjHasher // : public std::hash<InterObjPriorityQueue>
{
public:
    size_t operator()(const InterObjPriorityQueue& obj) const
    {
        std::hash<int> intHash;
        return
            intHash(obj.GetHour()) ^
            intHash(obj.GetMinute()) ^
            intHash(obj.GetSecond()) ^
            intHash(obj.GetFrame());
    }
};
// 方案三: 特化 std::hash<T>
_STD_BEGIN
template<>
class hash<InterObjPriorityQueue>
{
public:
    size_t operator()(const InterObjPriorityQueue& obj) const
    {
        return
            hash<int>()(obj.GetHour()) ^
            hash<int>()(obj.GetMinute()) ^
            hash<int>()(obj.GetSecond()) ^
            hash<int>()(obj.GetFrame());
    }
};
_STD_END

#endif // MISCELLANEOUS_STD_CONTAINER_H
