#pragma once

#include <QWidget>
#include "ui_MainTabPageFirst.h"

class MainTabPageFirst : public QWidget
{
    Q_OBJECT

private:
    Ui::MainTabPageFirst ui;

public:
    MainTabPageFirst(QWidget *parent = Q_NULLPTR);
    ~MainTabPageFirst();

protected:
    virtual bool event(QEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

private:
    void ShowDicomImage(QString &dcmFileName);
    void ReadJpegAndCopyToDicom();
    void BackupFunction();
    void SelfPaintImage8Bit();

private slots:
    void on_btnBrowserDcm_clicked();
    void on_btnBrowserImg_clicked();
};