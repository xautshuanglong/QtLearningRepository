#include "MainTabPageSetting.h"

MainTabPageSetting::MainTabPageSetting(QWidget *parent /* = Q_NULLPTR */)
    : QWidget(parent)
{
    ui.setupUi(this);

    ui.settingTxt->setText("setting ...");
}

MainTabPageSetting::~MainTabPageSetting()
{
}
