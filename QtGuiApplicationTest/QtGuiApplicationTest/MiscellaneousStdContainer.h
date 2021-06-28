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

#endif // MISCELLANEOUS_STD_CONTAINER_H
