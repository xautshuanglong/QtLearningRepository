#pragma once

#include <QWidget>
#include "ui_MainTabPageFirst.h"
//#include <BackgroundWorkerTest.h>

class DicomWindow;
class MyBackgroundWorker;
class MyWorkerThreadPool;

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
    DicomWindow                    *pDcmWidget;
    MyWorkerThreadPool             *mpMyWorkerThreadPool;
    SL::Core::BackgroundWorkerTest *mpBackgroundWorker;

public:
    MainTabPageFirst(QWidget *parent = Q_NULLPTR);
    ~MainTabPageFirst();

protected:
    virtual bool event(QEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

private:
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
};
