#pragma once

#include <QWidget>
#include "ui_MainTabPageSetting.h"

class MainTabPageSetting : public QWidget
{
    Q_OBJECT

private:
    Ui::MainTabPageSetting ui;

private slots:
    void SlotListVScrollBarRangeChanged(int min, int max);
    void SlotListVScrollBarValueChanged(int value);
    void on_btnGeneratePDF_clicked();
    void on_btnParseDocument_clicked();
    void on_btnChangeItemCount_clicked();
    void on_btnShowVerticalScrollBar_clicked();

public:
    MainTabPageSetting(QWidget *parent = Q_NULLPTR);
    ~MainTabPageSetting();
};
