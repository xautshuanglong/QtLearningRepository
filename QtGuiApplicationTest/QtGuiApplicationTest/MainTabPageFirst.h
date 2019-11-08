#pragma once

#include <QWidget>
#include "ui_MainTabPageFirst.h"
//#include <BackgroundWorkerTest.h>

class DicomWindow;
class MiscellaneousTesting;
class WinReportTesting;
class MyBackgroundWorker;
class MyWorkerThreadPool;
class QListWidgetItem;
class QStandardItemModel;

namespace SL
{
    namespace Core
    {
        class BackgroundWorkerTest;
    }
}

class MainTabPageFirst : public QWidget
{
    Q_OBJECT
private:
    Ui::MainTabPageFirst            ui;
    QListWidgetItem                *mpCurEnteredItem;
    DicomWindow                    *mpDcmWidget;
    MiscellaneousTesting           *mpMiscellaneousTest;
    WinReportTesting               *mpWinReportTest;
    MyWorkerThreadPool             *mpMyWorkerThreadPool;
    QStandardItemModel             *mpModelAppListItem;
    SL::Core::BackgroundWorkerTest *mpBackgroundWorker;

public:
    MainTabPageFirst(QWidget *parent = Q_NULLPTR);
    ~MainTabPageFirst();

protected:
    virtual bool event(QEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    void InitAppListView();
    void InitAppListWidget();
    //void ShowDicomImage(QString &dcmFileName);
    //void ReadImageByQImage();
    //void ReadImageByQImageMulti();
    //void ReadJpegAndCopyToDicom();
    //void BackupFunction();
    //void SelfPaintImage8Bit();
    void BackgroundWorkerTest();
    void QThreadPoolTest();
    //void MyThradPoolTest();

private slots:
    // QListWidget signals handlers
    void SlotListWidgetItemClicked(QListWidgetItem *pItem);
    void SlotListWidgetItemDoubleClicked(QListWidgetItem *pItem);
    void SlotListWidgetCurrentItemChanged(QListWidgetItem *pItemCurrent, QListWidgetItem *pItemPrevious);
    void SlotListWidgetCurrentRowChanged(int currentRow);
    // QListView signals handlers
    void SlotListViewItemClicked(const QModelIndex &index);
    void SlotListViewItemDoubleClicked(const QModelIndex &index);
    void SlotListViewItemPressed(const QModelIndex & index);
    // UI controls handlers
    void on_btnBrowserDcm_clicked();
    void on_btnBrowserImg_clicked();
    void on_btnPrint_clicked();
    // Custom Slots
    void SlotDicomWindowClosed();
    void SlotMiscellaneousWindowClosed();
};
