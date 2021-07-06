#pragma once

#include <QMainWindow>
#include <QMap>
#include "MiscellaneousDefines.h"

class FramelessWindowHelper;
class QTreeWidgetItem;
class MiscellaneousBase;

namespace Ui
{
    class MiscellaneousTesting;
}

class MiscellaneousTesting : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MiscellaneousTesting                        *ui;
    FramelessWindowHelper                           *mpFramelessHelper;
    QMap<MiscellaneousTestGroup, QTreeWidgetItem*>   mMapTestGroup;
    QMap<MiscellaneousTestItem, int>                 mMapTestPageIndex;

public:
    explicit MiscellaneousTesting(QWidget *parent = Q_NULLPTR);
    ~MiscellaneousTesting();

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    void InitializeUI();
    void AppendTestPage(MiscellaneousBase* pWidgetPage);
    void ShowTestPage(QTreeWidgetItem *pItem);

signals:
    void SignalClosed();

private slots:
    void on_twMiscellaneousTesting_itemClicked(QTreeWidgetItem *item, int column);
};
