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

public:
    MainTabPageSetting(QWidget *parent = Q_NULLPTR);
    ~MainTabPageSetting();
};
