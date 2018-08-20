#pragma once

#include <QWidget>
#include "ui_MainTabPageSetting.h"

class MainTabPageSetting : public QWidget
{
    Q_OBJECT

private:
    Ui::MainTabPageSetting ui;

public:
    MainTabPageSetting(QWidget *parent = Q_NULLPTR);
    ~MainTabPageSetting();
};
