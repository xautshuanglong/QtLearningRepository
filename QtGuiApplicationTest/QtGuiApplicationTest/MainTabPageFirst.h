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
class DebugPanel;

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
    DebugPanel                     *mpDebugPanel;
    DicomWindow                    *pDcmWidget;
    MiscellaneousTesting           *mpMiscellaneousTest;
    WinReportTesting               *mpWinReportTest;
    MyWorkerThreadPool             *mpMyWorkerThreadPool;
    SL::Core::BackgroundWorkerTest *mpBackgroundWorker;

public:
    MainTabPageFirst(QWidget *parent = Q_NULLPTR);
    ~MainTabPageFirst();

protected:
    virtual bool event(QEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

private:
    void InitAppList();
    void ShowDicomImage(QString &dcmFileName);
    void ReadImageByQImage();
    void ReadImageByQImageMulti();
    void ReadJpegAndCopyToDicom();
    void BackupFunction();
    void SelfPaintImage8Bit();
    void BackgroundWorkerTest();
    void QThreadPoolTest();
    void MyThradPoolTest();

private slots:
    void on_btnBrowserDcm_clicked();
    void on_btnBrowserImg_clicked();
    void on_btnPrint_clicked();
};
