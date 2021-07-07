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
    InterObjPriorityQueue(InterObjPriorityQueue& other) noexcept;
    InterObjPriorityQueue(InterObjPriorityQueue&& other) noexcept;
    ~InterObjPriorityQueue();

    InterObjPriorityQueue& operator=(const InterObjPriorityQueue& other);

    bool operator <(const InterObjPriorityQueue& other) const;
    bool operator >(const InterObjPriorityQueue& other) const;
    bool operator ==(const InterObjPriorityQueue& other) const;
    bool operator <=(const InterObjPriorityQueue& other) const;
    bool operator >=(const InterObjPriorityQueue& other) const;
    friend std::ostream &operator<<(std::ostream &output, const InterObjPriorityQueue &obj);

    int m_iHour;
    int m_iMinute;
    int m_iSecond;
    int m_iFrame;
};


#endif // MISCELLANEOUS_STD_CONTAINER_H
